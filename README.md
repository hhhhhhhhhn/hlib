# hlib
`hlib` is my personal replacement for the C and POSIX standard library.
The goal is to not need to require the standard C headers for most programs,
instead replacing them with a saner alternative.

These are some principles for `hlib`:
- Do not follow all traditional C conventions,
  as some of them are outdated and cause a lot of pain.
  For example, instead of `char*`, `hlib` has a sized string type `str`.
  One of the roles of `hlib` is to as a translation layer on top
  of the standard C library for the new conventions.
- Be as simple as you can.
- Target C99, as it is the oldest and simplest C standard that has everything I
  need, and runs basically everywhere.
  Don't use compiler-specific tricks for anything.
- Include commonly reimplemented functions.
- Plan ahead for multiple platforms.
- Prevent unnecessary memory allocation.
- Don't abuse macros. `__LINE__` and `__FILE__` are acceptable for debugging,
  but apart from simple definitions trivial constructors (e.g. `STR` in `hstring.h`),
  user-facing macros should be avoided.
  Generics should done with `void*`.

## Types
The library includes different categories of files.
- Basic types: Done in Rust fashion (`i32`, `f64`, etc),
  plus `str` for a string view (does not own the memory, immutable)
  and `strb` for a string builder (does own the memory, must be freed).
  `char` and `bool` are also part of the basic set.
  `char` is for string data, and `u8` for binary data.
- Normal structs, unions and enums: Written in Pascal Case, e.g. `HVec`.
  They all start with an `H` prefix.
- Result types, of the form `[TYPE]Result` (e.g. `strbResult`),
  which are a struct the value and an `ok` boolean.
  See `strb_from_file` in `hstring.c` for an example.


## Naming conventions for functions and variables
Function names are verbose, written with snake_case,
and start with either the structure of the type or the module they are part of.
For example:

- `str_new`, found in `hstring.h` creates a new `str`
- `hstring`, also found in `hstring.h` frees the `str`
- `hvec_push`, found in `hvec.h` pushes an element to an `HVec`

Names are predictable, and follow a standard vocabulary:
- `TYPE_new` and `TYPE_free` are used for constructors and destructors.
  Note that "free" applies not only to memory, but also, for example, for
  closing files.
- `TYPE_from_TYPE` for creating a type from another type, for example, `str_from_strb`.
  `TYPE_to_TYPE` is also possible, but the former generally makes more sense.
- `file` refers to a `FILE*` and `filepath` to a path to a file.
- `push` refers to adding a single element to the end of a list,
  and `append` refers to adding many.
- `len` refers to the amount of elements in a collection,
  and `cap` refers to the amount of elements that fit in the memory allocated for a collection.
  The latter definition is different for the amount of elements a collection could hold.
  For example, if a hashmap allocates 100 elements, its `cap`acity is 100,
  even if it would reallocate once it passes 75 elements.
