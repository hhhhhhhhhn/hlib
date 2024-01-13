#include <stdio.h>
#include <stdlib.h>

#ifndef HLIB_DEBUG
	#define HLIB_DEBUG
#endif

#include "hlib/core.h"

int main() {
	i32* numbers = malloc(1024*sizeof(i32));
	for (i32 i = 0; i < 1024; i++) {
		numbers[i] = i;
	}
	numbers = realloc(numbers, 1024*sizeof(i32));
	for (i32 i = 0; i < 1024; i++) {
		if (i % 100 == 0) {
			printf("%d\n", numbers[i]);
		}
	}
	free(numbers);
}
