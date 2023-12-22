#include "core.h"
#include <stdbool.h>
#include <stdint.h>

void memswap(void* a, void* b, size_t len) {
	char* aa = (char*)a;
	char* bb = (char*)b;
	for(size_t i = 0; i < len; i++) {
		char tmp = *(aa + i);
		*(aa + i) = *(bb + i);
		*(bb + i) = tmp;
	}
}

size_t median_of_3_pivot(void* data, size_t len, size_t element_size, int (*cmp)(void* a, void* b)) {
	size_t a_index = 0;
	size_t b_index = len/2;
	size_t c_index = len-1;

	void* a = (char*)data + a_index*element_size;
	void* b = (char*)data + b_index*element_size;
	void* c = (char*)data + c_index*element_size;

	bool a_lt_b = cmp(a, b) < 0;
	bool b_lt_c = cmp(b, c) < 0;
	bool a_lt_c = cmp(a, c) < 0;

	if (a_lt_b && b_lt_c) {
		return b_index;
	}
	if (a_lt_c && !b_lt_c) {
		return c_index;
	}
	if (!a_lt_b && a_lt_c) {
		return a_index;
	}
	return b_index;
}

// quicksort
void hsort(void* data, size_t len, size_t element_size, int (*cmp)(void* a, void* b)) {
	if (len < 2) {
		return;
	}
	char* dataa = (char*)data;
	intmax_t pivot = median_of_3_pivot(data, len, element_size, cmp);  // TODO: improve
	intmax_t low = 0;
	intmax_t high = len-2;

	memswap(dataa + pivot*element_size, dataa + (len-1)*element_size, element_size);
	pivot = len-1;

	while(true) {
		while (cmp(dataa + low*element_size, dataa + pivot*element_size) < 0 && low < (intmax_t)len) {
			low++;
		}
		while (cmp(dataa + high*element_size, dataa + pivot*element_size) > 0 && high >= 0) {
			high--;
		}
		if(low > high) {
			memswap(dataa + low*element_size, dataa + pivot*element_size, element_size);
			pivot = low;
			break;
		}
		memswap(dataa + low*element_size, dataa + high*element_size, element_size);
		low++;
		high--;
	}

	hsort(data, pivot, element_size, cmp);
	hsort((char*)data + (pivot+1)*element_size, len - pivot - 1, element_size, cmp);
}
