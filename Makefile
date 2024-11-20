CC=gcc
CFLAGS=-Wall -Wextra -std=c99

# Source file de modificat
SRC=main.c task1.c task2.c

# Executable file
EXEC=tema3

# Default target
all: build

build: $(SRC)
	$(CC) $(CFLAGS) -g -o $(EXEC) $^ -lm

clean:
	rm -f $(EXEC)
