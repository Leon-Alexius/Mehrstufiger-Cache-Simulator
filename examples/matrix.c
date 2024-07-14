// Lie Leon Alexius
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define RAND_MAX_VAL 100

/**
 * @brief Initialize Square Matrix of size `n` with random values
 * @note Represent matrix element A[x][y] as A[x * width + y].
 * @author Lie Leon Alexius
 */
void init_matrix(float* matrix, int n) {
    for (int i = 0; i < n * n; i++) {
        matrix[i] = (float) (rand() % RAND_MAX_VAL);
    }
}

/**
 * @brief A[i][k] * B[k][j] = C[i][j]
 * @note This function multiplies matrices in the order of i, j, k
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
 * @note This function builds the .csv file for the matrix multiplication of i, j, k
 * @author Lie Leon Alexius
 */
void build_csv_ijk(float* a, float* b, float* result, int n) {
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            float sum = 0.0f;
            for (int k = 0; k < n; k++) {
                printf("R, 0x%p, \n", &a[i * n + k]);
                printf("R, 0x%p, \n", &b[k * n + j]);
                sum += a[i * n + k] * b[k * n + j];
            }
            printf("W, 0x%p, %u \n", &result[i * n + j], (unsigned int) sum);
            result[i * n + j] = sum;
        }
    }
}

/**
 * @brief A[i][k] * B[k][j] = C[i][j]
 * @note This function multiplies matrices in the order of j, i, k
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
 * @note This function builds the .csv file for the matrix multiplication of j, i, k
 * @author Lie Leon Alexius
 */
void build_csv_jik(float* a, float* b, float* result, int n) {
    for (int j = 0; j < n; j++) {
        for (int i = 0; i < n; i++) {
            float sum = 0.0f;
            for (int k = 0; k < n; k++) {
                printf("R, 0x%p, \n", &a[i * n + k]);
                printf("R, 0x%p, \n", &b[k * n + j]);
                sum += a[i * n + k] * b[k * n + j];
            }
            printf("W, 0x%p, %u \n", &result[i * n + j], (unsigned int) sum);
            result[i * n + j] = sum;
        }
    }
}

/**
 * @brief A[i][k] * B[k][j] = C[i][j]
 * @note This function multiplies matrices in the order of i, k, j
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
 * @note This function builds the .csv file for the matrix multiplication of i, k, j
 * @author Lie Leon Alexius
 */
void build_csv_ikj(float* a, float* b, float* result, int n) {
    for (int i = 0; i < n; i++) {
        for (int k = 0; k < n; k++) {
            for (int j = 0; j < n; j++) {
                printf("R, 0x%p, \n", &a[i * n + k]);
                printf("R, 0x%p, \n", &b[k * n + j]);
                float sum = a[i * n + k] * b[k * n + j];

                printf("R, 0x%p, \n", &result[i * n + j]);
                float sum2 = result[i * n + j] + sum;

                printf("W, 0x%p, %u \n", &result[i * n + j], (unsigned int) sum2);
                result[i * n + j] = sum2;
            }
        }
    }
}

/**
 * @brief A[i][k] * B[k][j] = C[i][j]
 * @note This function multiplies matrices in the order of k, i, j
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
 * @note This function builds the .csv file for the matrix multiplication of k, i, j
 * @author Lie Leon Alexius
 */
void build_csv_kij(float* a, float* b, float* result, int n) {
    for (int k = 0; k < n; k++) {
        for (int i = 0; i < n; i++) {
            for (int j = 0; j < n; j++) {
                printf("R, 0x%p, \n", &a[i * n + k]);
                printf("R, 0x%p, \n", &b[k * n + j]);
                float sum = a[i * n + k] * b[k * n + j];

                printf("R, 0x%p, \n", &result[i * n + j]);
                float sum2 = result[i * n + j] + sum;

                printf("W, 0x%p, %u \n", &result[i * n + j], (unsigned int) sum2);
                result[i * n + j] = sum2;
            }
        }
    }
}

/**
 * @brief A[i][k] * B[k][j] = C[i][j]
 * @note This function multiplies matrices in the order of j, k, i
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
 * @note This function builds the .csv file for the matrix multiplication of j, k, i
 * @author Lie Leon Alexius
 */
void build_csv_jki(float* a, float* b, float* result, int n) {
    for (int j = 0; j < n; j++) {
        for (int k = 0; k < n; k++) {
            for (int i = 0; i < n; i++) {
                printf("R, 0x%p, \n", &a[i * n + k]);
                printf("R, 0x%p, \n", &b[k * n + j]);
                float sum = a[i * n + k] * b[k * n + j];

                printf("R, 0x%p, \n", &result[i * n + j]);
                float sum2 = result[i * n + j] + sum;

                printf("W, 0x%p, %u \n", &result[i * n + j], (unsigned int) sum2);
                result[i * n + j] = sum2;
            }
        }
    }
}

/**
 * @brief A[i][k] * B[k][j] = C[i][j]
 * @note This function multiplies matrices in the order of k, j, i
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
 * @brief A[i][k] * B[k][j] = C[i][j]
 * @note This function builds the .csv file for the matrix multiplication of k, j, i
 * @author Lie Leon Alexius
 */
void build_csv_kji(float* a, float* b, float* result, int n) {
    for (int k = 0; k < n; k++) {
        for (int j = 0; j < n; j++) {
            for (int i = 0; i < n; i++) {
                printf("R, 0x%p, \n", &a[i * n + k]);
                printf("R, 0x%p, \n", &b[k * n + j]);
                float sum = a[i * n + k] * b[k * n + j];

                printf("R, 0x%p, \n", &result[i * n + j]);
                float sum2 = result[i * n + j] + sum;

                printf("W, 0x%p, %u \n", &result[i * n + j], (unsigned int) sum2);
                result[i * n + j] = sum2;
            }
        }
    }
}

/**
 * @brief Resets all the values in a matrix to zero.
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
 * @brief Do run time analysis on different matrix multiplication accesses
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

    // Free allocated memory
    free_matrix(a);
    free_matrix(b);
    free_matrix(result);
}
