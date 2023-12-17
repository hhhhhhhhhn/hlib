#ifndef HLIB_CORE_H
#define HLIB_CORE_H

#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>

void* malloc_print(size_t size, const char* file, int line);
void free_print(void* ptr, const char* file, int line);
void* realloc_print(void* ptr, size_t size, const char* file, int line);

#ifdef HLIB_DEBUG
#define malloc(size) malloc_print(size, __FILE__, __LINE__)
#define free(ptr) free_print(ptr, __FILE__, __LINE__)
#define realloc(ptr, size) realloc_print(ptr, size, __FILE__, __LINE__)
#endif

#define nullpanic(ptr) if (!(ptr)) {fprintf(stderr, "Unexpected null at %s:%d\n", __FILE__, __LINE__); exit(1);}
#define todo() do {fprintf(stderr, "Not implemented at %s:%d\n", __FILE__, __LINE__); exit(1);} while(0);
#define unreachable() do {fprintf(stderr, "Unreachable at %s:%d\n", __FILE__, __LINE__); exit(1);} while(0);

#define ARCH_BYTES (sizeof(size_t))
#define ARCH_BITS (sizeof(size_t)*8)

#endif
