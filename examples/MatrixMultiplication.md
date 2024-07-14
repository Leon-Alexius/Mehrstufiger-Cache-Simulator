# Algorithm: Matrix Multiplication

In this project, Matrix Multiplication is choosen to understand how different patterns of accessing matrix elements correlates to cache behavior and can affect the runtime of the multiplication operation.

## Representation of a Matrix

To make it easier to analyze, we will represent matrix `A` as a 1D-Array and thus the matrix element  `A[x][y]` can be addressed as `A[x * width + y]`.

For example, suppose we have a float-type matrix `A`, `A[n][m]` can be represented as `A[n * width + m]`:

```ocaml
[ A[0][0] | A[0][1] | A[0][2] | A[0][3] | A[1][0] | A[1][1] | A[1][2] | A[1][3] | A[2][0] | A[2][1] | A[2][2] | A[2][3] | A[3][0] | A[3][1] | A[3][2] | A[3][3] ]
```

The cache loads adjacent addresses into the same cacheLine, meaning that a cache with `cacheLineSize` of `64` Bytes can load the matrix element from `A[0]` to `A[15]` (`16` elements) to one of its' `cacheLine`.

## Matrix Multiplication Analysis

Given the matrix multiplication:
```ocaml
A[i][k] * B[k][j] = C[i][j]
```
We can have different access-combinations from this 3 variabls `i`, `k`, `j`. (see `examples/matrix.c`)
Running the test using different `n` using C gives:

```text
n = 100
| Method     | Time (s)   |
|------------|------------|
| ijk        | 0.003000   |
| jik        | 0.002000   |
| ikj        | 0.004000   |
| jki        | 0.003000   |
| kij        | 0.004000   |
| kji        | 0.004000   |

n = 500
| Method     | Time (s)   |
|------------|------------|
| ijk        | 0.392000   |
| jik        | 0.331000   |
| ikj        | 0.429000   |
| jki        | 0.522000   |
| kij        | 0.402000   |
| kji        | 0.520000   |

n = 1000
| Method     | Time (s)   |
|------------|------------|
| ijk        | 3.093000   |
| jik        | 2.765000   |
| ikj        | 3.293000   |
| jki        | 10.422000  |
| kij        | 3.345000   |
| kji        | 10.265000  |
```
Here we can see that some patterns (such as `jki` and `kji`) is significantly slower than the other patterns.
We will now analyze it.

### Theoretical Cache Performance
For this analysis, we will set some rules to simplify things:
- both `A` and `B` matrix as `float_square_matrix` of size `16`.
- The `cacheLineSize` will be set to `64` Bytes.
- Only one row for each Matrix can be loaded to the cache, and the architecture has only `1` cache and `1` main memory.

1. Pattern `ijk`:
```C
void matrix_multiplication_ijk(float* a, float* b, float* result, int n) {
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            float sum = 0.0f;
            for (int k = 0; k < n; k++) {
                sum += a[i * n + k] * b[k * n + j];
            }
            result[i * n + j] = sum;
        }
    }
}
```

Given `A[i][k] * B[k][j] = C[i][j]`
- For matrix `A`, each row `k` is loaded to the cacheLine when its' first row element is read. This implies that every new `i` read (including start-cold miss) is a cache miss. Therefore, for each matrix_operation we have 1/16 memory access.
- For matrix `B`, we always have different read access `k` (collumn), this leads to cache miss. Therefore, for each matrix_operation we have 1 memory access.
- For matrix `C`, each matrix element will be written when it is complete (because the `sum` is in stack), thus memory_access = 0 for each matrix_operation (only write every 16 matrix_operation)

We can conclude that in average, each matrix operation has 1.0625 memory access

2. Pattern `jik`
```C
void matrix_multiplication_jik(float* a, float* b, float* result, int n) {
    for (int j = 0; j < n; j++) {
        for (int i = 0; i < n; i++) {
            float sum = 0.0f;
            for (int k = 0; k < n; k++) {
                sum += a[i * n + k] * b[k * n + j];
            }
            result[i * n + j] = sum;
        }
    }
}
```

Given `A[i][k] * B[k][j] = C[i][j]`
`ijk` vs `jik`
The average memory access is also 1.0625 here, see case `1`.


However, there is a noticable difference here: Theoretically, the pattern `jik` will cause many cache misses for `C` compared to the pattern `ijk`. If we have loaded a row of `C` as a part of write-back policy in Cache, we would have to load more rows from `C` due to misses (we also need to flush the changes more frequently to the memory due to the cache getting full much faster - compare to `ijk`).

3. Pattern `ikj`
```C
void matrix_multiplication_ikj(float* a, float* b, float* result, int n) {
    for (int i = 0; i < n; i++) {
        for (int k = 0; k < n; k++) {
            for (int j = 0; j < n; j++) {
                result[i * n + j] += a[i * n + k] * b[k * n + j];
            }
        }
    }
}
```

Given `A[i][k] * B[k][j] = C[i][j]`
- For matrix `A`, each row `k` is loaded to the cacheLine when its' first row element is read. This implies that every new `i` read (including cold miss) is a cache miss. Therefore, for each matrix_operation we have 1/16 memory access.
- For matrix `B`, we also now access it row-wise, same as matrix `A`, therefore 1/16 memory access
- For matrix `C`, we now read then write to the matrix, same reasoning as `A`, we have 1/16 memory access

We can conclude that in average, each matrix operation has 0.1875 memory access

4. Pattern `kij`
```C
void matrix_multiplication_kij(float* a, float* b, float* result, int n) {
    for (int k = 0; k < n; k++) {
        for (int i = 0; i < n; i++) {
            for (int j = 0; j < n; j++) {
                result[i * n + j] += a[i * n + k] * b[k * n + j];
            }
        }
    }
}
```

Given `A[i][k] * B[k][j] = C[i][j]`
- For `A`, it is always cache miss = 1
- For `B`, it miss for each new `k` = 1/16
- For `C`, it miss for each new `i` = 1/16

In average = 1.125 memory access per matrix operation

5. Pattern `jki`
```C
void matrix_multiplication_jki(float* a, float* b, float* result, int n) {
    for (int j = 0; j < n; j++) {
        for (int k = 0; k < n; k++) {
            for (int i = 0; i < n; i++) {
                result[i * n + j] += a[i * n + k] * b[k * n + j];
            }
        }
    }
}
```

Given `A[i][k] * B[k][j] = C[i][j]`
- For `A`, it is always cache miss = 1
- For `B`, it is always cache miss = 1
- For `C`, it is always cache miss = 1

In average = 3 memory access per matrix operation

6. Pattern `kji`
```C
void matrix_multiplication_kji(float* a, float* b, float* result, int n) {
    for (int k = 0; k < n; k++) {
        for (int j = 0; j < n; j++) {
            for (int i = 0; i < n; i++) {
                result[i * n + j] += a[i * n + k] * b[k * n + j];
            }
        }
    }
}
```

Given `A[i][k] * B[k][j] = C[i][j]`
- For `A`, it is always cache miss = 1
- For `B`, it misses every new `k` = 1/16
- For `C`, it is always cache miss = 1

In average = 2.0625 memory access per matrix operation

We can conclude the result of the analysis:
```
| Pattern | Average Memory Access |
|---------|-----------------------|
| ijk     | 1.0625                |
| jik     | 1.0625                |
| ikj     | 0.1875                |
| jki     | 3                     |
| kij     | 1.125                 |
| kji     | 2.0625                |
```

By this very simplified analysis (only with basis of memory access), the pattern from fastest to slowest:
`ikj, ijk, jik, kij, kji, jki`

Of course, this analysis is not 100% accurate, depending on how the cache is implemented, some pattern might be slower even thought they have lower Average Memory Access.

Also note that `ikj` has higher read/ write operations (see `C`) which would make it slower at higher size `n`

### By Cache Simulator

Even if some patterns such as `ikj` has lower Average Memory Access, due to the fact that the simulator is `write_through`, the total cycles needed for such patterns will be bigger than `ijk` or `jik`.

Here, we can run the simulator to see how many cycles does a pattern need (without optimization):
```text

```

Now, we will run the simulator with optimization (Storeback buffer/ Write through with conditional flush buffer):
```text

```


## Correctness
Here, we describe the term correctness as **whether our cache simulator and matrix multiplication algorithm are implemented in a way that adheres to their expected behaviors and specifications**.

In Example:
- If the cache management policy is direct associative, then the simulator is implemented as direct associative
- Matrix Multiplication Algorithm should return same correct result no matter the sequence of the accesses.

We will discuss this starting from the Matrix Multiplication Algorithm.
By using `examples/test_correctness_matrix.c`, we can safely conclude that the algorithm returns valid and correct result for all sequence of accesses.

Sample test with same Matrix `A` and `B` for each sequence yields this result:
Note that we set `n` as `5` and both `A` and `B` is a square matrix.
```
Matrix A
     41.00      67.00      34.00       0.00      69.00
     24.00      78.00      58.00      62.00      64.00
      5.00      45.00      81.00      27.00      61.00
     91.00      95.00      42.00      27.00      36.00
     91.00       4.00       2.00      53.00      92.00

Matrix B
     82.00      21.00      16.00      18.00      95.00
     47.00      26.00      71.00      38.00      69.00
     12.00      67.00      99.00      35.00      94.00
      3.00      11.00      22.00      33.00      73.00
     64.00      41.00      11.00      53.00      68.00

Result of ijk
  11335.00    7710.00    9538.00    8131.00   16406.00
  10612.00    9724.00   13732.00   10864.00   21992.00
   7482.00    9500.00   12559.00    8759.00   17313.00
  14816.00    8968.00   13349.00    9517.00   23567.00
  13721.00    6504.00    4116.00    8485.00   19234.00

Result of jik
  11335.00    7710.00    9538.00    8131.00   16406.00
  10612.00    9724.00   13732.00   10864.00   21992.00
   7482.00    9500.00   12559.00    8759.00   17313.00
  14816.00    8968.00   13349.00    9517.00   23567.00
  13721.00    6504.00    4116.00    8485.00   19234.00

Result of ikj
  11335.00    7710.00    9538.00    8131.00   16406.00
  10612.00    9724.00   13732.00   10864.00   21992.00
   7482.00    9500.00   12559.00    8759.00   17313.00
  14816.00    8968.00   13349.00    9517.00   23567.00
  13721.00    6504.00    4116.00    8485.00   19234.00

Result of jki
  11335.00    7710.00    9538.00    8131.00   16406.00
  10612.00    9724.00   13732.00   10864.00   21992.00
   7482.00    9500.00   12559.00    8759.00   17313.00
  14816.00    8968.00   13349.00    9517.00   23567.00
  13721.00    6504.00    4116.00    8485.00   19234.00

Result of kij
  11335.00    7710.00    9538.00    8131.00   16406.00
  10612.00    9724.00   13732.00   10864.00   21992.00
   7482.00    9500.00   12559.00    8759.00   17313.00
  14816.00    8968.00   13349.00    9517.00   23567.00
  13721.00    6504.00    4116.00    8485.00   19234.00

Result of kji
  11335.00    7710.00    9538.00    8131.00   16406.00
  10612.00    9724.00   13732.00   10864.00   21992.00
   7482.00    9500.00   12559.00    8759.00   17313.00
  14816.00    8968.00   13349.00    9517.00   23567.00
  13721.00    6504.00    4116.00    8485.00   19234.00
```

We can then compare it with the control result (using [online calculator](https://matrix.reshish.com/multCalculation.php)):
```
    11335	7710	9538	8131	16406
    10612	9724	13732	10864	21992
    7482	9500	12559	8759	17313
    14816	8968	13349	9517	23567
    13721	6504	4116	8485	19234
```

Next is ensuring the correctness of the Cache Simulator:
1. Ensuring that the cache simulator correctly implements the cache management policies and accurately simulates cache behavior (e.g., hits, misses) for given sequences of memory accesses.

## Accuracy
We describe the term accuracy as **how well the simulator reflects the actual behavior and performance of a cache in a real system**. 

Notes:
1. The bigger the cache is the slower they are (higher latency to check which cache line the data is, comparing the tags, etc.), but in the simulator, even if one were to set `cacheLineSize` to `0xFFFF.FFFF` (max value of `unsigned int`), the latency of said cache will be the same. (same for the `l1CacheLines`)
2. In real-life, the scheduler that decides the size of resources a process is given will also effects performance of the algorithm, however, in our simulation, for the same input, the output (e.g. total cycles count) will be always the same.

Demonstrating accuracy involve:
- Accuracy can be evaluated by comparing the simulator's predictions with real-world measurements or more detailed simulations.
- Comparing the cache hit/miss rates and execution times predicted by your simulator with those obtained from real hardware or a more detailed and validated cache simulation tool.
- Analyzing how changes in cache configuration (size, associativity, block size) affect the performance of matrix multiplication in your simulator and comparing these effects with theoretical expectations or empirical data.








Author: Lie Leon Alexius