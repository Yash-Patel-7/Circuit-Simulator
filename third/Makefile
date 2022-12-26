all: third

third: third.c
	rm -rf third && gcc -g -Wall -Werror -fsanitize=address -std=c11 third.c -o third

clean:
	rm -rf third

