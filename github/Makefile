CC = clang
LD = clang

CFLAGS = -Wall -Wextra -O0 -g -Iinclude
LDFLAGS = -lm
DEBUG = -DDEBUG -O0

# Par défaut, on compile tous les fichiers source (.c) qui se trouvent dans le
# répertoire bin/
SRC_FILES=$(wildcard bin/*.c)

# Par défaut, la compilation de bin/toto.c génère le fichier objet bin/toto.o
OBJ_FILES=$(patsubst bin/%.c,bin/%.o,$(SRC_FILES))

src/%.o: src/%.c
	$(CC) $(CFLAGS) $< -o $@

.PHONY: clean

clean:
	rm -rf $(OBJ_FILES)
