# Algorithm: Matrix Multiplication

In this project, Matrix Multiplication is choosen to understand how different patterns of accessing matrix elements correlates to cache behavior and can affect the runtime of the multiplication operation.

## Representation of a Matrix

To make it easier to analyze, we will represent float-matrix `A` as a 1D-Array and thus the matrix element  `A[x][y]` can be addressed as `A[x * width + y]` with `width` = number of columns.

For example:
```ocaml
[ A[0][0] | A[0][1] | A[0][2] | A[0][3] | A[1][0] | A[1][1] | A[1][2] | A[1][3] | A[2][0] | A[2][1] | A[2][2] | A[2][3] | A[3][0] | A[3][1] | A[3][2] | A[3][3] ]
```

Remember:
The cache loads adjacent addresses into the same cacheLine, e.g. a cache with `cacheLineSize` of `64` Bytes will load matrix elements from `A[0]` to `A[15]` (`16` elements) in one of its' `cacheLine` if `A[0]` is loaded from the memory.

## Matrix Multiplication Analysis

Given the matrix multiplication:
```ocaml
A[i][k] * B[k][j] = C[i][j]
```
We can have different access-combinations from this 3 variabls `i`, `k`, `j`. (see `examples/matrix.c`)
Running the combinations using different `n`in C gives:

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
Where several patterns is sometimes faster/ slower (will be discussed later).
We will now analyze it.

### Theoretical Analysis of Cache Performance
For this analysis, we will set some rules:
- `A`, `B`, `C` (`result`) matrix are `float_square_matrix` of size `n`.
- More detailed representation: `A[i][k] * B[k][j] = C[i][j]`
- The `cacheLineSize` will be set to `m` Bytes. Where `m` is equal to `sizeof(float) * n`

We will count the Average Memory Access per Matrix Operation.

1. Pattern `ijk`:
```C
void matrix_multiplication_ijk(float* a, float* b, float* result, int n) {
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            for (int k = 0; k < n; k++) {
                result[i * n + j] += a[i * n + k] * b[k * n + j];
            }
        }
    }
}
```

- For `A`: Access by row-wise (see `a[i * n + k]`), each loop `k` accesses the data that is already loaded in the cache, resulting in many read hits, the only time it misses (needs mem. access) is during the load of each row for the first time, in other words, every (0, n, 2n, ...) reads - **(1/n) mem. access/ matrix operation**
- For `B`: Access by column-wise (see `b[k * n + j]`), each loop `k` loads new matrix row to cache, which has a very big stride (if `n` is big enough) resulting in always read-miss - **1 mem. access/ matrix operation**
- For `C`: Access by row-wise (see `result[i * n + j]`), each loop `j` accesses the data that is already loaded in the cache, read miss will only trigger for each new `i`, meaning that 

We can conclude that in average **each matrix operation has (1 + 2/n) memory access**.

2. Pattern `jik`
```C
void matrix_multiplication_jik(float* a, float* b, float* result, int n) {
    for (int j = 0; j < n; j++) {
        for (int i = 0; i < n; i++) {
            for (int k = 0; k < n; k++) {
                result[i * n + j] += a[i * n + k] * b[k * n + j];
            }
        }
    }
}
```

We can also conclude that in average **each matrix operation has (1 + 2/n) memory access**. (see `ijk`)

However, there is a noticable difference here: In real life, the pattern `jik` has worse cache behaviour compared to the pattern `ijk` (see `result[i * n + j] = sum;`).

Example: If the cache is write-back (common in real life), The high frequency of `C` loads to cache would result in higher count of cache-eviction and write-backs.

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

- For `A`: Each `k` read is a read hit since the row is already loaded to the cacheLine when its' first row element is read. In other words, every new `i` causes a cache miss (including cold miss). For each matrix operation we have **(1/n) memory access**.
- For `B`: Same reasoning as `a`, we have **(1/n) memory access** for each matrix operation.
- For `C`: We also now read and write row-wise, we have **(1/n) memory access** for each matrix operation.

We can conclude that in average **each matrix operation has (3/n) memory access**.

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

- For `A`: it is accessed column-wise, always cache miss, **1 mem. access/ matrix operation**
- For `B`: it miss for each new `k`, **(1/n) memory access**
- For `C`: it miss for each new `i`, **(1/n) memory access**

We can conclude that in average **each matrix operation has (1 + 2/n) memory access**.

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
- For `A`, it is always cache miss, **1 mem. access/ matrix operation**
- For `B`, it is always cache miss, **1 mem. access/ matrix operation**
- For `C`, it is always cache miss, **1 mem. access/ matrix operation**

We can conclude that in average **each matrix operation has 3 memory access**.

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


# Correctness of Simulator
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

# Accuracy of Simulator
We describe the term accuracy as **how well the simulator reflects the actual behavior and performance of a cache in a real system**. 

Notes:
1. The bigger the cache is the slower they are (higher latency to check which cache line the data is, comparing the tags, etc.), but in the simulator, even if one were to set `cacheLineSize` to `0xFFFF.FFFF` (max value of `unsigned int`), the latency of said cache will be the same. (same for the `l1CacheLines`)
2. In real-life, the scheduler that decides the size of resources a process is given will also effects performance of the algorithm, however, in our simulation, for the same input, the output (e.g. total cycles count) will be always the same.

Demonstrating accuracy involve:
- Accuracy can be evaluated by comparing the simulator's predictions with real-world measurements or more detailed simulations.
- Comparing the cache hit/miss rates and execution times predicted by your simulator with those obtained from real hardware or a more detailed and validated cache simulation tool.
- Analyzing how changes in cache configuration (size, associativity, block size) affect the performance of matrix multiplication in your simulator and comparing these effects with theoretical expectations or empirical data.








Author: Lie Leon Alexius