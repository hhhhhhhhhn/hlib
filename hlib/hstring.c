#include "core.h"
#include <stdbool.h>
#include <assert.h>
#include <stdio.h>
#include "hstring.h"

HStringView hstring_view_new(char* data, size_t len) {
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

HStringView hstring_view_slice(HStringView view, size_t start, size_t end) {
	return (HStringView) {
		.data = view.data + start,
		.len = end - start
	};
}

void hstring_view_consume_chars(HStringView* view, size_t count) {
	view->data += count;
	view->len -= count;
}

HStringView hstring_view_split_char(HStringView* view, char delim) {
	for(size_t i = 0; i < view->len; i++) {
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
	size_t i = 0;
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

bool hstring_view_write_to_file(HStringView* view, FILE* file) {
	int written = fprintf(file, "%.*s", (int)view->len, view->data);;
	return (size_t)written == view->len;
}

bool hstring_view_write_to_path(HStringView* view, char* path) {
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
void hstring_builder_resize(HStringBuilder* builder, size_t new_cap) {
	builder->cap = new_cap;
	char* new_data = realloc(builder->data, new_cap);
	nullpanic(new_data);
	builder->data = new_data;
}

void hstring_builder_append_file(HStringBuilder* builder, FILE* file) {
	while (true) {
		if (builder->len >= builder->cap) {
			hstring_builder_resize(builder, builder->cap*2);
		}
		size_t bytes_read = fread(builder->data + builder->len, 1, builder->cap - builder->len, file);
		builder->len += bytes_read;

		if (bytes_read == 0) {
			break;
		}
	}
}

void hstring_builder_append_char(HStringBuilder* builder, char c) {
	if (builder->len >= builder->cap) {
		hstring_builder_resize(builder, builder->cap*2);
	}
	builder->data[builder->len] = c;
	builder->len++;
}

void hstring_builder_free(HStringBuilder* builder) {
	free(builder->data);
}
