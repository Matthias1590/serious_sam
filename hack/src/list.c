#include "list.h"
#include <assert.h>
#include <stdlib.h>

void list_clear(list_t **head) {
	list_t *current = *head;
	list_t *next_node;

	while (current != NULL) {
		next_node = current->next;
		free(current->data);
		free(current);
		current = next_node;
	}

	*head = NULL;
}

void list_push_back(list_t **head, void *data) {
	list_push_back_ex(head, data, free);
}

void list_push_back_ex(list_t **head, void *data, void (*free_func)(void *data)) {
	list_t *new_node = malloc(sizeof(list_t));
	assert(new_node != NULL);

	new_node->data = data;
	new_node->free_func = free_func;
	new_node->next = NULL;

	if (*head == NULL) {
		*head = new_node;
	} else {
		list_t *current = *head;
		while (current->next != NULL) {
			current = current->next;
		}
		current->next = new_node;
	}
}

void list_push_front(list_t **head, void *data) {
	list_push_front_ex(head, data, free);
}

void list_push_front_ex(list_t **head, void *data, void (*free_func)(void *data)) {
	list_t *new_node = malloc(sizeof(list_t));
	assert(new_node != NULL);

	new_node->data = data;
	new_node->free_func = free_func;
	new_node->next = *head;
	*head = new_node;
}

void *list_get(list_t *head, int index) {
	assert(index >= 0);
	list_t *current = head;
	int i = 0;

	while (current != NULL) {
		if (i == index) {
			return current->data;
		}
		current = current->next;
		i++;
	}

	assert(0 && "Index out of bounds");
}

size_t list_length(list_t *head) {
	size_t length = 0;
	list_t *current = head;

	while (current != NULL) {
		length++;
		current = current->next;
	}

	return length;
}
