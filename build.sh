make

echo "IJK Standard"
./cache -c 2147483646 --cacheline-size 16 --l1-lines 4 --l2-lines 16 --pretty-print true examples/ijk.csv
echo "=================================================================================="

echo "IJK Optimized 1"
./cache --cacheline-size 16 --l1-lines 4 --l2-lines 16 --pretty-print true examples/ijk_opt1.csv
echo "=================================================================================="

echo "IJK Optimized 2"
./cache --cacheline-size 16 --l1-lines 4 --l2-lines 16 --pretty-print true examples/ijk_opt2.csv
echo "=================================================================================="

echo "IJK Standard with Storeback Buffer"
./cache --cacheline-size 16 --l1-lines 4 --l2-lines 16 --storeback-buffer 4 --pretty-print true examples/ijk.csv
echo "=================================================================================="

echo "IJK Optimized 1 with Storeback Buffer"
./cache --cacheline-size 16 --l1-lines 4 --l2-lines 16 --storeback-buffer 4 --pretty-print true examples/ijk_opt1.csv
echo "=================================================================================="

echo "IJK Optimized 2 with Storeback Buffer"
./cache --cacheline-size 16 --l1-lines 4 --l2-lines 16 --storeback-buffer 4 --pretty-print true examples/ijk_opt2.csv
echo "=================================================================================="

make clean