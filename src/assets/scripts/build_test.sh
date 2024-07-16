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

echo "IJK Standard with Unconditional Storeback Buffer"
./cache -c 2000000 --cacheline-size 16 --l1-lines 4 --l2-lines 16 --storeback-buffer 4 -p true examples/ijk/ijk.csv
echo "=================================================================================="

echo "IJK Optimized 1 with Unconditional Storeback Buffer"
./cache -c 2000000 --cacheline-size 16 --l1-lines 4 --l2-lines 16 --storeback-buffer 4 -p true examples/ijk/ijk_opt1.csv
echo "=================================================================================="

echo "IJK Optimized 2 with Unconditional Storeback Buffer"
./cache -c 2000000 --cacheline-size 16 --l1-lines 4 --l2-lines 16 --storeback-buffer 4 -p true examples/ijk/ijk_opt2.csv
echo "=================================================================================="

echo "IJK Standard with Conditional Storeback Buffer"
./cache -c 2000000 --cacheline-size 16 --l1-lines 4 --l2-lines 16 --storeback-buffer 4 --storeback-condition true -p true examples/ijk/ijk.csv
echo "=================================================================================="

echo "IJK Optimized 1 with Conditional Storeback Buffer"
./cache -c 2000000 --cacheline-size 16 --l1-lines 4 --l2-lines 16 --storeback-buffer 4 --storeback-condition true -p true examples/ijk/ijk_opt1.csv
echo "=================================================================================="

echo "IJK Optimized 2 with Conditional Storeback Buffer"
./cache -c 2000000 --cacheline-size 16 --l1-lines 4 --l2-lines 16 --storeback-buffer 4 --storeback-condition true -p true examples/ijk/ijk_opt2.csv
echo "=================================================================================="

echo "IJK Standard with Prefetching"
./cache -c 50000000 --cacheline-size 16 --l1-lines 4 --l2-lines 16 --prefetch-buffer 4 -p true examples/ijk/ijk.csv
echo "=================================================================================="

echo "IJK Optimized 1 with Prefetching"
./cache -c 50000000 --cacheline-size 16 --l1-lines 4 --l2-lines 16 --prefetch-buffer 4 -p true examples/ijk/ijk_opt1.csv
echo "=================================================================================="

echo "IJK Optimized 2 with Prefetching"
./cache -c 50000000 --cacheline-size 16 --l1-lines 4 --l2-lines 16 --prefetch-buffer 4 -p true examples/ijk/ijk_opt2.csv
echo "=================================================================================="

make clean