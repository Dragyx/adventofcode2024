FLAGS=-O3 -Wall
FILES=src/helpers.c src/datastructures.c src/day*.c src/main.c 


run: all
	./aoc

all:
	gcc $(FLAGS) -o aoc $(FILES)

clean:
	rm ./aoc

.PHONY: clean
