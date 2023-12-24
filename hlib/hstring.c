#include "core.h"
#include <stdbool.h>
#include <assert.h>
#include <stdio.h>
#include <string.h>
#include "hstring.h"

HStringView hstring_view_new(char* data, usize len) {
	return (HStringView) {
		.data = data,
		.len = len
	};
}

HStringView hstring_view_from_builder(HStringBuilder* builder) {
	return (HStringView) {
		.data = builder->data,
		.len = builder->len
	};
}

HStringView hstring_view_slice(HStringView view, usize start, usize end) {
	return (HStringView) {
		.data = view.data + start,
		.len = end - start
	};
}

void hstring_view_consume_chars(HStringView* view, usize count) {
	view->data += count;
	view->len -= count;
}

HStringView hstring_view_split_char(HStringView* view, char delim) {
	for(usize i = 0; i < view->len; i++) {
		if (view->data[i] == delim) {
			HStringView result = hstring_view_slice(*view, 0, i);
			hstring_view_consume_chars(view, i+1);
			return result;
		}
	}

	HStringView result = hstring_view_slice(*view, 0, view->len);
	hstring_view_consume_chars(view, view->len);
	return result;
}

HStringView hstring_view_split_while_predicate(HStringView* view, bool(*pred)(char)) {
	usize i = 0;
	while (!pred(view->data[i]) && i < view->len) {
		i++;
	}
	HStringView result = hstring_view_slice(*view, 0, i);

	while (pred(view->data[i]) && i < view->len) {
		i++;
	}
	hstring_view_consume_chars(view, i);

	return result;
}

HStringView hstring_view_trim_left(HStringView view) {
	for(usize i = 0; i < view.len; i++) {
		if (!hstring_is_whitespace(view.data[i])) {
			return hstring_view_slice(view, i, view.len);
		}
	}
	return hstring_view_slice(view, view.len, view.len);
}

HStringView hstring_view_trim_right(HStringView view) {
	for(isize i = view.len-1; i >= 0; i--) {
		if (!hstring_is_whitespace(view.data[i])) {
			return hstring_view_slice(view, 0, i+1);
		}
	}
	return hstring_view_slice(view, 0, 0);
}

HStringView hstring_view_trim(HStringView view) {
	return hstring_view_trim_right(hstring_view_trim_left(view));
}

bool hstring_view_eq(HStringView a, HStringView b) {
	if (a.len != b.len) {
		return false;
	}
	return memcmp(a.data, b.data, a.len) == 0;
}

bool hstring_view_write_to_file(HStringView* view, FILE* file) {
	int written = fprintf(file, "%.*s", (int)view->len, view->data);;
	return (usize)written == view->len;
}

bool hstring_view_write_to_filepath(HStringView* view, char* path) {
	FILE* file = fopen(path, "w");
	if (file == NULL) {
		return false;
	}
	bool status = hstring_view_write_to_file(view, file);
	bool close_status = fclose(file);
	assert(close_status);
	return status;
}

bool hstring_is_whitespace(char c) {
	return c == ' ' || c == '\t' || c == '\n' || c == '\r';
}

/////////////////////// BUILDER /////////////////////////////

HStringBuilder hstring_builder_new() {
	char* data = malloc(32);
	nullpanic(data);

	return (HStringBuilder) {
		.data = data,
		.len = 0,
		.cap = 32
	};
}

// internal
void hstring_builder_resize(HStringBuilder* builder, usize new_cap) {
	builder->cap = new_cap;
	char* new_data = realloc(builder->data, new_cap);
	nullpanic(new_data);
	builder->data = new_data;
}

void hstring_builder_append_view(HStringBuilder* builder, HStringView view) {
	while (builder->len + view.len >= builder->cap) {
		hstring_builder_resize(builder, builder->cap*2); // TODO: Calculate how many times to resize beforehand
	}
	memcpy(builder->data + builder->len, view.data, view.len);
}

bool hstring_builder_append_file(HStringBuilder* builder, FILE* file) {
	while (true) {
		if (builder->len >= builder->cap) {
			hstring_builder_resize(builder, builder->cap*2);
		}
		usize bytes_read = fread(builder->data + builder->len, 1, builder->cap - builder->len, file);
		if (ferror(file)) {
			return false;
		}
		builder->len += bytes_read;

		if (bytes_read == 0) {
			break;
		}
	}
	return true;
}

bool hstring_builder_append_filepath(HStringBuilder* builder, char* path) {
	FILE* file = fopen(path, "r");
	if (file == NULL) {
		return false;
	}
	if (!hstring_builder_append_file(builder, file)) {
		fclose(file);
		return false;
	}
	if (fclose(file) != 0) {
		return false;
	}
	return true;
}

HStringBuilder hstring_builder_from_view(HStringView view) {
	HStringBuilder builder = hstring_builder_new();
	hstring_builder_append_view(&builder, view);
	return builder;
}

HStringBuilderResult hstring_builder_from_file(FILE* file) {
	HStringBuilderResult result = {0};
	result.builder = hstring_builder_new();
	if(!hstring_builder_append_file(&result.builder, file)) {
		hstring_builder_free(&result.builder);
		return result;
	}
	result.ok = 1;
	return result;
}

HStringBuilderResult hstring_builder_from_filepath(char* path) {
	HStringBuilderResult result = {0};
	result.builder = hstring_builder_new();
	if(!hstring_builder_append_filepath(&result.builder, path)) {
		hstring_builder_free(&result.builder);
		return result;
	}
	result.ok = 1;
	return result;
}

void hstring_builder_push_char(HStringBuilder* builder, char c) {
	if (builder->len >= builder->cap) {
		hstring_builder_resize(builder, builder->cap*2);
	}
	builder->data[builder->len] = c;
	builder->len++;
}

void hstring_builder_free(HStringBuilder* builder) {
	free(builder->data);
}
