#include "core.h"
#include <stdbool.h>
#include <stdint.h>
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef enum HFlagType {
	HFLAGTYPE_BOOL,
	HFLAGTYPE_INT,
	HFLAGTYPE_DOUBLE,
	HFLAGTYPE_STRING
} HFlagType;

// TODO: This assumes that the pointer size is <= 64 bit
typedef struct HFlag {
	char      short_name;
	char*     long_name;
	char*     description;
	HFlagType type;
	uint64_t  default_value;
	uint64_t  value;
} HFlag;


#define FLAGS_CAP 128
HFlag flags[FLAGS_CAP] = {
	{
		.short_name = 'h',
		.long_name = "help",
		.description = "Print this help message and exit.",
		.type = HFLAGTYPE_BOOL,
		.default_value = 0,
		.value = 0
	}
};
size_t flags_len = 1;

#define EXTRA_ARGS_CAP 128
char*  extra_args[EXTRA_ARGS_CAP];
size_t extra_args_len = 0;

int64_t* hflag_int(char short_name, char* long_name, char* description, int64_t default_value) {
	assert(flags_len < FLAGS_CAP);
	nullpanic(long_name);
	flags[flags_len] = (HFlag){
		.short_name = short_name,
		.long_name = long_name,
		.description = description,
		.type = HFLAGTYPE_INT,
		.default_value = *(uint64_t*)&default_value,
		.value = *(uint64_t*)&default_value,
	};
	flags_len++;
	return (int64_t*)&flags[flags_len-1].value;
}

char** hflag_str(char short_name, char* long_name, char* description, char* default_value) {
	assert(flags_len < FLAGS_CAP);
	nullpanic(long_name);
	flags[flags_len] = (HFlag){
		.short_name = short_name,
		.long_name = long_name,
		.description = description,
		.type = HFLAGTYPE_STRING,
		.default_value = *(uint64_t*)&default_value,
		.value = *(uint64_t*)&default_value,
	};
	flags_len++;
	return (char**)&flags[flags_len-1].value;
}

double* hflag_double(char short_name, char* long_name, char* description, double default_value) {
	assert(flags_len < FLAGS_CAP);
	nullpanic(long_name);
	flags[flags_len] = (HFlag){
		.short_name = short_name,
		.long_name = long_name,
		.description = description,
		.type = HFLAGTYPE_DOUBLE,
		.default_value = *(uint64_t*)&default_value,
		.value = *(uint64_t*)&default_value,
	};
	flags_len++;
	return (double*)&flags[flags_len-1].value;
}

bool* hflag_bool(char short_name, char* long_name, char* description) {
	assert(flags_len < FLAGS_CAP);
	nullpanic(long_name);
	flags[flags_len] = (HFlag){
		.short_name = short_name,
		.long_name = long_name,
		.description = description,
		.type = HFLAGTYPE_BOOL,
		.default_value = (uint64_t)false,
		.value = (uint64_t)false,
	};
	flags_len++;
	return (bool*)&flags[flags_len-1].value;
}

// internal
uint64_t hflag_parse_value(HFlagType type, char* value) {
	nullpanic(value);
	switch(type) {
		case HFLAGTYPE_BOOL:
			unreachable(); // Flags do not need a value, they are set to true with just -f, not -f true|false
			return 0;
		case HFLAGTYPE_INT: {
			char* endptr;
			int64_t result = strtoll(value, &endptr, 10);
			if (*endptr != '\0') {
				panicf("Invalid integer value: %s", value);
			}
			return *(uint64_t*)&result;
		}
		case HFLAGTYPE_DOUBLE: {
			char* endptr;
			double result = strtod(value, &endptr);
			if (*endptr != '\0') {
				panicf("Invalid double value: %s", value);
			}
			return *(uint64_t*)&result;
		}
		case HFLAGTYPE_STRING: {
			return *(uint64_t*)&value;
		}
	}
	unreachable();
	return 0;
}

void print_help(char* program_name) {
	fprintf(stderr, "Usage: %s [options]\n", program_name);
	for (size_t i = 0; i < flags_len; i++) {
		HFlag flag = flags[i];
		if(flag.short_name) {
			fprintf(stderr, "    -%c,  ", flag.short_name);
		}
		else {
			fprintf(stderr, "         ");
		}

		fprintf(stderr, "--%-10s  %s ", flag.long_name, flag.description);
		switch (flag.type) {
			case HFLAGTYPE_BOOL:
				fprintf(stderr, "[bool, default: false]\n");
				break;
			case HFLAGTYPE_INT:
				fprintf(stderr, "[int, default: %li]\n", *(int64_t*)&flag.default_value);
				break;
			case HFLAGTYPE_STRING:
				fprintf(stderr, "[string, default: %s]\n", *(char**)&flag.default_value);
				break;
			case HFLAGTYPE_DOUBLE:
				fprintf(stderr, "[float, default: %lf]\n", *(double*)&flag.default_value);
				break;
		}
	}
}

// NOTE: If argument parsing fails, this function just fails
void hflag_parse(int* argc_pointer, char*** argv_pointer) {
	char** argv = *argv_pointer;
	int argc = *argc_pointer;

	char* program_name = argv[0];
	argv++; argc--;

	while (argc > 0) {
		char* arg = argv[0];
		nullpanic(arg);

		if (arg[0] == '-' && arg[1] == '-' && arg[2] == '\0') { // stop parsing, interpret literally
			argv++; argc--;
			while(argc > 0) {
				assert(extra_args_len < EXTRA_ARGS_CAP);
				extra_args[extra_args_len] = argv[0];
				extra_args_len++;
				argv++; argc--;
			}
		}
		else if (arg[0] == '-' && arg[2] == '\0') { // short name
			char name = arg[1];
			int flag = -1;

			for(size_t i = 0; i < flags_len; i++) {
				if (flags[i].short_name == name) {
					flag = i;
					break;
				}
			}

			if (flag == -1) panicf("Unknown flag '%c'", name);
			if (flags[flag].type == HFLAGTYPE_BOOL) {
				flags[flag].value = (uint64_t)true;
			}
			else {
				argv++; argc--;
				if (argc == 0) panicf("Flag '%c' requires an argument", name);
				int64_t value = hflag_parse_value(flags[flag].type, argv[0]);
				flags[flag].value = *(uint64_t*)&value;
			}
		}
		else if (arg[0] == '-' && arg[1] == '-') { // long name
			char* name = &arg[2];

			int flag = -1;
			for(size_t i = 0; i < flags_len; i++) {
				if (strcmp(flags[i].long_name, name) == 0) {
					flag = i;
					break;
				}
			}

			if (flag == -1) panicf("Unknown flag '%s'", name);
			if (flags[flag].type == HFLAGTYPE_BOOL) {
				flags[flag].value = (uint64_t)true;
			}
			else {
				argv++; argc--;
				if (argc == 0) panicf("Flag '%s' requires an argument", name);
				int64_t value = hflag_parse_value(flags[flag].type, argv[0]);
				flags[flag].value = *(uint64_t*)&value;
			}
		}
		else { // Extra argument, not a flag
			assert(extra_args_len < EXTRA_ARGS_CAP);
			extra_args[extra_args_len] = arg;
			extra_args_len++;
		}
		argv++; argc--;
	}

	*argv_pointer = extra_args;
	*argc_pointer = extra_args_len;

	if (flags[0].value == 1) { // Help flag activated
		print_help(program_name);
		exit(0);
	}
}
