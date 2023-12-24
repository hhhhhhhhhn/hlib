#ifndef HLIB_HSTRING_H
#define HLIB_HSTRING_H

#include <stdbool.h>
#include "core.h"

// Represents immutable view into a string, does not allocate
typedef struct HStringView {
	char* data;
	usize len;
} HStringView;

typedef struct HStringBuilder {
	char* data;
	usize len;
	usize cap;
} HStringBuilder;

typedef struct {
	HStringBuilder builder;
	bool ok;
} HStringBuilderResult;

#define HSTRV(str) ((HStringView) { .data = (str), .len = sizeof(str) - 1 })

HStringView hstring_view_new(char* data, usize len);
HStringView hstring_view_from_builder(HStringBuilder* builder);
HStringView hstring_view_slice(HStringView view, usize start, usize end);
void hstring_view_consume_chars(HStringView* view, usize count);
HStringView hstring_view_trim_left(HStringView view);
HStringView hstring_view_trim_right(HStringView view);
HStringView hstring_view_trim(HStringView view);
HStringView hstring_view_split_char(HStringView* view, char delim);
HStringView hstring_view_split_while_predicate(HStringView* view, bool(*pred)(char));
bool hstring_view_eq(HStringView a, HStringView b);
bool hstring_view_write_to_file(HStringView* view, FILE* file);
bool hstring_view_write_to_filepath(HStringView* view, char* path);

bool hstring_is_whitespace(char c);

HStringBuilder hstring_builder_new();
HStringBuilder hstring_builder_from_view(HStringView view); // Clones the data
HStringBuilderResult hstring_builder_from_filepath(char* path);
HStringBuilderResult hstring_builder_from_file(FILE* file);
void hstring_builder_append_view(HStringBuilder* builder, HStringView view); // Clones the data
bool hstring_builder_append_file(HStringBuilder* builder, FILE* file); // Returns true on success
bool hstring_builder_append_filepath(HStringBuilder* builder, char* path);
void hstring_builder_push_char(HStringBuilder* builder, char c);
void hstring_builder_free(HStringBuilder* builder);

#endif
