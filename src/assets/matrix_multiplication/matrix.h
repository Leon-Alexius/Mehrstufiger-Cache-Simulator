// Lie Leon Alexius

#ifndef MATRIX_H
#define MATRIX_H

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>

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
                result[i * n + j] += a[i * n + k] * b[k * n + j];
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
                result[i * n + j] += a[i * n + k] * b[k * n + j];
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
                result[i * n + j] += a[i * n + k] * b[k * n + j];
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
                result[i * n + j] += a[i * n + k] * b[k * n + j];
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
                result[i * n + j] += a[i * n + k] * b[k * n + j];
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
                result[i * n + j] += a[i * n + k] * b[k * n + j];
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
                sum += a[i * n + k] * b[k * n + j];
            }
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
            float rowA = a[i * n + k];
            for (int j = 0; j < n; j++) {
                result[i * n + j] += rowA * b[k * n + j];
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
                sum += a[i * n + k] * b[k * n + j];
            }
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
            float colB = b[k * n + j];
            for (int i = 0; i < n; i++) {
                result[i * n + j] += a[i * n + k] * colB;
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
            float colA = a[i * n + k];
            for (int j = 0; j < n; j++) {
                result[i * n + j] += colA * b[k * n + j];
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
            float rowB = b[k * n + j];
            for (int i = 0; i < n; i++) {
                result[i * n + j] += a[i * n + k] * rowB;
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
                sum += a[i * n + k] * b[j * n + k];
            }
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
                sum += a[i * n + k] * b[j * n + k];
            }
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
            float colB = b[j * n + k];
            for (int i = 0; i < n; i++) {
                result[j * n + i] += a[k * n + i] * colB;
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
            float colA = a[k * n + i];
            for (int j = 0; j < n; j++) {
                result[i * n + j] += colA * b[k * n + j];
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
            float rowB = b[k * n + j];
            for (int i = 0; i < n; i++) {
                result[j * n + i] += a[k * n + i] * rowB;
            }
        }
    }
}

#endif // MATRIX_H