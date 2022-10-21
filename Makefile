all: main

.PHONY: main
main: main.c tries.c
	gcc -Wall -Wextra -o $@ $^

clean:
	rm -rf main *.o