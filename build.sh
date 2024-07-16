# This file is needed for Artemis output

make release

echo "IJK Standard"
./cache -c 2000000 --cacheline-size 16 --l1-lines 4 --l2-lines 16 -p true examples/ijk/ijk.csv
echo "=================================================================================="

echo "IJK Optimized 1"
./cache -c 2000000 --cacheline-size 16 --l1-lines 4 --l2-lines 16 -p true examples/ijk/ijk_opt1.csv
echo "=================================================================================="

echo "IJK Optimized 2"
./cache -c 2000000 --cacheline-size 16 --l1-lines 4 --l2-lines 16 -p true examples/ijk/ijk_opt2.csv
echo "=================================================================================="

make clean