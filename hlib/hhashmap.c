#include "core.h"
#include <stdbool.h>
#include <string.h>

typedef struct HKeyType {
	size_t (*hash)(void* key, size_t size);
	bool (*eq)(void* key1, void* key2, size_t size);
} HKeyType;

// internal
typedef struct EntryInfo {
	bool occupied;
	bool deleted;
} EntryInfo;

// TODO: Store key-value pairs for memory coherence, (alignment is needed)
typedef struct HHashMap {
	size_t len;
	size_t cap;
	size_t key_size;
	size_t value_size;
	void* keys;
	void* values;
	EntryInfo* info;
	HKeyType type;
} HHashMap;

HHashMap hhashmap_new_with_cap(size_t key_size, size_t value_size, HKeyType type, size_t cap) {
	void* keys = malloc(cap * key_size);
	nullpanic(keys);
	void* values = malloc(cap * value_size);
	nullpanic(values);
	EntryInfo* info = malloc(cap * sizeof(EntryInfo));
	nullpanic(info);
	memset(info, 0, cap*sizeof(EntryInfo));

	return (HHashMap) {
		.len = 0,
		.cap = cap,
		.key_size = key_size,
		.value_size = value_size,
		.keys = keys,
		.values = values,
		.type = type,
		.info = info,
	};
}

HHashMap hhashmap_new(size_t key_size, size_t value_size, HKeyType type) {
	return hhashmap_new_with_cap(key_size, value_size, type, 128);
}

// Index must start as 0
bool hhashmap_next(HHashMap* map, void** ret_key, void** ret_value, size_t* index) {
	while(*index < map->cap && (!map->info[*index].occupied || map->info[*index].deleted)) {
		(*index)++;
	}
	if (*index >= map->cap) {
		return false;
	}
	*ret_key = (char*)map->keys + *index*map->key_size;
	*ret_value = (char*)map->values + *index*map->value_size;

	(*index)++;
	return true;
}

void hhashmap_set(HHashMap* map, void* key, void* value);
void hhashmap_free(HHashMap* map);
// internal
void hhashmap_grow(HHashMap* map) {
	HHashMap new_map = hhashmap_new_with_cap(map->key_size, map->value_size, map->type, map->cap*2);
	void* key;
	void* value;
	size_t index = 0;

	while(hhashmap_next(map, &key, &value, &index)) {
		hhashmap_set(&new_map, key, value);
	}

	hhashmap_free(map);

	*map = new_map;
}

void hhashmap_set(HHashMap* map, void* key, void* value) {
	if (4*map->len > 3*map->cap) { // A 0.75 load factor
		hhashmap_grow(map);
	}
	size_t index = map->type.hash(key, map->key_size) % map->cap;

	for(size_t safety = 0; safety < map->len+1; safety++) {
		if (!map->info[index].occupied || map->info[index].deleted) {
			map->info[index].occupied = true;
			map->info[index].deleted = false;
			memcpy((char*)map->keys + index*map->key_size, key, map->key_size);
			memcpy((char*)map->values + index*map->value_size, value, map->value_size);
			map->len++;
			return;
		}
		index++;
	}
	unreachable();
}

// Returns -1 if not found
// internal
int hhashmap_get_index(HHashMap* map, void* key) {
	size_t hash = map->type.hash(key, map->key_size);
	int index = hash % map->cap;

	for(size_t safety = 0; safety < map->cap; safety++) {
		if (!map->info[index].occupied) {
			break;
		}
		if (map->info[index].deleted) {
			index++;
			continue;
		}
		if (map->type.eq(key, (char*)map->keys + index*map->key_size, map->key_size)) {
			return index;
		}
		index++;
	}
	return -1;
}

// Returns NULL if not found
void* hhashmap_get(HHashMap* map, void* key) {
	int index = hhashmap_get_index(map, key);
	if (index == -1) {
		return NULL;
	}
	return (char*)map->values + index*map->value_size;
}

// Cannot fail
void hhashmap_delete(HHashMap* map, void* key) {
	int index = hhashmap_get_index(map, key);

	if (index == -1) {
		return;
	}

	map->info[index].deleted = true;
	map->len--;
}

void hhashmap_free(HHashMap* map) {
	free(map->keys);
	free(map->values);
	free(map->info);
}

bool hkeytype_direct_eq(void* key1, void* key2, size_t size) {
	return memcmp(key1, key2, size) == 0;
}

// sdbm
size_t hkeytype_direct_hash(void* key, size_t size) {
	size_t hash = 0;
	for (size_t i = 0; i < size; i++) {
		hash = ((char*)key)[i] + (hash << 6) + (hash << 16) - hash;
	}
	return hash;
}

const HKeyType HKEYTYPE_DIRECT = {
	.eq = hkeytype_direct_eq,
	.hash = hkeytype_direct_hash,
};
