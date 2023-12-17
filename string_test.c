#include <stdio.h>
#include "hlib/hstring.h"

int main() {
	HStringBuilder builder = hstring_builder_new();

	hstring_builder_append_file(&builder, stdin);

	HStringView view = hstring_view_from_builder(&builder);

	while(view.len) {
		HStringView line = hstring_view_split_char(&view, '\n');
		printf("Line: %.*s\n", (int)line.len, line.data);
	}

	hstring_builder_free(&builder);
}
