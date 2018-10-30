all: bin/AODjustify

AODjustify: bin/AODjustify.o
	gcc -o AODjustify AODjustify.o

AODjustify.o: bin/AODjustify.c
	gcc -o bin/AODjustify.o -c bin/AODjustify.c -W -Wall -ansi -pedantic

.PHONY: clean

clean:
	rm -rf bin/*.o
	rm -rf bin/AODjustify
