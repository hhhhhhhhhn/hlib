#ifndef HLIB_HVEC_H
#define HLIB_HVEC_H

#include <stddef.h>

typedef struct HVec {
	size_t element_size;
	size_t len;
	size_t cap;
	void* data;
} HVec;

HVec hvec_new_with_cap(size_t element_size, size_t cap);
HVec hvec_new(size_t element_size);
void hvec_push(HVec* vec, void* element);
void hvec_free(HVec* vec);
void* hvec_at(HVec* vec, size_t index);

#endif
