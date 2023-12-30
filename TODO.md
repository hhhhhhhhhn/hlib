# Todo
- A small string type (?)
- Dequeue, and similar
- make install
- Clarify error handling
- Threads
  - Making `str_to_temp_cstr` thread safe (ring buffer?)
    - Even better: A sized `str_to_cstr_buf(str string, char* buf, usize buf_cap)`,
      and use that for all standard libraries (mainly `hfs.h`)
    - Deprecate `str_to_temp_cstr`
- Last modification timestamp for `HFSDirEntry`
  - Needs `htime`
- Parsing numbers
  - JSON (?)
- Spawning child processes
- `STR` for arrays
- Exit handler

## Done
- Arena allocator
- Argument parsing (with shift and such)
- Writing to file
- Sorting
- Switch to Rust-style primitive types
- A way to write information into the --help menu
- Make strings more convenient
  - Removing all char*
- File making clear vocabulary and naming conventions
- Temporal `str_to_cstr` using a reusable stack allocated string
- File handling, ls and find equivalents
- `str_split_by_str`
- `str_{starts,ends}_with`, `str_remove_{start,end}`
- Better memory debugging (dump allocations)
  - `-fsanitize`

## Idea for Thread Local Variables
```c
HThreadLocal data = HTL(char[256]);
// A.k.a.
HThreadLocal data = {
    .size: sizeof(char[256]),
    .key_creation_mutex: PTHREAD_MUTEX_INITIALIZER,
    .key: {0},
}

// TODO: Handle mutex and {get/set}_specific failures
void* hthread_get_local(HThreadLocal* data) {
    if (!data-key) {
        pthread_mutex_lock(&data.key_creation_mutex);
        if (!data->key) { // Prevents race: the first check might be true for
                          // multiple threads at once, so one locks the mutex
                          // and creates the key while the other waits
                          // Then, the first one finishes
                          // and the second one, now free, re-creates the key
            pthread_key_create(&data->key, free);
            // TODO: use atexit() to pthread_key_delete()
            // May need a handler, because atexit() does not take arguments
              // Even if it took arguments, it would still be called only once,
              // because atexit only calls every function pointer once.
              // Maybe DIY on_exit() (?)
        }
        pthread_mutex_unlock(&data.key_creation_mutex);
    }

    // From here on, the key is constant

    void* ptr = pthread_getspecific(data->key);
    // From here on, the ptr is thread-specific, so no race conditions
    if(ptr == NULL) {
        ptr = malloc(data->size);
        nullpanic(ptr);
        pthread_setspecific(data->key, ptr);
    }

    return ptr;
}
```
<https://www.ibm.com/docs/en/zos/2.2.0?topic=functions-pthread-setspecific-set-thread-specific-value-key>
<https://pubs.opengroup.org/onlinepubs/9699919799/functions/pthread_key_create.html>
