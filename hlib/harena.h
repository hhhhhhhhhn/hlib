#ifndef HLIB_HARENA_H
#define HLIB_HARENA_H

#include "core.h"

typedef struct HStaticArena {
	void* data;
	size_t cap;
	size_t used;
} HStaticArena;

HStaticArena hstaticarena_new(size_t cap);
void* hstaticarena_alloc(HStaticArena* arena, size_t size);
void hstaticarena_free(HStaticArena* arena);

#define ARENA_AMOUNT 30

typedef struct HArena {
	HStaticArena sarenas[ARENA_AMOUNT];
	size_t sarenas_used;
	size_t cap;
} HArena;

HArena harena_new_with_cap(size_t cap);
HArena harena_new();
void* harena_alloc(HArena* arena, size_t size);
void harena_free(HArena* arena);

#endif
