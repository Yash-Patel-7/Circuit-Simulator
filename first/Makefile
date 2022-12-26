all: first

first: first.c
	rm -rf first && gcc -g -Wall -Werror -fsanitize=address -std=c11 first.c -o first

clean:
	rm -rf first

