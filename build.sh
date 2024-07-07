make clean
make
./cache --num-requests 3 src/assets/csv/test_valid.csv

# errors:
# free() invalid pointer: 7, 11, 15, 19, 23
# munmap_chunk(): 8, 12, 16, 20