#ifndef HLIB_HSTRING_H
#define HLIB_HSTRING_H

#include <stddef.h>
#include <stdio.h>
#include <stdbool.h>

typedef struct HStringView {
	char* data;
	size_t len;
} HStringView;

typedef struct HStringBuilder {
	char* data;
	size_t len;
	size_t cap;
} HStringBuilder;

HStringView hstring_view_new(char* data, size_t len);
HStringView hstring_view_from_builder(HStringBuilder* builder);
HStringView hstring_view_slice(HStringView view, size_t start, size_t end);
void hstring_view_consume_chars(HStringView* view, size_t count);
HStringView hstring_view_split_char(HStringView* view, char delim);
HStringView hstring_view_split_while_predicate(HStringView* view, bool(*pred)(char));

bool hstring_is_whitespace(char c);

HStringBuilder hstring_builder_new();
void hstring_builder_append_file(HStringBuilder* builder, FILE* file);
void hstring_builder_free(HStringBuilder* builder);

#endif
