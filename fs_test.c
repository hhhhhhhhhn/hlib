#include "hlib/core.h"
#include "hlib/hfs.h"
#include "hlib/hstring.h"

i32 main() {
	FILE* file = hfs_open_file(STR("fs_test.c"), true, false);
	strbResult result = strb_from_file(file);
	hfs_close_file(file);

	assert(result.ok);
	strb builder = result.builder;

	str view = str_from_strb(&builder);

	while(view.len) {
		str line = str_split_char(&view, '\n');
		printf("Line: %.*s\n", (int)line.len, line.data);
	}

	strb_free(&builder);

	HFSDirResult dir_result = hfs_open_dir(STR("."));
	assert(dir_result.ok);

	HFSDir dir = dir_result.dir;
	HFSDirEntry entry;
	while(hfs_dir_next(&dir, &entry)) {
		if (entry.is_dir) {
			printf("Dir: %.*s\n", (int)entry.path.len, entry.path.data);
		}
		else {
			printf("File: %.*s\n", (int)entry.path.len, entry.path.data);
		}
	}

	hfs_close_dir(dir);
}
