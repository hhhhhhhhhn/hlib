#ifndef HLIB_HFLAG_H
#define HLIB_HFLAG_H

#include <stdint.h>
#include <stdbool.h>

// Short names are optional, long names, descriptions, and default values aren't
int64_t* hflag_int(char short_name, char* long_name, char* description, int64_t default_value);
char** hflag_str(char short_name, char* long_name, char* description, char* default_value);
double* hflag_double(char short_name, char* long_name, char* description, double default_value);
bool* hflag_bool(char short_name, char* long_name, char* description);

void hflag_parse(int* argc_pointer, char*** argv_pointer);
#endif
