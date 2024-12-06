FLAGS=-O3 -Wall -Wno-unused-function -fsanitize=address -ggdb3
FILES=src/helpers.c src/day*.c src/main.c 


run: all
	./aoc

all:
	gcc $(FLAGS) -o aoc $(FILES)

clean:
	rm ./aoc

.PHONY: clean
