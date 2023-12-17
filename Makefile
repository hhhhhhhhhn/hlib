CFLAGS = -Wall -Werror -Wextra -Wpedantic -std=c99 -g

ifdef debug
	CFLAGS += -DHLIB_DEBUG
endif

all: hlib.o tests

hlib.o: $(wildcard hlib/*.c)
	cc $(CFLAGS) -c hlib/hlib.c -o hlib.o

%_test: %_test.c hlib.o
	cc $(CFLAGS) -o $@ $< hlib.o

tests: mem_test vec_test string_test hashmap_test

clean:
	rm -f *.o *_test
