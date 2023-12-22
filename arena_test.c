#include "hlib/harena.h"

#ifndef HLIB_DEBUG
	#define HLIB_DEBUG
#endif

int main() {
	HArena arena = harena_new();

	for(size_t i = 0; i < 16; i++) {
		int* data = harena_alloc(&arena, sizeof(int) * (1 << i));

		for(size_t j = 0; j < (size_t)(1 << i); j++) {
			data[j] = j*i;
		}
	}

	harena_free(&arena);
}
