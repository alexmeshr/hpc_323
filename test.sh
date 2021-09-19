echo "start testing... \nnum-intervals = 10, num-threads = 1"
./zad0 10 1
echo "\nnum-intervals = 10, num-threads = 10"
./zad0 10 10
echo "\nnum-intervals = 100000000, num-threads = 1"
./zad0 100000000 1
echo "\nnum-intervals = 100000000, num-threads = 2"
./zad0 100000000 2
echo "\nnum-intervals = 100000000, num-threads = 3"
./zad0 100000000 3
echo "\nnum-intervals = 100000000, num-threads = 10"
./zad0 100000000 10
echo "\nnum-intervals = 100000000, num-threads = 100"
./zad0 100000000 100
echo "\nend of test"

