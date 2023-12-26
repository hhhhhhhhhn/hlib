#include "core.h"
#include "hfs.h"

FILE* hfs_open_file(str path, bool read, bool writing) {
	char* path_cstr = str_to_cstr(path); // TODO: Remove allocation
	FILE* result;
	if (read && writing) {
		result = fopen(path_cstr, "rb+");
	} else if (read) {
		result = fopen(path_cstr, "rb");
	} else if (writing) {
		result = fopen(path_cstr, "wb");
	} else {
	}

	free(path_cstr);
	return result;
}

void hfs_close_file(FILE* file) {
	fclose(file);
}
