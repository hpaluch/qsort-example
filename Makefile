CPPFLAGS = -D_BSD_SOURCE=1
CFLAGS = -Wall -g
LDFLAGS = -g

all : qsort_example
qsort_example : qsort_example.o
qsort_example.o : qsort_example.c

.PHONY: run
run : qsort_example
	./qsort_example

.PHONY: valgrind
valgrind : qsort_example
	valgrind ./qsort_example

.PHONY: clean
clean: 
	rm -f qsort_example *.o


