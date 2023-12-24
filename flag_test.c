#include "hlib/hflag.h"
#include <stdio.h>

int main(int argc, char** argv) {
	i64* int_value = hflag_int('i', "int", "integer value", -12);
	bool* bool_value = hflag_bool('b', "bool", "boolean value");
	f64* float_value = hflag_float('f', "float", "float value", -20);
	char** string_value = hflag_str('s', "string", "string value", "hello there");
	hflag_str(0, "unused", "unused value", "useless");

	hflag_parse(&argc, &argv);

	printf("int value: %ld, bool value: %d, float value: %f, string value: %s\n", *int_value, *bool_value, *float_value, *string_value);

	printf("\nRemaining args:\n");
	for(int i = 0; i < argc; i++) {
		printf("%s\n", argv[i]);
	}
	return 0;
}
