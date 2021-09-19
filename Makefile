CC=gcc

all: zad0.cpp
	$(CC) zad0.cpp -o zad0 -Wall -Werror -pthread -Wpedantic

zad0: zad0.cpp
	$(CC) zad0.cpp -o zad0 -Wall -Werror -pthread -Wpedantic

run: zad0
	./zad0 $A $B

test: test.sh
	$(CC) zad0.cpp -o zad0 -Wall -Werror -pthread -Wpedantic -fsanitize=address
	sh test.sh

