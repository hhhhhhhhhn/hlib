#include "hlib/hflag.h"
#include <stdint.h>
#include <stdio.h>

int main(int argc, char** argv) {
	int64_t* int_value = hflag_int('i', "int", "integer value", -12);
	bool* bool_value = hflag_bool('b', "bool", "boolean value");
	double* double_value = hflag_double('d', "double", "double value", -20);
	char** string_value = hflag_str('s', "string", "string value", "hello there");
	hflag_str(0, "unused", "unused value", "useless");

	hflag_parse(&argc, &argv);

	printf("int value: %ld, bool value: %d, double value: %f, string value: %s\n", *int_value, *bool_value, *double_value, *string_value);

	printf("\nRemaining args:\n");
	for(int i = 0; i < argc; i++) {
		printf("%s\n", argv[i]);
	}
	return 0;
}
