#ifndef HLIB_HFLAG_H
#define HLIB_HFLAG_H

#include "core.h"
#include "hstring.h"

FILE* hfs_open_file(str path, bool read, bool writing);
void hfs_close_file(FILE* file);

#endif
