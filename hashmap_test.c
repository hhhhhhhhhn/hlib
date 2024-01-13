#include <stdio.h>
#include <string.h>
#include "hlib/hhashmap.h"
#include "hlib/hstring.h"
#include "hlib/core.h"
#include "hlib/hparse.h"

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

	map = hhashmap_new(sizeof(int), sizeof(int), HKEYTYPE_DIRECT);
	for (int key = -512; key < 512; key += 2) {
		int value = -key;
		hhashmap_set(&map, &key, &value);
	}
	for (int key = -511; key < 511; key += 2) {
		int value = key*2;
		hhashmap_set(&map, &key, &value);
	}

	for (int key = -512; key < 512; key += 2) {
		hhashmap_delete(&map, &key);
	}

	index = 0;
	while(hhashmap_next(&map, &key, &value, &index)) {
		assert((*key % 2 == 1) || (*key % 2 == -1));
		assert(*value == *key*2);
	}

	hhashmap_free(&map);

	map = hhashmap_new(sizeof(str), sizeof(i64), HKEYTYPE_DIRECT);
	str str_key = STR("1");
	i64 int_value = 1;
	hhashmap_set(&map, &str_key, &int_value);

	str_key = STR("2");
	int_value = 2;
	hhashmap_set(&map, &str_key, &int_value);

	str_key = STR("3");
	int_value = 3;
	hhashmap_set(&map, &str_key, &int_value);

	str* iter_key;
	i64* iter_value;
	index = 0;
	while(hhashmap_next(&map, &iter_key, &iter_value, &index)) {
		i64 actual;
		assert(hparse_i64(*iter_key, &actual));

		assert(actual == *iter_value);
	}
}
