all: zad3

test: ./test.sh
	chmod +x test.sh
	./test.sh $$n

zad3: zad3.c
	g++ -fopenmp zad3.c -o zad3 -Werror
clean:
	rm -f zad3 

