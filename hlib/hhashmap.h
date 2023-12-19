#ifndef HLIB_HHASHMAP_H
#define HLIB_HHASHMAP_H

#include "core.h"
#include <stdint.h>
#include <stdbool.h>
#include <string.h>

typedef struct HKeyType {
	size_t (*hash)(void* key, size_t size);
	bool (*eq)(void* key1, void* key2, size_t size);
} HKeyType;

// internal. TODO: Hide
typedef struct EntryInfo {
	bool occupied;
	bool deleted;
} EntryInfo;

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

HHashMap hhashmap_new_with_cap(size_t key_size, size_t value_size, HKeyType type, size_t cap);
HHashMap hhashmap_new(size_t key_size, size_t value_size, HKeyType type);
bool hhashmap_next(HHashMap* map, void* ret_key, void* ret_value, size_t* index);
void hhashmap_set(HHashMap* map, void* key, void* value);
void* hhashmap_get(HHashMap* map, void* key);
void hhashmap_delete(HHashMap* map, void* key);
void hhashmap_free(HHashMap* map);

bool hkeytype_direct_eq(void* key1, void* key2, size_t size);
size_t hkeytype_direct_hash(void* key, size_t size);
extern const HKeyType HKEYTYPE_DIRECT;

#endif
