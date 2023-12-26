#include "hlib/hflag.h"
#include <stdio.h>

int main(int argc, char** argv) {
	i64* int_value = hflag_int('i', "int", "integer value", -12);
	bool* bool_value = hflag_bool('b', "bool", "boolean value");
	f64* float_value = hflag_float('f', "float", "float value", -20);
	str* string_value = hflag_str('s', "string", "string value", STR("hello there"));
	hflag_str(0, "unused", "unused value", STR("useless"));

	hflag_set_help_intro("This is a test program for the hlib flags\n\n"
			"Usage: flag_test [flags] [additional args]\n");
	hflag_parse(&argc, &argv);

	printf("int value: %ld, bool value: %d, float value: %f, string value: %.*s\n", *int_value, *bool_value, *float_value, (int)string_value->len, string_value->data);

	printf("\nRemaining args:\n");
	for(int i = 0; i < argc; i++) {
		printf("%s\n", argv[i]);
	}
	return 0;
}
