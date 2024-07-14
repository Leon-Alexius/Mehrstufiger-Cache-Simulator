// Lie Leon Alexius
// This is a modified matrix.h to create .csv requests

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <stdint.h>

#define RAND_MAX_VAL 100

// ============================================ Helper Functions ============================================

/**
 * @brief Initialize Square Matrix of size `n` with random values
 * @author Lie Leon Alexius
 */
void init_matrix(float* matrix, int n) {
    for (int i = 0; i < n * n; i++) {
        matrix[i] = (float) (rand() % RAND_MAX_VAL);
    }
}

/**
 * @brief Transpose a square matrix of size `n`
 * @author Lie Leon Alexius
 */
void transpose_matrix(float* matrix, float* transposed_matrix, int n) {
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            transposed_matrix[j * n + i] = matrix[i * n + j];
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
 * @warning Don't delete, looks cool
 * @author Lie Leon Alexius
 */
void free_matrix(float* matrix) {
    free(matrix);
}


/**
 * @brief print the matrix
 * @author Lie Leon Alexius
 */
void print_matrix(float* matrix, int n) {
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            printf("%10.2f ", matrix[i * n + j]);
        }
        printf("\n");
    }
}

int compare_matrices(float* result, float* control, int n) {
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            if (fabs(result[i * n + j] - control[i * n + j]) > 1e-6) {
                printf("Error: matrices are not equal at position (%d, %d)\n", i, j);
                return 1;
            }
        }
    }
    return 0;
}

// ====================================== Standard Matrix Multiplication ======================================

/*
    Combinations:
    ijk
    ikj
    jik
    jki
    kij
    kji
*/

/**
 * @brief A[i][k] * B[k][j] = C[i][j]
 * @author Lie Leon Alexius
 */
void matrix_multiplication_ijk(float* a, float* b, float* result, int n) {
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            for (int k = 0; k < n; k++) {
                // result[i * n + j] += a[i * n + k] * b[k * n + j];

                printf("R, 0x%08x, \n", (uint32_t)((uintptr_t)&a[i * n + k] & 0xFFFFFFFF));
                printf("R, 0x%08x, \n", (uint32_t)((uintptr_t)&b[k * n + j] & 0xFFFFFFFF));
                float sum = a[i * n + k] * b[k * n + j];

                printf("R, 0x%08x, \n", (uint32_t)((uintptr_t)&result[i * n + j] & 0xFFFFFFFF));
                float sum2 = result[i * n + j] + sum;

                printf("W, 0x%08x, %u \n", (uint32_t)((uintptr_t)&result[i * n + j] & 0xFFFFFFFF), (unsigned int) sum2);
                result[i * n + j] = sum2;
            }
        }
    }
}

/**
 * @brief A[i][k] * B[k][j] = C[i][j]
 * @author Lie Leon Alexius
 */
void matrix_multiplication_ikj(float* a, float* b, float* result, int n) {
    for (int i = 0; i < n; i++) {
        for (int k = 0; k < n; k++) {
            for (int j = 0; j < n; j++) {
                // result[i * n + j] += a[i * n + k] * b[k * n + j];

                printf("R, 0x%08x, \n", (uint32_t)((uintptr_t)&a[i * n + k] & 0xFFFFFFFF));
                printf("R, 0x%08x, \n", (uint32_t)((uintptr_t)&b[k * n + j] & 0xFFFFFFFF));
                float sum = a[i * n + k] * b[k * n + j];

                printf("R, 0x%08x, \n", (uint32_t)((uintptr_t)&result[i * n + j] & 0xFFFFFFFF));
                float sum2 = result[i * n + j] + sum;

                printf("W, 0x%08x, %u \n", (uint32_t)((uintptr_t)&result[i * n + j] & 0xFFFFFFFF), (unsigned int) sum2);
                result[i * n + j] = sum2;
            }
        }
    }
}

/**
 * @brief A[i][k] * B[k][j] = C[i][j]
 * @author Lie Leon Alexius
 */
void matrix_multiplication_jik(float* a, float* b, float* result, int n) {
    for (int j = 0; j < n; j++) {
        for (int i = 0; i < n; i++) {
            for (int k = 0; k < n; k++) {
                // result[i * n + j] += a[i * n + k] * b[k * n + j];

                printf("R, 0x%08x, \n", (uint32_t)((uintptr_t)&a[i * n + k] & 0xFFFFFFFF));
                printf("R, 0x%08x, \n", (uint32_t)((uintptr_t)&b[k * n + j] & 0xFFFFFFFF));
                float sum = a[i * n + k] * b[k * n + j];

                printf("R, 0x%08x, \n", (uint32_t)((uintptr_t)&result[i * n + j] & 0xFFFFFFFF));
                float sum2 = result[i * n + j] + sum;

                printf("W, 0x%08x, %u \n", (uint32_t)((uintptr_t)&result[i * n + j] & 0xFFFFFFFF), (unsigned int) sum2);
                result[i * n + j] = sum2;
            }
        }
    }
}

/**
 * @brief A[i][k] * B[k][j] = C[i][j]
 * @author Lie Leon Alexius
 */
void matrix_multiplication_jki(float* a, float* b, float* result, int n) {
    for (int j = 0; j < n; j++) {
        for (int k = 0; k < n; k++) {
            for (int i = 0; i < n; i++) {
                // result[i * n + j] += a[i * n + k] * b[k * n + j];

                printf("R, 0x%08x, \n", (uint32_t)((uintptr_t)&a[i * n + k] & 0xFFFFFFFF));
                printf("R, 0x%08x, \n", (uint32_t)((uintptr_t)&b[k * n + j] & 0xFFFFFFFF));
                float sum = a[i * n + k] * b[k * n + j];

                printf("R, 0x%08x, \n", (uint32_t)((uintptr_t)&result[i * n + j] & 0xFFFFFFFF));
                float sum2 = result[i * n + j] + sum;

                printf("W, 0x%08x, %u \n", (uint32_t)((uintptr_t)&result[i * n + j] & 0xFFFFFFFF), (unsigned int) sum2);
                result[i * n + j] = sum2;
            }
        }
    }
}

/**
 * @brief A[i][k] * B[k][j] = C[i][j]
 * @author Lie Leon Alexius
 */
void matrix_multiplication_kij(float* a, float* b, float* result, int n) {
    for (int k = 0; k < n; k++) {
        for (int i = 0; i < n; i++) {
            for (int j = 0; j < n; j++) {
                // result[i * n + j] += a[i * n + k] * b[k * n + j];

                printf("R, 0x%08x, \n", (uint32_t)((uintptr_t)&a[i * n + k] & 0xFFFFFFFF));
                printf("R, 0x%08x, \n", (uint32_t)((uintptr_t)&b[k * n + j] & 0xFFFFFFFF));
                float sum = a[i * n + k] * b[k * n + j];

                printf("R, 0x%08x, \n", (uint32_t)((uintptr_t)&result[i * n + j] & 0xFFFFFFFF));
                float sum2 = result[i * n + j] + sum;

                printf("W, 0x%08x, %u \n", (uint32_t)((uintptr_t)&result[i * n + j] & 0xFFFFFFFF), (unsigned int) sum2);
                result[i * n + j] = sum2;
            }
        }
    }
}

/**
 * @brief A[i][k] * B[k][j] = C[i][j]
 * @author Lie Leon Alexius
 */
void matrix_multiplication_kji(float* a, float* b, float* result, int n) {
    for (int k = 0; k < n; k++) {
        for (int j = 0; j < n; j++) {
            for (int i = 0; i < n; i++) {
                // result[i * n + j] += a[i * n + k] * b[k * n + j];

                printf("R, 0x%08x, \n", (uint32_t)((uintptr_t)&a[i * n + k] & 0xFFFFFFFF));
                printf("R, 0x%08x, \n", (uint32_t)((uintptr_t)&b[k * n + j] & 0xFFFFFFFF));
                float sum = a[i * n + k] * b[k * n + j];

                printf("R, 0x%08x, \n", (uint32_t)((uintptr_t)&result[i * n + j] & 0xFFFFFFFF));
                float sum2 = result[i * n + j] + sum;

                printf("W, 0x%08x, %u \n", (uint32_t)((uintptr_t)&result[i * n + j] & 0xFFFFFFFF), (unsigned int) sum2);
                result[i * n + j] = sum2;
            }
        }
    }
}

// ===================================== Optimized 1 Matrix Multiplication =====================================

/*
    With inner variables to reduce the number of memory accesses

    Combinations:
    ijk
    ikj
    jik
    jki
    kij
    kji
*/

/**
 * @brief A[i][k] * B[k][j] = C[i][j]
 * @author Lie Leon Alexius
 */
void matrix_multiplication_ijk_opt1(float* a, float* b, float* result, int n) {
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            float sum = 0.0f;
            for (int k = 0; k < n; k++) {
                printf("R, 0x%08x, \n", (uint32_t)((uintptr_t)&a[i * n + k] & 0xFFFFFFFF));
                printf("R, 0x%08x, \n", (uint32_t)((uintptr_t)&b[k * n + j] & 0xFFFFFFFF));
                sum += a[i * n + k] * b[k * n + j];
            }
            printf("W, 0x%08x, %u \n", (uint32_t)((uintptr_t)&result[i * n + j] & 0xFFFFFFFF), (unsigned int) sum);
            result[i * n + j] = sum;
        }
    }
}

/**
 * @brief A[i][k] * B[k][j] = C[i][j]
 * @author Lie Leon Alexius
 */
void matrix_multiplication_ikj_opt1(float* a, float* b, float* result, int n) {
    for (int i = 0; i < n; i++) {
        for (int k = 0; k < n; k++) {
            printf("R, 0x%08x, \n", (uint32_t)((uintptr_t)&a[i * n + k] & 0xFFFFFFFF));
            float rowA = a[i * n + k];
            for (int j = 0; j < n; j++) {
                printf("R, 0x%08x, \n", (uint32_t)((uintptr_t)&b[k * n + j] & 0xFFFFFFFF));
                printf("R, 0x%08x, \n", (uint32_t)((uintptr_t)&result[i * n + j] & 0xFFFFFFFF));
                float sum2 = result[i * n + j] + rowA * b[k * n + j];

                printf("W, 0x%08x, %u \n", (uint32_t)((uintptr_t)&result[i * n + j] & 0xFFFFFFFF), (unsigned int) sum2);
                result[i * n + j] = sum2;
            }
        }
    }
}

/**
 * @brief A[i][k] * B[k][j] = C[i][j]
 * @author Lie Leon Alexius
 */
void matrix_multiplication_jik_opt1(float* a, float* b, float* result, int n) {
    for (int j = 0; j < n; j++) {
        for (int i = 0; i < n; i++) {
            float sum = 0.0f;
            for (int k = 0; k < n; k++) {
                printf("R, 0x%08x, \n", (uint32_t)((uintptr_t)&a[i * n + k] & 0xFFFFFFFF));
                printf("R, 0x%08x, \n", (uint32_t)((uintptr_t)&b[k * n + j] & 0xFFFFFFFF));
                sum += a[i * n + k] * b[k * n + j];
            }
            printf("W, 0x%08x, %u \n", (uint32_t)((uintptr_t)&result[i * n + j] & 0xFFFFFFFF), (unsigned int) sum);
            result[i * n + j] = sum;
        }
    }
}

/**
 * @brief A[i][k] * B[k][j] = C[i][j]
 * @author Lie Leon Alexius
 */
void matrix_multiplication_jki_opt1(float* a, float* b, float* result, int n) {
    for (int j = 0; j < n; j++) {
        for (int k = 0; k < n; k++) {
            printf("R, 0x%08x, \n", (uint32_t)((uintptr_t)&b[k * n + j] & 0xFFFFFFFF));
            float colB = b[k * n + j];
            for (int i = 0; i < n; i++) {
                printf("R, 0x%08x, \n", (uint32_t)((uintptr_t)&a[i * n + k] & 0xFFFFFFFF));
                printf("R, 0x%08x, \n", (uint32_t)((uintptr_t)&result[i * n + j] & 0xFFFFFFFF));
                float sum2 = result[i * n + j] + a[i * n + k] * colB;

                printf("W, 0x%08x, %u \n", (uint32_t)((uintptr_t)&result[i * n + j] & 0xFFFFFFFF), (unsigned int) sum2);
                result[i * n + j] = sum2;
            }
        }
    }
}

/**
 * @brief A[i][k] * B[k][j] = C[i][j]
 * @author Lie Leon Alexius
 */
void matrix_multiplication_kij_opt1(float* a, float* b, float* result, int n) {
    for (int k = 0; k < n; k++) {
        for (int i = 0; i < n; i++) {
            printf("R, 0x%08x, \n", (uint32_t)((uintptr_t)&a[i * n + k] & 0xFFFFFFFF));
            float colA = a[i * n + k];
            for (int j = 0; j < n; j++) {
                printf("R, 0x%08x, \n", (uint32_t)((uintptr_t)&b[k * n + j] & 0xFFFFFFFF));
                printf("R, 0x%08x, \n", (uint32_t)((uintptr_t)&result[i * n + j] & 0xFFFFFFFF));
                float sum2 = result[i * n + j] + colA * b[k * n + j];

                printf("W, 0x%08x, %u \n", (uint32_t)((uintptr_t)&result[i * n + j] & 0xFFFFFFFF), (unsigned int) sum2);
                result[i * n + j] = sum2;
            }
        }
    }
}

/**
 * @brief A[i][k] * B[k][j] = C[i][j]
 * @author Lie Leon Alexius
 */
void matrix_multiplication_kji_opt1(float* a, float* b, float* result, int n) {
    for (int k = 0; k < n; k++) {
        for (int j = 0; j < n; j++) {
            printf("R, 0x%08x, \n", (uint32_t)((uintptr_t)&b[k * n + j] & 0xFFFFFFFF));
            float rowB = b[k * n + j];
            for (int i = 0; i < n; i++) {
                printf("R, 0x%08x, \n", (uint32_t)((uintptr_t)&a[i * n + k] & 0xFFFFFFFF));
                printf("R, 0x%08x, \n", (uint32_t)((uintptr_t)&result[i * n + j] & 0xFFFFFFFF));
                float sum2 = result[i * n + j] + a[i * n + k] * rowB;

                printf("W, 0x%08x, %u \n", (uint32_t)((uintptr_t)&result[i * n + j] & 0xFFFFFFFF), (unsigned int) sum2);
                result[i * n + j] = sum2;
            }
        }
    }
}

// ===================================== Optimized 2 Matrix Multiplication =====================================

/*
    We transpose either A or B for better Stride + use inner variable to reduce the number of memory accesses

    Combinations:
    ijk
    ikj - not applicable since everything is already accessed by row-wise
    jik
    jki - is basically ikj for matrix that is saved collumn-wise
    kij
    kji
*/

/**
 * @brief A[i][k] * B_T[j][k] = C[i][j]
 * @note B is transposed
 * @author Lie Leon Alexius
 */
void matrix_multiplication_ijk_opt2(float* a, float* b, float* result, int n) {
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            float sum = 0.0f;
            for (int k = 0; k < n; k++) {
                printf("R, 0x%08x, \n", (uint32_t)((uintptr_t)&a[i * n + k] & 0xFFFFFFFF));
                printf("R, 0x%08x, \n", (uint32_t)((uintptr_t)&b[j * n + k] & 0xFFFFFFFF));
                sum += a[i * n + k] * b[j * n + k];
            }
            printf("W, 0x%08x, %u \n", (uint32_t)((uintptr_t)&result[i * n + j] & 0xFFFFFFFF), (unsigned int) sum);
            result[i * n + j] = sum;
        }
    }
}

/**
 * @brief A[i][k] * B[k][j] = C[i][j]
 * @note B and C is transposed
 * @author Lie Leon Alexius
 */
void matrix_multiplication_jik_opt2(float* a, float* b, float* result, int n) {
    for (int j = 0; j < n; j++) {
        for (int i = 0; i < n; i++) {
            float sum = 0.0f;
            for (int k = 0; k < n; k++) {
                printf("R, 0x%08x, \n", (uint32_t)((uintptr_t)&a[i * n + k] & 0xFFFFFFFF));
                printf("R, 0x%08x, \n", (uint32_t)((uintptr_t)&b[j * n + k] & 0xFFFFFFFF));
                sum += a[i * n + k] * b[j * n + k];
            }
            printf("W, 0x%08x, %u \n", (uint32_t)((uintptr_t)&result[j * n + i] & 0xFFFFFFFF), (unsigned int) sum);
            result[j * n + i] = sum;
        }
    }
}

/**
 * @brief A[i][k] * B[k][j] = C[i][j]
 * @note A, B, and C is transposed
 * @author Lie Leon Alexius
 */
void matrix_multiplication_jki_opt2(float* a, float* b, float* result, int n) {
    for (int j = 0; j < n; j++) {
        for (int k = 0; k < n; k++) {
            printf("R, 0x%08x, \n", (uint32_t)((uintptr_t)&b[j * n + k] & 0xFFFFFFFF));
            float colB = b[j * n + k];
            for (int i = 0; i < n; i++) {
                // result[j * n + i] += a[k * n + i] * colB;

                printf("R, 0x%08x, \n", (uint32_t)((uintptr_t)&a[k * n + i] & 0xFFFFFFFF));
                printf("R, 0x%08x, \n", (uint32_t)((uintptr_t)&result[j * n + i] & 0xFFFFFFFF));
                float sum2 = result[j * n + i] + a[k * n + i] * colB;

                printf("W, 0x%08x, %u \n", (uint32_t)((uintptr_t)&result[j * n + i] & 0xFFFFFFFF), (unsigned int) sum2);
                result[j * n + i] = sum2;
            }
        }
    }
}

/**
 * @brief A[i][k] * B[k][j] = C[i][j]
 * @note A is transposed
 * @author Lie Leon Alexius
 */
void matrix_multiplication_kij_opt2(float* a, float* b, float* result, int n) {
    for (int k = 0; k < n; k++) {
        for (int i = 0; i < n; i++) {
            printf("R, 0x%08x, \n", (uint32_t)((uintptr_t)&a[k * n + i] & 0xFFFFFFFF));
            float colA = a[k * n + i];
            for (int j = 0; j < n; j++) {
                // result[i * n + j] += colA * b[k * n + j];

                printf("R, 0x%08x, \n", (uint32_t)((uintptr_t)&b[k * n + j] & 0xFFFFFFFF));
                printf("R, 0x%08x, \n", (uint32_t)((uintptr_t)&result[i * n + j] & 0xFFFFFFFF));
                float sum2 = result[i * n + j] + colA * b[k * n + j];

                printf("W, 0x%08x, %u \n", (uint32_t)((uintptr_t)&result[i * n + j] & 0xFFFFFFFF), (unsigned int) sum2);
                result[i * n + j] = sum2;
            }
        }
    }
}

/**
 * @brief A[i][k] * B[k][j] = C[i][j]
 * @note A and C is transposed
 * @author Lie Leon Alexius
 */
void matrix_multiplication_kji_opt2(float* a, float* b, float* result, int n) {
    for (int k = 0; k < n; k++) {
        for (int j = 0; j < n; j++) {
            printf("R, 0x%08x, \n", (uint32_t)((uintptr_t)&b[k * n + j] & 0xFFFFFFFF));
            float rowB = b[k * n + j];
            for (int i = 0; i < n; i++) {
                // result[j * n + i] += a[k * n + i] * rowB;

                printf("R, 0x%08x, \n", (uint32_t)((uintptr_t)&a[k * n + i] & 0xFFFFFFFF));
                printf("R, 0x%08x, \n", (uint32_t)((uintptr_t)&result[j * n + i] & 0xFFFFFFFF));
                float sum2 = result[j * n + i] + a[k * n + i] * rowB;

                printf("W, 0x%08x, %u \n", (uint32_t)((uintptr_t)&result[j * n + i] & 0xFFFFFFFF), (unsigned int) sum2);
                result[j * n + i] = sum2;
            }
        }
    }
}

// =========================================== Run Creator Here ===========================================

int create_csv() {
    int n = 16;

    float* a = (float*) malloc(n * n * sizeof(float));
    float* b = (float*) malloc(n * n * sizeof(float));
    float* result = (float*) malloc(n * n * sizeof(float));
    float* result_transposed = (float*) malloc(n * n * sizeof(float));

    init_matrix(a, n);
    init_matrix(b, n);

    float* a_transposed = (float*) malloc(n * n * sizeof(float));
    float* b_transposed = (float*) malloc(n * n * sizeof(float));
    transpose_matrix(a, a_transposed, n);
    transpose_matrix(b, b_transposed, n);

    reset_matrix(result, n);
    reset_matrix(result_transposed, n);

    // call here
    // matrix_multiplication_ijk(a, b, result, n);
    // matrix_multiplication_ikj(a, b, result, n);
    // matrix_multiplication_jik(a, b, result, n);
    // matrix_multiplication_jki(a, b, result, n);
    // matrix_multiplication_kij(a, b, result, n);
    // matrix_multiplication_kji(a, b, result, n);

    // matrix_multiplication_ijk_opt1(a, b, result, n);
    // matrix_multiplication_ikj_opt1(a, b, result, n);
    // matrix_multiplication_jik_opt1(a, b, result, n);
    // matrix_multiplication_jki_opt1(a, b, result, n);
    // matrix_multiplication_kij_opt1(a, b, result, n);
    // matrix_multiplication_kji_opt1(a, b, result, n);

    // matrix_multiplication_ijk_opt2(a, b_transposed, result, n);
    // matrix_multiplication_jik_opt2(a, b_transposed, result_transposed, n);
    // matrix_multiplication_jki_opt2(a_transposed, b_transposed, result_transposed, n);
    // matrix_multiplication_kij_opt2(a_transposed, b, result, n);
    // matrix_multiplication_kji_opt2(a_transposed, b, result_transposed, n);

    // free
    free(a);
    free(b);
    free(a_transposed);
    free(b_transposed);
    free(result);
    free(result_transposed);

    return 0;
}
