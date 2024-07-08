make clean
make

# ./cache --num-requests 7 src/assets/csv/test_valid.csv

echo "JIK"
./cache --cacheline-size 16 --l1-lines 4 --l2-lines 16 src/assets/matrix_multiplication/jik.csv
echo "=================================================================================="

echo "KIJ"
./cache --cacheline-size 16 --l1-lines 4 --l2-lines 16 src/assets/matrix_multiplication/kij.csv
echo "=================================================================================="

echo "IJK"
./cache --cacheline-size 16 --l1-lines 4 --l2-lines 16 src/assets/matrix_multiplication/ijk.csv
echo "=================================================================================="

echo "IKJ"
./cache --cacheline-size 16 --l1-lines 4 --l2-lines 16 src/assets/matrix_multiplication/ikj.csv
echo "=================================================================================="

echo "KJI"
./cache --cacheline-size 16 --l1-lines 4 --l2-lines 16 src/assets/matrix_multiplication/kji.csv
echo "=================================================================================="

echo "JKI"
./cache --cacheline-size 16 --l1-lines 4 --l2-lines 16 src/assets/matrix_multiplication/jki.csv

echo "Simulation Ended"
