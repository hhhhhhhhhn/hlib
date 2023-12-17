#include <stdlib.h>
#include <stddef.h>
#include <assert.h>
#include <string.h>
#include "core.h"

typedef struct HVec {
	size_t element_size;
	size_t len;
	size_t cap;
	void* data;
} HVec;

HVec hvec_new_with_cap(size_t element_size, size_t cap) {
	void* data = malloc(cap * element_size);
	nullpanic(data);
	HVec vec = {
		.element_size = element_size,
		.len = 0,
		.cap = cap,
		.data = data,
	};

	return vec;
}

HVec hvec_new(size_t element_size) {
	return hvec_new_with_cap(element_size, 16);
}

// internal
void hvec_resize(HVec* vec, size_t new_cap) {
	assert(new_cap >= vec->len);

	void* new_data = realloc(vec->data, new_cap*vec->element_size);
	nullpanic(new_data);
	vec->data = new_data;
	vec->cap = new_cap;
}

void hvec_push(HVec* vec, void* element) {
	if (vec->len >= vec->cap) {
		hvec_resize(vec, vec->cap * 2);
	}
	void* destination = (char*)vec->data + vec->len*vec->element_size;
	memcpy(destination, element, vec->element_size);
	vec->len++;
}

void hvec_free(HVec* vec) {
	free(vec->data);
}

void* hvec_at(HVec* vec, size_t index) {
	if (index >= vec->len) {
		return NULL;
	}
	return (char*)vec->data + index*vec->element_size;
}
