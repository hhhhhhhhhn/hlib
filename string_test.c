#include <stdio.h>
#include "hlib/hstring.h"
#include "hlib/core.h"
#include <assert.h>

int main() {
	strb builder = strb_new();

	strb_append_file(&builder, stdin);

	str view = str_from_strb(&builder);

	while(view.len) {
		str line = str_split_char(&view, '\n');
		printf("Line: %.*s\n", (int)line.len, line.data);
	}

	strb_free(&builder);

	strbResult res = strb_from_filepath(STR("string_test.c"));
	assert(res.ok);

	builder = res.builder;

	view = str_from_strb(&builder);
	usize count = 0;
	while(view.len) {
		str line = str_split_char(&view, '\n');
		line = str_trim(line);
		printf("Trimmed: %.*s\n", (int)line.len, line.data);
		count++;
	}
	for(usize i = 0; i < 1000; i++) {
		str_split_char(&view, '\n');
	}
	printf("This test file has %li lines\n", count);

	view = str_from_strb(&builder);
	while(view.len) {
		str word = str_split_while_predicate(&view, hstring_is_whitespace);
		printf("Word: '%.*s'\n", (int)word.len, word.data);
	}
	for(usize i = 0; i < 1000; i++) {
		str_split_while_predicate(&view, hstring_is_whitespace);
	}

	strb_free(&builder);
}
