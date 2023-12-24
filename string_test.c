#include <stdio.h>
#include "hlib/hstring.h"
#include "hlib/core.h"
#include <assert.h>

int main() {
	HStringBuilder builder = hstring_builder_new();

	hstring_builder_append_file(&builder, stdin);

	HStringView view = hstring_view_from_builder(&builder);

	while(view.len) {
		HStringView line = hstring_view_split_char(&view, '\n');
		printf("Line: %.*s\n", (int)line.len, line.data);
	}

	hstring_builder_free(&builder);

	HStringBuilderResult res = hstring_builder_from_filepath("string_test.c");
	assert(res.ok);

	builder = res.builder;

	view = hstring_view_from_builder(&builder);
	usize count = 0;
	while(view.len) {
		HStringView line = hstring_view_split_char(&view, '\n');
		line = hstring_view_trim(line);
		printf("Trimmed: %.*s\n", (int)line.len, line.data);
		count++;
	}
	for(usize i = 0; i < 1000; i++) {
		hstring_view_split_char(&view, '\n');
	}
	printf("This test file has %li lines\n", count);

	view = hstring_view_from_builder(&builder);
	while(view.len) {
		HStringView word = hstring_view_split_while_predicate(&view, hstring_is_whitespace);
		printf("Word: '%.*s'\n", (int)word.len, word.data);
	}
	for(usize i = 0; i < 1000; i++) {
		hstring_view_split_while_predicate(&view, hstring_is_whitespace);
	}

	hstring_builder_free(&builder);
}
