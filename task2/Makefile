all: zad1

run: ./zad1
	mpisubmit.pl ./zad1 a b c 0

generate_d: ./zad1 
	./zad1 + a d $$n
	./zad1 + b d $$n
	./zad1 + c d $$n

generate_l: ./zad1
	./zad1 + a l $$n
	./zad1 + b l $$n
	./zad1 + c l $$n

test: ./test.sh
	chmod +x test.sh
	./test.sh $$n

zad1: zad1.c
	g++ zad1.c -o zad1 -Werror -pthread -O0 -lpapi

clean:
	rm -f zad1
	rm -f a
	rm -f b
	rm -f c

