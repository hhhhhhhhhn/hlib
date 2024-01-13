#include <stdio.h>
#include "hlib/hsort.h"

i32 cmp_int(void* a, void* b) {
	return *(i32*)a - *(i32*)b;
}

#define NUMS 64
i32 main() {
	i32 numbers[NUMS];

	i32 seed = 123823;
	for (usize i = 0; i < NUMS; i++) {
		seed = (u32)seed*(u32)38212642837 - i;
		numbers[i] = seed % (NUMS*2);
	}

	hsort(numbers, NUMS, sizeof(int), cmp_int);

	for (usize i = 0; i < NUMS; i++) {
		printf("%i ", numbers[i]);
	}
	printf("\n");
}
