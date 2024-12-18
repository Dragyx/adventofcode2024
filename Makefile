FLAGS=-O3 -Wall -march=native -Wno-unused-function -fsanitize=address -ggdb3
FILES=src/helpers.c src/day*.c src/main.c src/common.c 


run: all
	./aoc

all: $(FILES)
	gcc $(FLAGS) -o aoc $(FILES)

clean:
	rm ./aoc

.PHONY: clean
