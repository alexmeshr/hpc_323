echo "start testing... \nnum-intervals = 10, num-threads = 1" >> results.csv
./zad0 10 1 >> results.csv
echo "\nnum-intervals = 10, num-threads = 10"  >> results.csv
./zad0 10 10 >> results.csv
echo "\nnum-intervals = 100000000, num-threads = 1" >> results.csv
./zad0 100000000 1 >> results.csv
echo "\nnum-intervals = 100000000, num-threads = 2" >> results.csv
./zad0 100000000 2 >> results.csv
echo "\nnum-intervals = 100000000, num-threads = 3" >> results.csv
./zad0 100000000 3 >> results.csv
echo "\nnum-intervals = 100000000, num-threads = 10" >> results.csv
./zad0 100000000 10 >> results.csv
echo "\nnum-intervals = 100000000, num-threads = 100" >> results.csv
./zad0 100000000 100 >> results.csv
echo "\nend of test" >> results.csv

