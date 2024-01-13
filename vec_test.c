#include <stdio.h>


#ifndef HLIB_DEBUG
	#define HLIB_DEBUG
#endif
#include "hlib/core.h"
#include "hlib/hvec.h"

i32 main() {
	HVec vec = hvec_new(sizeof(f64));

	for (i32 i = 0; i < 1024; i++) {
		f64 item = i;
		hvec_push(&vec, &item);
	}

	for (i32 i = 0; i < 1024; i++) {
		f64* item = hvec_at(&vec, i);
		nullpanic(item);
		if (i % 100 == 0) {
			printf("%f\n", *item);
		}

		assert(*item == (f64)i);
	}

	hvec_free(&vec);
}
