make clean

make generate_d n=$1

make

./zad1 a b c 0 >> results.csv

./zad1 a b c 1 >> results.csv

./zad1 a b c 2 >> results.csv

./zad1 a b c 3 >> results.csv

./zad1 a b c 4 >> results.csv

./zad1 a b c 5 >> results.csv

make generate_l n=$1

./zad1 a b c 0 >> results.csv

./zad1 a b c 1 >> results.csv

./zad1 a b c 2 >> results.csv

./zad1 a b c 3 >> results.csv

./zad1 a b c 4 >> results.csv

./zad1 a b c 5 >> results.csv

make clean
