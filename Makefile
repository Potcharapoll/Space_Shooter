CC      = gcc
CFLAGS  = -std=c11 -O3 -Wall -Wextra -Wpedantic -g -Wstrict-aliasing
CFLAGS += -Wno-unused-parameter -Wno-missing-field-initializer

LD      = gcc
LDFLAGS = lib/glad/src/glad.o lib/cglm/libcglm.a lib/glfw/src/libglfw3.a -lm

SRC     = $(shell find src -name "*.c")
OBJ     = $(SRC:.c=.o)
INCLUDE = -Ilib/cglm/include -Ilib/glad/include -Ilib/glfw/include -Ilib/stb
BIN     = bin

.PHONY: all clean

all: dirs libs game

libs:
	cd lib/cglm && cmake . -DCGLM_STATIC=ON && make
	cd lib/glad && $(CC) -Iinclude -c src/glad.c -o src/glad.o
	cd lib/glfw && cmake . && make

dirs:
	mkdir -p ./$(BIN)

game: $(OBJ)
	$(CC) -o $(BIN)/game $^ $(LDFLAGS)

%.o: %.c
	$(CC) -o $@ -c $< $(CFLAGS) $(INCLUDE)

run: all
	$(BIN)/game

clean:
	rm -rf $(OBJ) $(BIN) lib/glad/src/glad.o
