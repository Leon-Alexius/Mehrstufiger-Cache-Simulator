make clean
make
./cache --l1-lines 4 --cacheline-size 64 --l1-latency 1 --num-requests 2 src/assets/csv/test_valid.csv
