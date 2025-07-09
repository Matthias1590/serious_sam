#pragma once

#include <stddef.h>

typedef struct list_t {
	void *data;
	void (*free_func)(void *data);
	struct list_t *next;
} list_t;

void list_clear(list_t **head);
void list_push_back(list_t **head, void *data);
void list_push_back_ex(list_t **head, void *data, void (*free_func)(void *data));
void list_push_front(list_t **head, void *data);
void list_push_front_ex(list_t **head, void *data, void (*free_func)(void *data));
void *list_get(list_t *head, int index);
size_t list_length(list_t *head);
