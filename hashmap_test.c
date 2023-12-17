#include <assert.h>
#include <stdio.h>
#include "hlib/hhashmap.h"

typedef char key[64];
typedef char value[256];


int main() {
	HHashMap map = hhashmap_new(sizeof(key), sizeof(value), HKEYTYPE_DIRECT);

	key k1 = {0};
	strcpy(k1, "key1");
	value v1 = {0};
	strcpy(v1, "value1");

	key k2 = {0};
	strcpy(k2, "key2");
	value v2 = {0};
	strcpy(v2, "value2");

	hhashmap_set(&map, k1, v1);
	hhashmap_set(&map, k2, v2);

	assert(strcmp(hhashmap_get(&map, k1), v1) == 0);
	assert(strcmp(hhashmap_get(&map, k2), v2) == 0);

	hhashmap_delete(&map, k2);

	assert(hhashmap_get(&map, k2) == NULL);

	hhashmap_free(&map);


	map = hhashmap_new(sizeof(int), sizeof(int), HKEYTYPE_DIRECT);
	for (int key = -512; key < 512; key++) {
		int value = -key;
		hhashmap_set(&map, &key, &value);
	}

	int* key;
	int* value;
	size_t index;
	while (hhashmap_next(&map, &key, &value, &index)) {
		assert(*key == -(*value));
	};

	hhashmap_free(&map);
}
