Author: Lie Leon Alexius

# Algorithm: Matrix Multiplication

In this project, Matrix Multiplication is choosen to understand how different patterns of accessing matrix elements correlates to cache behavior and can affect the runtime of the multiplication operation.

## Representation of a Matrix

To make it easier to analyze, we will represent float-matrix `A` as a 1D-Array and thus the matrix element  `A[x][y]` can be addressed as `A[x * width + y]` with `width` is equal to number of columns.

Example illustration of a square matrix of size 4:
```ocaml
[ A[0][0] | A[0][1] | A[0][2] | A[0][3] | A[1][0] | A[1][1] | A[1][2] | A[1][3] | A[2][0] | A[2][1] | A[2][2] | A[2][3] | A[3][0] | A[3][1] | A[3][2] | A[3][3] ]
```

Remember:
The cache loads adjacent addresses into the same cacheLine, e.g. a cache with `cacheLineSize` of `64` Bytes will load matrix elements from `A[0]` to `A[15]` (`16` elements) in one of its' `cacheLine` if `A[0]` is loaded from the memory during read miss (assuming `A[0]` is positioned at the start of the cacheLine).

## Matrix Multiplication Analysis

Given the matrix multiplication:
```ocaml
A[i][k] * B[k][j] = C[i][j]
```
We can have different access-combinations from this 3 variabls `i`, `k`, `j`. (see `src/assets/matrix_multiplication/matrix.h`)

Furthermore, each of the access-combinations can be "optimized" in 2 ways: introducing local variables, and using transposed matrix. (will be discussed later)

Running the combinations and their respective "optimized" version in C results in:

```text
| Method          | Time (s)   |  | Method          | Time (s)   |  | Method          | Time (s)   |
|-----------------|------------|  |-----------------|------------|  |-----------------|------------|
| ijk             | 4.606000   |  | ijk             | 4.581000   |  | ijk             | 4.494000   |
| ikj             | 3.326000   |  | ikj             | 3.319000   |  | ikj             | 3.304000   |
| jik             | 4.160000   |  | jik             | 4.169000   |  | jik             | 4.159000   |
| jki             | 10.893000  |  | jki             | 10.740000  |  | jki             | 10.447000  |
| kij             | 3.269000   |  | kij             | 3.266000   |  | kij             | 3.267000   |
| kji             | 10.713000  |  | kji             | 10.612000  |  | kji             | 10.362000  |
|-----------------|------------|  |-----------------|------------|  |-----------------|------------|
| ijk_opt1        | 2.920000   |  | ijk_opt1        | 2.900000   |  | ijk_opt1        | 2.914000   |
| ikj_opt1        | 2.533000   |  | ikj_opt1        | 2.536000   |  | ikj_opt1        | 2.527000   |
| jik_opt1        | 2.770000   |  | jik_opt1        | 2.769000   |  | jik_opt1        | 2.761000   |
| jki_opt1        | 10.652000  |  | jki_opt1        | 10.590000  |  | jki_opt1        | 10.377000  |
| kij_opt1        | 2.531000   |  | kij_opt1        | 2.546000   |  | kij_opt1        | 2.594000   |
| kji_opt1        | 10.578000  |  | kji_opt1        | 10.400000  |  | kji_opt1        | 10.353000  |
|-----------------|------------|  |-----------------|------------|  |-----------------|------------|
| ijk_opt2        | 2.310000   |  | ijk_opt2        | 2.309000   |  | ijk_opt2        | 2.320000   |
| jik_opt2        | 2.310000   |  | jik_opt2        | 2.319000   |  | jik_opt2        | 2.305000   |
| jki_opt2        | 2.534000   |  | jki_opt2        | 2.530000   |  | jki_opt2        | 2.528000   |
| kij_opt2        | 2.520000   |  | kij_opt2        | 2.517000   |  | kij_opt2        | 2.522000   |
| kji_opt2        | 2.516000   |  | kji_opt2        | 2.517000   |  | kji_opt2        | 2.497000   |
```

We can see that some patterns (such as `jki` and `kji`) is significantly slower than the other patterns.
We will now analyze it.

### Theoretical Analysis of Cache Performance
For this analysis, the following applies:
- `A`, `B`, `C` (`result`) matrix are `square_matrix` of size `n` and type `float`.
- The matrix multiplication would be: `A[i][k] * B[k][j] = C[i][j]`
- The `cacheLineSize` is equal to `m` Bytes. Where `m` is equal to `sizeof(float) * n`. In other words, one row of the matrix fits into a single cache line.
- It is impossible to load all complete matrices to the cache (otherwise we will get always cache hits).

Also, some definitions apply:
- Memory Access: Reading or writing to a memory location (theoretically to any kind of memory, but we consider only cache and main memory).
- Matrix Operation: A set of operation that reads elements from matrices `A` and `B`, performs a multiplication, then updates an element in `C` (e.g. inner loop in every standard form).

We will do analysis in two parts, the first one is to proof that **all optimized form is theoretically faster than the original one**.

1. Pattern `ijk` and `jik`

Standard Version
```C
void matrix_multiplication_ijk(const float* a, const float* b, float* result, int n) {
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            for (int k = 0; k < n; k++) {
                result[i * n + j] += a[i * n + k] * b[k * n + j];
            }
        }
    }
}
```

Total Memory Access:
- Memory accesses per inner loop (`k`-loop):
    > `a[i * n + k]`: 1 read <br>
    > `b[k * n + j]`: 1 read <br>
    > `result[i * n + j]`: 1 read + 1 write <br>
    > Total = 4 Memory accesses
- Memory accesses per middle loop (`j`-loop):
    > 4n (from `k` = 4 * n)
- Memory access per outer loop (`i`-loop):
    > 4n<sup>2</sup> (from `j` = 4n * n)
- Total memory access:
    > 4n<sup>3</sup> (from `i` = 4n<sup>2</sup> * n)

Optimized Version (local variable - Loop Tiling/ Blocking)
```C
void matrix_multiplication_ijk_opt1(const float* a, const float* b, float* result, int n) {
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

Total Memory Access:
- Memory accesses per inner loop (`k`-loop):
    > `a[i * n + k]`: 1 read <br>
    > `b[k * n + j]`: 1 read <br>
    > Total = 2 Memory accesses
- Memory accesses per middle loop (`j`-loop):
    > 2n (from `k` = 2 * n) <br>
    > `result[i * n + j]` = 1 write <br>
    > Total = 2n + 1 Memory accesses
- Memory access per outer loop (`i`-loop):
    > 2n<sup>2</sup> + n (from `j` = (2n + 1) * n)
- Total memory access:
    > 2n<sup>3</sup> + n<sup>2</sup> (from `i` = (2n<sup>2</sup> + n) * n)

It can be concluded that **4n<sup>3</sup>** > **2n<sup>3</sup> + n<sup>2</sup>** and thus the optimized version is theoretically faster. 

2. Pattern `ikj`, `jki`, `kij`, `kji`

Standard version
```C
void matrix_multiplication_ikj(const float* a, const float* b, float* result, int n) {
    for (int i = 0; i < n; i++) {
        for (int k = 0; k < n; k++) {
            for (int j = 0; j < n; j++) {
                result[i * n + j] += a[i * n + k] * b[k * n + j];
            }
        }
    }
}
```

Total Memory Access (same as `ijk`, `jik`):
- Memory accesses per inner loop (`k`-loop):
    > `a[i * n + k]`: 1 read <br>
    > `b[k * n + j]`: 1 read <br>
    > `result[i * n + j]`: 1 read + 1 write <br>
    > Total = 4 Memory accesses
- Memory accesses per middle loop (`j`-loop):
    > 4n (from `k` = 4 * n)
- Memory access per outer loop (`i`-loop):
    > 4n<sup>2</sup> (from `j` = 4n * n)
- Total memory access:
    > 4n<sup>3</sup> (from `i` = 4n<sup>2</sup> * n)

Optimized version
```C
void matrix_multiplication_ikj_opt1(const float* a, const float* b, float* result, int n) {
    for (int i = 0; i < n; i++) {
        for (int k = 0; k < n; k++) {
            float rowA = a[i * n + k];
            for (int j = 0; j < n; j++) {
                result[i * n + j] += rowA * b[k * n + j];
            }
        }
    }
}
```

Total Memory Access:
- Memory accesses per inner loop (`k`-loop):
    > `b[k * n + j]`: 1 read <br>
    > `result[i * n + j]`: 1 read + 1 write <br>
    > Total = 3 Memory accesses
- Memory accesses per middle loop (`j`-loop):
    > 3n (from `k` = 3 * n) <br>
    > `a[i * n + k]` = 1 read <br>
    > Total = 3n + 1 Memory accesses
- Memory access per outer loop (`i`-loop):
    > 3n<sup>2</sup> + n (from `j` = (3n + 1) * n)
- Total memory access:
    > 3n<sup>3</sup> + n<sup>2</sup> (from `i` = (3n<sup>2</sup> + n) * n)

Similar as before, **4n<sup>3</sup>** > **3n<sup>3</sup> + n<sup>2</sup>** and thus the optimized version is theoretically faster. 

**Note**: The difference for these patterns is that the local variable is used to reduce read operations to `A` or `B`, where in pattern `ijk` and `jik` the local variable is used to reduce read-write operations to `C`

In Conclusion:
| Method | Standard Memory Access | Optimized Memory Access |
|--------|------------------------|-------------------------|
| `ijk`  | $$4n^3$$               | $$2n^3 + n^2$$          |
| `jik`  | $$4n^3$$               | $$2n^3 + n^2$$          |
| `ikj`  | $$4n^3$$               | $$3n^3 + n^2$$          |
| `jki`  | $$4n^3$$               | $$3n^3 + n^2$$          |
| `kij`  | $$4n^3$$               | $$3n^3 + n^2$$          |
| `kji`  | $$4n^3$$               | $$3n^3 + n^2$$          |

For the second part, we will analyze the cache-behaviour. We will use the base form as it is easier to see the cache behavior.

Note:
- We consider the cache to be write back with write allocation (often implemented in real world) to simplify calculating main memory accesses for `C` (if we consider write through, we need to count each write operation to `C` as a main memory access, which only complicates the analysis)

1. Pattern `ijk`
```C
void matrix_multiplication_ijk(const float* a, const float* b, float* result, int n) {
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            for (int k = 0; k < n; k++) {
                result[i * n + j] += a[i * n + k] * b[k * n + j];
            }
        }
    }
}
```
Main Memory Access:
- `a[i * n + k]` is accessed row-wise, read miss only occurs for new matrix row (inner loop `k` and middle loop `j` has run n-times before that happens) = **1/n<sup>2</sup> Main Memory Access per matrix operation**
- `b[k * n + j]` is accessed column-wise, leading to always read miss = **1 Main Memory Access per matrix operation**
- `result[i * n + j]` is accessed row-wise, read miss occurs only for new row = **1/n<sup>2</sup> Main Memory Access per matrix operation** (inner loop `k` has run n-times, middle loop `j` has run n-times before fetching next row)

In total: $$1 + \frac{2}{n^2}$$ Main Memory Access per matrix operation

2. Pattern `ikj`
```C
void matrix_multiplication_ikj(const float* a, const float* b, float* result, int n) {
    for (int i = 0; i < n; i++) {
        for (int k = 0; k < n; k++) {
            for (int j = 0; j < n; j++) {
                result[i * n + j] += a[i * n + k] * b[k * n + j];
            }
        }
    }
}
```
Main Memory Access:
- `a[i * n + k]` has **1/n<sup>2</sup> Main Memory Access per matrix operation** (inner loop `j` has run n-times and middle loop `k` has also run n-times before the next row need to be fetched)
- `b[k * n + j]` has **1/n Main Memory Access per matrix operation** (inner loop `j` has run n-times)
- `result[i * n + j]` has **1/n<sup>2</sup> Main Memory Access per matrix operation** (for each read miss, both `k` and `j` must have run n-times)

In total: $$\frac{1}{n} + \frac{2}{n^2}$$ Main Memory Access per matrix operation

3. Pattern `jik`
```C
void matrix_multiplication_jik(const float* a, const float* b, float* result, int n) {
    for (int j = 0; j < n; j++) {
        for (int i = 0; i < n; i++) {
            for (int k = 0; k < n; k++) {
                result[i * n + j] += a[i * n + k] * b[k * n + j];
            }
        }
    }
}
```
Main Memory Access:
- `a[i * n + k]` has **1/n Main Memory Access per matrix operation**
- `b[k * n + j]` has **1 Main Memory Access per matrix operation**
- `result[i * n + j]` is accessed row-wise, read miss occurs only when `i` changes (inner loop `k` has run n-times) = **1/n Main Memory Access per matrix operation**

In total: $$ 1 + \frac{2}{n}$$ Main Memory Access per matrix operation

4. Pattern `jki`
```C
void matrix_multiplication_jki(const float* a, const float* b, float* result, int n) {
    for (int j = 0; j < n; j++) {
        for (int k = 0; k < n; k++) {
            for (int i = 0; i < n; i++) {
                result[i * n + j] += a[i * n + k] * b[k * n + j];
            }
        }
    }
}
```
Main Memory Access:
- `a[i * n + k]` has **1 Main Memory Access per matrix operation**
- `b[k * n + j]` has **1/n Main Memory Access per matrix operation** (inner loop `i` has run n-times)
- `result[i * n + j]` has **1 Main Memory Access per matrix operation**

In total: $$ 2 + \frac{1}{n}$$ Main Memory Access per matrix operation

5. Pattern `kij`
```C
void matrix_multiplication_kij(const float* a, const float* b, float* result, int n) {
    for (int k = 0; k < n; k++) {
        for (int i = 0; i < n; i++) {
            for (int j = 0; j < n; j++) {
                result[i * n + j] += a[i * n + k] * b[k * n + j];
            }
        }
    }
}
```
Main Memory Access:
- `a[i * n + k]` has **1/n Main Memory Access per matrix operation** (inner `j` has run n-times)
- `b[k * n + j]` has **1/n<sup>2</sup> Main Memory Access per matrix operation** (miss only when `k` changes, each `k` change means `i` and `j` has run n-times each)
- `result[i * n + j]` has **1/n Main Memory Access per matrix operation** (row-access)

In total: $$\frac{2}{n} + \frac{1}{n^2}$$ Main Memory Access per matrix operation

6. Pattern `kji`
```C
void matrix_multiplication_kji(const float* a, const float* b, float* result, int n) {
    for (int k = 0; k < n; k++) {
        for (int j = 0; j < n; j++) {
            for (int i = 0; i < n; i++) {
                result[i * n + j] += a[i * n + k] * b[k * n + j];
            }
        }
    }
}
```
Main Memory Access:
- `a[i * n + k]` has **1 Main Memory Access per matrix operation**
- `b[k * n + j]` has **1/n<sup>2</sup> Main Memory Access per matrix operation** (miss only when `k` changes, both `j` and `i` has run n-times each)
- `result[i * n + j]` has **1 Main Memory Access per matrix operation**

In total: $$2 + \frac{1}{n^2}$$ Main Memory Access per matrix operation

In Conclusion:
| Method |       Main Memory Access        |
|--------|---------------------------------|
| `ikj`  | $$\frac{1}{n} + \frac{2}{n^2}$$ |
| `kij`  | $$\frac{2}{n} + \frac{1}{n^2}$$ |
| `ijk`  | $$1 + \frac{2}{n^2}$$           |
| `jik`  | $$1 + \frac{2}{n}$$             |
| `kji`  | $$2 + \frac{1}{n^2}$$           |
| `jki`  | $$2 + \frac{1}{n}$$             |

From both parts, we can conclude:

1. Each Patten has different performance due to their cache behaviour. Optimizing them using local variables (e.g. `sum` which would be placed in a register - decided by compiler) can increase their overall performance. But note that which pattern is faster than which will remain relatively the same.

    > Note that the difference between several patterns is not stable (sometimes pattern A is faster than B, vice versa). This is due to several reasons such as scheduler, etc. (e.g. Another background task get priority and the processor decides to stop for a while and do something else - time sharing/ context switch).

    > Also note that the previous analysis assumes a cacheLine is fixed to a row, and that cacheLine will not be evicted (e.g. `n = 1000` which is used here, will likely cache evictions). Other factors such as cache-eviction-policy (normally LRU), etc. will also impact the real-world performance. Also, there are optimizations such as prefetching which reduces the penalty of cache misses.

```text
| Method          | Time (s)   |  | Method          | Time (s)   |  | Method          | Time (s)   |
|-----------------|------------|  |-----------------|------------|  |-----------------|------------|
| ijk             | 4.606000   |  | ijk             | 4.581000   |  | ijk             | 4.494000   |
| ikj             | 3.326000   |  | ikj             | 3.319000   |  | ikj             | 3.304000   |
| jik             | 4.160000   |  | jik             | 4.169000   |  | jik             | 4.159000   |
| jki             | 10.893000  |  | jki             | 10.740000  |  | jki             | 10.447000  |
| kij             | 3.269000   |  | kij             | 3.266000   |  | kij             | 3.267000   |
| kji             | 10.713000  |  | kji             | 10.612000  |  | kji             | 10.362000  |
|-----------------|------------|  |-----------------|------------|  |-----------------|------------|
| ijk_opt1        | 2.920000   |  | ijk_opt1        | 2.900000   |  | ijk_opt1        | 2.914000   |
| ikj_opt1        | 2.533000   |  | ikj_opt1        | 2.536000   |  | ikj_opt1        | 2.527000   |
| jik_opt1        | 2.770000   |  | jik_opt1        | 2.769000   |  | jik_opt1        | 2.761000   |
| jki_opt1        | 10.652000  |  | jki_opt1        | 10.590000  |  | jki_opt1        | 10.377000  |
| kij_opt1        | 2.531000   |  | kij_opt1        | 2.546000   |  | kij_opt1        | 2.594000   |
| kji_opt1        | 10.578000  |  | kji_opt1        | 10.400000  |  | kji_opt1        | 10.353000  |
```

2. For optimized form 2 of each pattern (it uses transposed matrix), we can see that `ijk` and `jik` has better performance due to them having lower Memory Access of $$2n^3 + n^2$$ compared to the others $$3n^3 + n^2$$ (Part 1 analysis). 

    > Note that the measured time didn't consider time needed to transpose the matrix. But it is relatively negligible, since a matrix of size `1000`, the average time needed to transpose is `0.004000` seconds.

    > Also note that `ikj_opt2` doesn't exist because there that pattern doesn't benefit from having any of its' matrix getting transposed. (compare with `ikj_opt1` instead)

```text
| Method          | Time (s)   |  | Method          | Time (s)   |  | Method          | Time (s)   |
|-----------------|------------|  |-----------------|------------|  |-----------------|------------|
| ijk_opt2        | 2.310000   |  | ijk_opt2        | 2.309000   |  | ijk_opt2        | 2.320000   |
| jik_opt2        | 2.310000   |  | jik_opt2        | 2.319000   |  | jik_opt2        | 2.305000   |
| ikj_opt1        | 2.533000   |  | ikj_opt1        | 2.536000   |  | ikj_opt1        | 2.527000   |
| jki_opt2        | 2.534000   |  | jki_opt2        | 2.530000   |  | jki_opt2        | 2.528000   |
| kij_opt2        | 2.520000   |  | kij_opt2        | 2.517000   |  | kij_opt2        | 2.522000   |
| kji_opt2        | 2.516000   |  | kji_opt2        | 2.517000   |  | kji_opt2        | 2.497000   |
```

### Analysis using Cache Simulator

We can analyze the behaviour of the algorithm using the simulator.

With `n = 16`, `cacheline-size = 16`, `cacheLines-L1 = 4`, `cacheLines-L2 = 16` <br>
Source: `examples/ijk/ijk.csv`
| Algorithm | Number of Cycles Simulated | Number of Cache Hits | Number of Cache Misses | Number of Gates |
|-----------|----------------------------|----------------------|------------------------|-----------------|
| Standard ijk | 1,196,432               | 10,700               | 5,684                  | 5,566           |
| ijk Optimized 1 | 594,512              | 3,580                | 4,868                  | 5,566           |
| ijk Optimized 2 | 228,288              | 6,864                | 1,584                  | 5,566           |

This proves that the previous analysis is correct.
- Optimized 1 introduces a local variable, reducing the number of write operations - reducing overall cycles.
- Optimized 2 enables better cache behaviour, with a negligible downside of having to transpose the matrix

Now, we will run the simulator with the Store buffer enabled.
- The simulator is implemented as write through and uses the No-Write-Allocate Policy. This means that every changes is written to the cache (if any) and to the memory. This results in a low performance because the Processor now needs to wait for the write (in main memory) to finish.
- The Buffer is therefore useful to reduce the overall cycle since the processor can now continue processing the next operation without waiting for the main memory to finish write changes (assume that the buffer is not full).

With the cache condition as: <br>
`n = 16`, `cacheline-size = 16`, `cacheLines-L1 = 4`, `cacheLines-L2 = 16`, `--storeback-buffer 4` <br>
| Algorithm | Number of Cycles Simulated | Number of Cache Hits | Number of Cache Misses | Number of Gates |
|-----------|----------------------------|----------------------|------------------------|-----------------|
| Standard ijk | 897,465                 | 10,700               | 5,684                  | 5,566           |
| ijk Optimized 1 | 576,997              | 3,580                | 4,868                  | 5,566           |
| ijk Optimized 2 | 205,985              | 6,864                | 1,584                  | 5,566           |

We will now use the other variant of the Store Buffer.
- **unconditional store buffer**: 
    > An unconditional store buffer holds write operations until they can be written to the main memory.
    
    When a read miss occurs (i.e., the requested data is not found in the cache), a read operation from the main memory is requested. Before said operation proceeds, the store buffer is flushed. <br> This ensures that any pending writes are committed to the memory, maintaining memory consistency. After flushing the store buffer, the read operation fetches the data from the main memory.

- **conditional store buffer**: 
    > A conditional store buffer holds write operations conditionally based on certain criteria, such as the presence of recently written data or synchronization events.

    When a read operation from the main memory is requested, the buffer is first checked for the requested data. <br>
    If the data still exist in the queue (i.e. same tag). Every data in the buffer will be flushed. <br>
    If no matching data is found in the buffer, the read operation proceeds directly from the main memory. The buffer is interrupted and the write must start from the beginning.<br>

    A more advanced implementation of the buffer does not use a simple FIFO Queue. Instead, it prioritizes flushing the relevant data first, reducing the likelihood of interruptions and improving efficiency.

With the cache condition as: <br>
`n = 16`, `cacheline-size = 16`, `cacheLines-L1 = 4`, `cacheLines-L2 = 16`, `--storeback-buffer 4`, `--storeback-condition true` <br>
| Algorithm | Number of Cycles Simulated | Number of Cache Hits | Number of Cache Misses | Number of Gates |
|-----------|----------------------------|----------------------|------------------------|-----------------|
| Standard ijk | 1,103,353               | 10,700               | 5,684                  | 5,566           |
| ijk Optimized 1 | 590,245              | 3,580                | 4,868                  | 5,566           |
| ijk Optimized 2 | 216,491              | 6,864                | 1,584                  | 5,566           |

The reason why the conditional store buffer has worse performance gain than unconditional store buffer is due to the added cycles from resetted write process which occurs when the buffer is interuppted. (e.g. Write to main memory needs 100 cycles, at 50th cycle we have read request which interrupts the Buffer, and thus it needs to write from the start again (lost 50 cycles))

# Correctness Analysis

Here, we describe the term correctness as **whether our cache simulator and matrix multiplication algorithm are implemented in a way that adheres to their expected behaviors and specifications**.

Some examples:
- If the cache management policy is direct associative, then the simulator is implemented as direct associative
- Matrix Multiplication Algorithm returns same correct result regardless of the sequence of the accesses.

## Correctness - Matrix Multiplication

We will discuss this starting from the Matrix Multiplication Algorithm.
By using `src/assets/matrix_multiplication/test_correctness_matrix.c`, we can safely conclude that the algorithm returns valid and correct result for all sequence of accesses.

Sample test with same Matrix `A` and `B` for each sequence yields this result:
With `n = 5` for both `A` and `B` (square matrix).
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

// truncated output
```

We can then compare it with the control result (using [online calculator](https://matrix.reshish.com/multCalculation.php)):
```
    11335	7710	9538	8131	16406
    10612	9724	13732	10864	21992
    7482	9500	12559	8759	17313
    14816	8968	13349	9517	23567
    13721	6504	4116	8485	19234
```

## Correctness - Cache Simulator

Based on the task provided, the cache simulator has been implemented with the following considerations:

1. Language and Version Compatibility <br>
    > The cache simulator has been developed using C17, C++14, and SystemC 2.3.3. It has been compiled with specific flags in the Makefile, ensuring it compiles to the correct versions of the languages.

2. Level of Abstraction <br>
    > The simulator has been designed with a sensible level of abstraction (CPU - L1 - L2 - Main Memory), focusing on the impact of multi-level caches on runtime and latency. This includes: 

    - Cache Levels: The simulator implements L1, L2, and Main Memory caches. The storage for each of them is abstracted as an array, not as “gates” (flip-flops), since we are focusing on cache behavior and runtime rather than the physical storage of data.
    - Cache Split: The L1 cache is not split into L1-I (Instruction) and L1-D (Data) caches. This is because the “processor” in this context is only executing the requests from a CSV files, and therefore does not benefit from having separate caches for instructions and data (it doesn't actually do calculations).
    - Exclusion of Certain Features: Features such as the Translation Lookaside Buffer (TLB) and others related to multi-app/multi-user systems is not implemented. This is because our processor is not running multiple programs or serving multiple users. It is solely reading a CSV file which contains requests from a single program (which multiplies matrices).

3. Structure of cache
    - Latency
        The simulator has default value of `4`, `12`, and `100` cycles for L1, L2, and Main Memory respectively. It also blocks user input that has invalid latency values, such as L1 latency > L2 latency, or L2 latency > Main Memory latency or 0 latency. 
    - Associativity
        The simulator is implemented as direct-mapped as the task specifies.
    - Cache Line count
        Default value is `64` for L1 and `256` for L2 (typical real world implementation). The simulator blocks user input that has invalid cache line count, such as L1 cache line count > L2 cache line count or 0 cache line count.
    - Cache Line Size
        Default value is `64` bytes (typical real world implementation). The simulator blocks user input that has invalid cache line size, such as 0 cache line size.

4. Cache management policies
    > The simulator is implemented as Direct-mapped, and thus has no cache-eviction-policies. However, the simulator has write-through without write-allocate policy. This means that every write operation is written to the cache (if any) and main memory.

5. Cache behavior (e.g. hits, misses)
    > The simulator correctly calculates the number of cache hits and misses for each cache level and tallied them to a struct called `CacheStats`. It also correctly calculates the number of cycles needed to complete the matrix multiplication operation. The simulator also correctly calculates the number of gates needed for the cache architecture specified by the user input/ default value.

6. Implementation of the Framework program
    > The framework program is implemented in C. It accepts and processes command line arguments in a manner similar to Linux command line programs. It has sensible default values for options (see `src/main/parser/terminal_parser.c`), and correctly catches incorrect or nonsensical options, outputting them to `stderr` with meaningful error messages and explanations.

# Accuracy Analysis

The term **accuracy**, in the context of our cache simulator, is defined as **how closely the simulator emulates the actual behavior and performance of a cache in a real system**. 

There are several limitations to the accuracy of the simulator, which include but not limited to:

1. **Relation between Cache Size and Speed**
    - In real-world systems, the larger the cache, the slower it tends to be. This is due to the time required by the processor to check which cache line the data is in, compare the tags, etc. However, in the simulator, even if one were to set `cacheLineSize` to `0xFFFF.FFFF` (the maximum value of an `unsigned int`), the latency of the cache remains constant (specified through the input or default value of `4` for L1). This also applies to cache lines count, such as `l1CacheLines`.

2. **Latency for Loading Cache**
    - In real-world systems, loading data into the cache takes time. However, in the simulator, loading data into the cache, both L1 and L2, takes no cycles. In other words, the typical calculation of cache penalty, which would normally account for this latency, does not apply in the simulator.

3. **Performance of the Algorithm**
    - The simulator does not take into account the scheduler that decides the size of resources a process is given. In our simulation, for the same input - the output (e.g., total cycle count) will always be the same. In other words, the simulator assumes that a program always get 100% resources needed, resulting in a deviation from real-world performance.

4. **Implementation of Optimization**
    - The simulator has prefetching implemented. But the said prefetcher is a very simple one, which only prefetches the next cache line. In real-world systems, prefetchers are much more complex and can predict which cache lines will be needed next (e.g. stride pattern). This can lead to a significant difference in performance between the simulator and a real system.

5. **Simplification of Memory Hierarchy**
    - The simulator simplifies the memory hierarchy by only implementing L1, L2, and Main Memory caches. In a real system, there may be additional levels of cache, as well as other memory components such as registers and virtual memory. The absence of these components in the simulator can affect the accuracy of the result of the simulation (e.g. total cycles count).
