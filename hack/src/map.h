#pragma once

typedef struct {
	char path[256];
	char name[256];
	void *start;
	void *end;
} map_t;
