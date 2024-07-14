make

echo "JIK"
./cache --cacheline-size 16 --l1-lines 4 --l2-lines 16 --pretty-print true examples/jik.csv
echo "=================================================================================="

# echo "KIJ"
# ./cache --cacheline-size 16 --l1-lines 4 --l2-lines 16 --pretty-print false examples/kij.csv
# echo "=================================================================================="

# echo "IJK"
# ./cache --cacheline-size 16 --l1-lines 4 --l2-lines 16 --pretty-print false examples/ijk.csv
# echo "=================================================================================="

# echo "IJK with Storeback Buffer"
# ./cache --cacheline-size 16 --l1-lines 4 --l2-lines 16 --storeback-buffer 4 --pretty-print false examples/ijk.csv
# echo "=================================================================================="

# echo "IJK with Storeback Buffer and Prefetching"
# ./cache --cacheline-size 16 --l1-lines 4 --l2-lines 16 --prefetch-buffer 4 --pretty-print false examples/ijk.csv  
# echo "=================================================================================="

# echo "IKJ"
# ./cache --cacheline-size 16 --l1-lines 4 --l2-lines 16 --pretty-print false examples/ikj.csv
# echo "=================================================================================="

# echo "KJI"
# ./cache --cacheline-size 16 --l1-lines 4 --l2-lines 16 --pretty-print false examples/kji.csv
# echo "=================================================================================="

# echo "JKI"
# ./cache --cacheline-size 16 --l1-lines 4 --l2-lines 16 --pretty-print false examples/jki.csv

make clean