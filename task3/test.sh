make clean

make
echo "$1:" >> results.csv

./zad3 $1 1 >> results.csv

./zad3 $1 2 >> results.csv

./zad3 $1 4 >> results.csv

./zad3 $1 8 >> results.csv

./zad3 $1 16 >> results.csv

./zad3 $1 32 >> results.csv


make clean
