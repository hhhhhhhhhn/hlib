#include <stdio.h>
#include "hlib/hsort.h"

int cmp_int(void* a, void* b) {
	return *(int*)a - *(int*)b;
}

#define NUMS 64
int main() {
	i32 numbers[NUMS];

	i32 seed = 123823;
	for (size_t i = 0; i < NUMS; i++) {
		seed = (u32)seed*(u32)38212642837 - i;
		numbers[i] = seed % (NUMS*2);
	}

	hsort(numbers, NUMS, sizeof(int), cmp_int);

	for (size_t i = 0; i < NUMS; i++) {
		printf("%i ", numbers[i]);
	}
	printf("\n");
}
