#include <stdio.h>


#ifndef HLIB_DEBUG
	#define HLIB_DEBUG
#endif
#include "hlib/core.h"
#include "hlib/hvec.h"

int main() {
	HVec vec = hvec_new(sizeof(double));

	for (int i = 0; i < 1024; i++) {
		double item = i;
		hvec_push(&vec, &item);
	}

	for (int i = 0; i < 1024; i++) {
		double* item = hvec_at(&vec, i);
		nullpanic(item);
		if (i % 100 == 0) {
			printf("%f\n", *item);
		}

		assert(*item == (double)i);
	}

	hvec_free(&vec);
}
