CC = gcc
CFLAGS = -Wall -pthread

all: mode1 mode2

mode1: sudoku_validator_mode1.c
	$(CC) $(CFLAGS) -o mode1 sudoku_validator_mode1.c

mode2: sudoku_validator_mode2.c
	$(CC) $(CFLAGS) -o mode2 sudoku_validator_mode2.c

clean:
	rm -f mode1 mode2
