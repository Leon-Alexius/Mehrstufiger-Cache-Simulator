#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define RAND_MAX_VAL 100

/**
 * @brief Initialize Square Matrix of size `n` with random values
 * 
 * @note
 * Represent matrix A[x][y] as A[x * width + y].
 * We are doing this because cache normally load adjacent address to a cache line
 * 
 * @details
 * Example Matrix A[3][3] as A[9]
 * [ 0,0 | 0,1 | 0,2 | 1,0 | 1,1 | 1,2 | 2,0 | 2,1 | 2,2 ]
 * 
 * If we have cache size x-KB, with each cache line of 64 Bytes (cache line count = x * 1000 / 64)
 * then we can save 64 / 4 = 16 data in one cache line
 * (Assume metadatas such as Tags, Dirty Bit, Valid Bit is very small - can be counted out here)
 * 
 * That means, loading A[0][0] would leads to A[0][1], ..., A[0][15] loaded to that cache line too
 * Therefore, when doing matrix operation by row, chance of cache hit would be bigger
 * 
 * Other notable thing is that by doing row access, we have very low stride.
 * Meaning we don't skip many elements during iteration - higher cache hit.
 * Stride = fixed distance (ex. stride of 4 = acc every (multiple of 4)th - element)
 * 
 * Which is also has relation to Spatial Locality:
 * If a memory location is accessed, nearby memory locations are likely to be accessed soon.
 * 
 * @warning
 * I assume our cache implementation has:
 * 1. Load Blocks (address adjacent is also loaded to that cache Line)
 *  
 * @author Lie Leon Alexius
 */
void init_matrix(float* matrix, int n) {
    for (int i = 0; i < n * n; i++) {
        matrix[i] = (float) (rand() % RAND_MAX_VAL);
    }
}

/**
 * @brief A[i][k] * B[k][j] = C[i][j]
 * 
 * @note
 * This function multiplies matrices in the order of i, j, k
 * 
 * This function should have good cache performance because it accesses the elements of the matrices 
 * in a row-major order, which is how they’re stored in memory in C/C++.
 * 
 * @details
 * 1. The sum C[] is written back until the most inner loop k is finished. 
 * Thus, no cache access (read) for C[], we only do (write) for that C[]. 
 * 
 * 2. The Matrix B is accessed by Collumn (vertical) - high cache Miss -> high reload
 * Then, cost of cache access (read) for B[] is 1 (each call = load once)
 * 
 * 3. Accesses to Matrix A here is efficient, due to row-based access
 * Suppose each cache Line = 16 data, then once every 16 reads, we just need to load once = 1/16 cost
 * 
 * Conclusion - expected number of cache misses per access:
 * 1. C = 0 (no read)
 * 2. B = 1 (each access to B is a miss)
 * 3. A = 1/16 (every 16th access is a miss)
 * 
 * So, memory access per instruction (per cell) = 0 + 1 + 1/16 = count for yourself
 * For each cell operation in the matrix multiplication, expect to have >= 1 memory access on average due to cache misses.
 * 
 * @author Lie Leon Alexius
 */
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

/**
 * @brief A[i][k] * B[k][j] = C[i][j]
 * 
 * @details
 * Here we only modify calling j, i, k. This should be slower than i, j, k due to:
 * 
 * 1. Matrix C - different i being called for each j (after each k-loop need to load new i)
 * Where before, new i would be loaded only when all j has finished for that i (lower cache miss -> reload)
 * The difference won't be a big problem if C is small.
 * 
 * The calculation for memory access for each cell operation still the same. (see for yourself)
 * But the total time is bigger.
 * 
 * @author Lie Leon Alexius
 */
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

/**
 * @brief A[i][k] * B[k][j] = C[i][j]
 * 
 * @details
 * Here we have i, k, j pattern.
 * This is the most effective.
 * 
 * 1. A here doesn't have any misses ("most" inner loop access is hit) = 0
 * 2. B is accessed by row (each 16th access j need to load new) = 1/16
 * 3. C is also the same as B = 1/16
 * 
 * Memory acc per cell op = 1/8 = 0.125
 * More effective than ijk and jik
 * 
 * @author Lie Leon Alexius
 */
void matrix_multiplication_ikj(float* a, float* b, float* result, int n) {
    for (int i = 0; i < n; i++) {
        for (int k = 0; k < n; k++) {
            for (int j = 0; j < n; j++) {
                result[i * n + j] += a[i * n + k] * b[k * n + j];
            }
        }
    }
}

/**
 * @brief A[i][k] * B[k][j] = C[i][j]
 * 
 * @brief
 * Access by k, i, j is also efficient (+- the same as i, k, j).
 * Memory acc per cell op = 0.125 
 * 
 * Reason: each row j is loaded (B, C is accessed by row),
 * although different i is called for each k, the difference is not that big
 * 
 * @author Lie Leon Alexius
 */
void matrix_multiplication_kij(float* a, float* b, float* result, int n) {
    for (int k = 0; k < n; k++) {
        for (int i = 0; i < n; i++) {
            for (int j = 0; j < n; j++) {
                result[i * n + j] += a[i * n + k] * b[k * n + j];
            }
        }
    }
}

/**
 * @brief A[i][k] * B[k][j] = C[i][j]
 * 
 * @details
 * Access by j, k, i
 * It can be seen directly that this is THE WORST ONE
 * 
 * 1. Cost of each call A = 1 (always miss - A[i][k])
 * 2. Cost of each call B = 0
 * 3. Cost of each call C = 1 (always miss - C[i][j])
 * 
 * In total: Memory acc per cell op = 2
 * This is even worse than k, j, i due to different k must also be loaded for B for each j (see reason jik worse than ijk)
 * 
 * @author Lie Leon Alexius
 */
void matrix_multiplication_jki(float* a, float* b, float* result, int n) {
    for (int j = 0; j < n; j++) {
        for (int k = 0; k < n; k++) {
            for (int i = 0; i < n; i++) {
                result[i * n + j] += a[i * n + k] * b[k * n + j];
            }
        }
    }
}

/**
 * @brief A[i][k] * B[k][j] = C[i][j]
 * 
 * @details
 * Access k, j, i
 * 1. A addressed as column (different i must be loaded for each k, still cache miss tho)
 * 2. B is actually row-addressed (the only redeeming thing)
 * 3. C addressed as column (same as a)
 * 
 * Memory acc per cell op = 2
 * 
 * @author Lie Leon Alexius
 */
void matrix_multiplication_kji(float* a, float* b, float* result, int n) {
    for (int k = 0; k < n; k++) {
        for (int j = 0; j < n; j++) {
            for (int i = 0; i < n; i++) {
                result[i * n + j] += a[i * n + k] * b[k * n + j];
            }
        }
    }
}

/**
 * @brief Resets all the values in a matrix to zero.
 * 
 * @param matrix Pointer to the matrix.
 * @param n The number of rows/columns in the matrix.
 * 
 * @author Lie Leon Alexius
 */
void reset_matrix(float* matrix, int n) {
    for (int i = 0; i < n * n; i++) {
        matrix[i] = 0.0f;
    }
}

/**
 * @brief Free a matrix
 * @warning assumes that the matrix is a 1D-Array
 * @author Lie Leon Alexius
 */
void free_matrix(float* matrix) {
    free(matrix);
}

/**
 * @brief Do test on various cache behavior
 * @param n The size of the matrix (n x n)
 * @author Lie Leon Alexius
 */
void test_matrix_multiplication() {
    // Assume a cache Line holds 16 values
    int n = 16;

    // Allocate Memory
    float* a = (float*) malloc(n * n * sizeof(float));
    float* b = (float*) malloc(n * n * sizeof(float));
    float* result = (float*) calloc(n * n, sizeof(float)); // Initialize to 0

    // For Analyzing
    clock_t start, end;
    double cpu_time_used;

    // Fill the array
    init_matrix(a, n);
    init_matrix(b, n);

    // Analyze here
    printf("| %-10s | %-10s |\n", "Method", "Time (s)");
    printf("|------------|------------|\n");

    char* methods[] = {"ijk", "jik", "ikj", "jki", "kij", "kji"};
    void (*funcs[]) (float*, float*, float*, int) = {
        matrix_multiplication_ijk, 
        matrix_multiplication_jik, 
        matrix_multiplication_ikj, 
        matrix_multiplication_jki, 
        matrix_multiplication_kij, 
        matrix_multiplication_kji
    };

    for (int m = 0; m < 6; m++) {
        // Reset result matrix
        reset_matrix(result, n);

        start = clock();
        funcs[m](a, b, result, n); // Invoke the function pointer
        end = clock();

        cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;
        printf("| %-10s | %-10f |\n", methods[m], cpu_time_used);
    }

    // Print result matrix as a table
    printf("\nResult Matrix:\n");
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            printf("%10.2f ", result[i * n + j]);
        }
        printf("\n");
    }

    // Free allocated memory
    free_matrix(a);
    free_matrix(b);
    free_matrix(result);
}
