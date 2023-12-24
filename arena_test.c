#include "hlib/core.h"
#include "hlib/harena.h"

#ifndef HLIB_DEBUG
	#define HLIB_DEBUG
#endif

int main() {
	HArena arena = harena_new();

	for(usize i = 0; i < 16; i++) {
		i32* data = harena_alloc(&arena, sizeof(i32) * (1 << i));

		for(usize j = 0; j < (usize)(1 << i); j++) {
			data[j] = j*i;
		}
	}

	harena_free(&arena);
}
