#include <stdio.h>
#include <pthread.h>
#include <stdbool.h>
#include <stdint.h>
#include <assert.h>
#include <string.h>
#include <stdarg.h>
#include <stdlib.h>
#include "map.h"
#include "list.h"

FILE *log_file = NULL;

void vlogf(const char *level, const char *message, va_list args) {
	fprintf(log_file, "[%s] ", level);
	vfprintf(log_file, message, args);
	fprintf(log_file, "\n");
	fflush(log_file);
}

void info(const char *fmt, ...) {
	va_list args;
	va_start(args, fmt);
	vlogf("INFO", fmt, args);
	va_end(args);
}

list_t *get_maps(void) {
	list_t *maps = NULL;

	// Read /proc/self/maps to get the loaded maps
	char maps_path[256];
	snprintf(maps_path, sizeof(maps_path), "/proc/self/maps");

	FILE *maps_file = fopen(maps_path, "r");
	assert(maps_file != NULL);

	char line[1024];
	while (fgets(line, sizeof(line), maps_file)) {
		char *path = strchr(line, '/'); // first '/' indicates start of path if any
		if (path) {
			// Check if path contains ".so" (shared object file)
			if (strstr(path, ".so")) {
				// Add path to the list of maps
				char *newline = strchr(path, '\n');
				if (newline) *newline = '\0';

				map_t *map = malloc(sizeof(map_t));
				assert(map != NULL);

				assert(strlen(path) < 256);
				strcpy(map->path, path);

				assert(strchr(map->path, '/'));
				assert(strchr(map->path, '/')[1] != '\0');
				assert(strlen(map->path) < 256);
				strcpy(map->name, strrchr(map->path, '/') + 1);

				sscanf(line, "%p-%p", &map->start, &map->end);

				list_push_back(&maps, map);
			}
		}
	}

	fclose(maps_file);
	return maps;
}

list_t *scan_map(map_t *map, uint8_t *data, size_t len) {
	list_t *found = NULL;

	// Scan the map for the specified data
	uint8_t *start = (uint8_t *)map->start;
	uint8_t *end = (uint8_t *)map->end;

	for (uint8_t *ptr = start; ptr < end - len; ptr++) {
		if (memcmp(ptr, data, len) == 0) {
			// Found a match, add to the list
			list_push_back_ex(&found, ptr, NULL);
		}
	}

	return found;
}

void thread(void) {
	log_file = stderr;
	assert(log_file != NULL);

	info("Initializing hack");
	info("Sleeping for 30 seconds to allow the player to join a server");
	sleep(30);
	info("Done sleeping, starting scan");

	list_t *maps = get_maps();
	list_t *curr = maps;
	list_t *found = NULL;
	while (curr) {
		map_t *map = (map_t *)curr->data;
		if (strstr(map->name, "Engine")) {
			info("Scanning map for pattern 100");
			uint32_t pattern = 100;
			found = scan_map(map, (uint8_t *)&pattern, sizeof(pattern));
			if (!found) {
				info("None found");
			}
			while (found) {
				info("Found at %p in map %s", found->data, map->name);
				found = found->next;
			}
		}
		curr = curr->next;
	}

	info("Scan complete, found %d addresses", list_length(found));

	for (int i = 5; i > 0; i--) {
		info("Sleeping for 3 seconds");
		sleep(3);
		info("Filtering scan results, %d more iteration(s)", i);

		curr = found;
		list_t *prev = NULL;
		while (curr) {
			bool is_still_valid = *((uint32_t *)curr->data) == 100;
			if (is_still_valid) {
				info("Address %p is still valid", curr->data);
				prev = curr;
				curr = curr->next;
			} else {
				info("Address %p is no longer valid, removing", curr->data);
				list_t *to_free = curr;
				if (prev) {
					prev->next = curr->next;
				} else {
					found = curr->next; // Update head if we are removing the first element
				}
				curr = curr->next;
				free(to_free);
			}
		}
		if (!found) {
			info("Everything filtered, exiting");
			break;
		}

		info("Filtering done, found %d addresses", list_length(found));
	}

	if (found) {
		while (1) {
			info("Sleeping for 2 seconds");
			sleep(2);

			info("Dumping found addresses");
			curr = found;
			while (curr) {
				void *addr = curr->data;
				info("Address: %p -> %d", addr, *(uint32_t *)addr);
				curr = curr->next;
			}
		}
	}

	list_clear(&maps);
}

__attribute__((constructor))
void init(void) {
	// Create thread
	pthread_t thread_id;
	if (pthread_create(&thread_id, NULL, (void *(*)(void *))thread, NULL) != 0) {
		fprintf(stderr, "Failed to create thread\n");
		exit(EXIT_FAILURE);
	}
	pthread_detach(thread_id); // Detach the thread so it cleans up after itself
}
