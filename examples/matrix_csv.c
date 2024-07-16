// Lie Leon Alexius
// This is a modified matrix.h to create .csv requests
// NOT A DUPLICATE! DON'T DELETE THIS!

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
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
void transpose_matrix(const float* matrix, float* transposed_matrix, int n) {
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
void matrix_multiplication_ijk(const float* a, const float* b, float* result, int n) {
    // w+ is "write and read", file is created if it doesn't exist or truncated if exist
    FILE* file = fopen("examples/ijk/ijk.csv", "w+");
    if (file == NULL) {
        fprintf(stderr, "Failed to open file\n");
        exit(EXIT_FAILURE);
    }

    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            for (int k = 0; k < n; k++) {
                // result[i * n + j] += a[i * n + k] * b[k * n + j];

                fprintf(file, "R, 0x%08x, \n", (uint32_t)((uintptr_t)&a[i * n + k] & 0xFFFFFFFF));
                fprintf(file, "R, 0x%08x, \n", (uint32_t)((uintptr_t)&b[k * n + j] & 0xFFFFFFFF));
                float sum = a[i * n + k] * b[k * n + j];

                fprintf(file, "R, 0x%08x, \n", (uint32_t)((uintptr_t)&result[i * n + j] & 0xFFFFFFFF));
                float sum2 = result[i * n + j] + sum;

                fprintf(file, "W, 0x%08x, %u \n", (uint32_t)((uintptr_t)&result[i * n + j] & 0xFFFFFFFF), (unsigned int) sum2);
                result[i * n + j] = sum2;
            }
        }
    }

    // Flush the file buffer to ensure all data is written to the file
    if (fflush(file) != 0) {
        fprintf(stderr, "Failed to flush file\n");
        exit(EXIT_FAILURE);
    }

    // Close the file
    if (fclose(file) != 0) {
        fprintf(stderr, "Failed to close file\n");
        exit(EXIT_FAILURE);
    }
}

/**
 * @brief A[i][k] * B[k][j] = C[i][j]
 * @author Lie Leon Alexius
 */
void matrix_multiplication_ikj(const float* a, const float* b, float* result, int n) {
    // w+ is "write and read", file is created if it doesn't exist or truncated if exist
    FILE* file = fopen("examples/ikj/ikj.csv", "w+");
    if (file == NULL) {
        fprintf(stderr, "Failed to open file\n");
        exit(EXIT_FAILURE);
    }

    for (int i = 0; i < n; i++) {
        for (int k = 0; k < n; k++) {
            for (int j = 0; j < n; j++) {
                // result[i * n + j] += a[i * n + k] * b[k * n + j];

                fprintf(file, "R, 0x%08x, \n", (uint32_t)((uintptr_t)&a[i * n + k] & 0xFFFFFFFF));
                fprintf(file, "R, 0x%08x, \n", (uint32_t)((uintptr_t)&b[k * n + j] & 0xFFFFFFFF));
                float sum = a[i * n + k] * b[k * n + j];

                fprintf(file, "R, 0x%08x, \n", (uint32_t)((uintptr_t)&result[i * n + j] & 0xFFFFFFFF));
                float sum2 = result[i * n + j] + sum;

                fprintf(file, "W, 0x%08x, %u \n", (uint32_t)((uintptr_t)&result[i * n + j] & 0xFFFFFFFF), (unsigned int) sum2);
                result[i * n + j] = sum2;
            }
        }
    }

    // Flush the file buffer to ensure all data is written to the file
    if (fflush(file) != 0) {
        fprintf(stderr, "Failed to flush file\n");
        exit(EXIT_FAILURE);
    }

    // Close the file
    if (fclose(file) != 0) {
        fprintf(stderr, "Failed to close file\n");
        exit(EXIT_FAILURE);
    }
}

/**
 * @brief A[i][k] * B[k][j] = C[i][j]
 * @author Lie Leon Alexius
 */
void matrix_multiplication_jik(const float* a, const float* b, float* result, int n) {
    // w+ is "write and read", file is created if it doesn't exist or truncated if exist
    FILE* file = fopen("examples/jik/jik.csv", "w+");
    if (file == NULL) {
        fprintf(stderr, "Failed to open file\n");
        exit(EXIT_FAILURE);
    }

    for (int j = 0; j < n; j++) {
        for (int i = 0; i < n; i++) {
            for (int k = 0; k < n; k++) {
                // result[i * n + j] += a[i * n + k] * b[k * n + j];

                fprintf(file, "R, 0x%08x, \n", (uint32_t)((uintptr_t)&a[i * n + k] & 0xFFFFFFFF));
                fprintf(file, "R, 0x%08x, \n", (uint32_t)((uintptr_t)&b[k * n + j] & 0xFFFFFFFF));
                float sum = a[i * n + k] * b[k * n + j];

                fprintf(file, "R, 0x%08x, \n", (uint32_t)((uintptr_t)&result[i * n + j] & 0xFFFFFFFF));
                float sum2 = result[i * n + j] + sum;

                fprintf(file, "W, 0x%08x, %u \n", (uint32_t)((uintptr_t)&result[i * n + j] & 0xFFFFFFFF), (unsigned int) sum2);
                result[i * n + j] = sum2;
            }
        }
    }

    // Flush the file buffer to ensure all data is written to the file
    if (fflush(file) != 0) {
        fprintf(stderr, "Failed to flush file\n");
        exit(EXIT_FAILURE);
    }

    // Close the file
    if (fclose(file) != 0) {
        fprintf(stderr, "Failed to close file\n");
        exit(EXIT_FAILURE);
    }
}

/**
 * @brief A[i][k] * B[k][j] = C[i][j]
 * @author Lie Leon Alexius
 */
void matrix_multiplication_jki(const float* a, const float* b, float* result, int n) {
    // w+ is "write and read", file is created if it doesn't exist or truncated if exist
    FILE* file = fopen("examples/jki/jki.csv", "w+");
    if (file == NULL) {
        fprintf(stderr, "Failed to open file\n");
        exit(EXIT_FAILURE);
    }

    for (int j = 0; j < n; j++) {
        for (int k = 0; k < n; k++) {
            for (int i = 0; i < n; i++) {
                // result[i * n + j] += a[i * n + k] * b[k * n + j];

                fprintf(file, "R, 0x%08x, \n", (uint32_t)((uintptr_t)&a[i * n + k] & 0xFFFFFFFF));
                fprintf(file, "R, 0x%08x, \n", (uint32_t)((uintptr_t)&b[k * n + j] & 0xFFFFFFFF));
                float sum = a[i * n + k] * b[k * n + j];

                fprintf(file, "R, 0x%08x, \n", (uint32_t)((uintptr_t)&result[i * n + j] & 0xFFFFFFFF));
                float sum2 = result[i * n + j] + sum;

                fprintf(file, "W, 0x%08x, %u \n", (uint32_t)((uintptr_t)&result[i * n + j] & 0xFFFFFFFF), (unsigned int) sum2);
                result[i * n + j] = sum2;
            }
        }
    }

    // Flush the file buffer to ensure all data is written to the file
    if (fflush(file) != 0) {
        fprintf(stderr, "Failed to flush file\n");
        exit(EXIT_FAILURE);
    }

    // Close the file
    if (fclose(file) != 0) {
        fprintf(stderr, "Failed to close file\n");
        exit(EXIT_FAILURE);
    }
}

/**
 * @brief A[i][k] * B[k][j] = C[i][j]
 * @author Lie Leon Alexius
 */
void matrix_multiplication_kij(const float* a, const float* b, float* result, int n) {
    // w+ is "write and read", file is created if it doesn't exist or truncated if exist
    FILE* file = fopen("examples/kij/kij.csv", "w+");
    if (file == NULL) {
        fprintf(stderr, "Failed to open file\n");
        exit(EXIT_FAILURE);
    }

    for (int k = 0; k < n; k++) {
        for (int i = 0; i < n; i++) {
            for (int j = 0; j < n; j++) {
                // result[i * n + j] += a[i * n + k] * b[k * n + j];

                fprintf(file, "R, 0x%08x, \n", (uint32_t)((uintptr_t)&a[i * n + k] & 0xFFFFFFFF));
                fprintf(file, "R, 0x%08x, \n", (uint32_t)((uintptr_t)&b[k * n + j] & 0xFFFFFFFF));
                float sum = a[i * n + k] * b[k * n + j];

                fprintf(file, "R, 0x%08x, \n", (uint32_t)((uintptr_t)&result[i * n + j] & 0xFFFFFFFF));
                float sum2 = result[i * n + j] + sum;

                fprintf(file, "W, 0x%08x, %u \n", (uint32_t)((uintptr_t)&result[i * n + j] & 0xFFFFFFFF), (unsigned int) sum2);
                result[i * n + j] = sum2;
            }
        }
    }

    // Flush the file buffer to ensure all data is written to the file
    if (fflush(file) != 0) {
        fprintf(stderr, "Failed to flush file\n");
        exit(EXIT_FAILURE);
    }

    // Close the file
    if (fclose(file) != 0) {
        fprintf(stderr, "Failed to close file\n");
        exit(EXIT_FAILURE);
    }
}

/**
 * @brief A[i][k] * B[k][j] = C[i][j]
 * @author Lie Leon Alexius
 */
void matrix_multiplication_kji(const float* a, const float* b, float* result, int n) {
    // w+ is "write and read", file is created if it doesn't exist or truncated if exist
    FILE* file = fopen("examples/kji/kji.csv", "w+");
    if (file == NULL) {
        fprintf(stderr, "Failed to open file\n");
        exit(EXIT_FAILURE);
    }

    for (int k = 0; k < n; k++) {
        for (int j = 0; j < n; j++) {
            for (int i = 0; i < n; i++) {
                // result[i * n + j] += a[i * n + k] * b[k * n + j];

                fprintf(file, "R, 0x%08x, \n", (uint32_t)((uintptr_t)&a[i * n + k] & 0xFFFFFFFF));
                fprintf(file, "R, 0x%08x, \n", (uint32_t)((uintptr_t)&b[k * n + j] & 0xFFFFFFFF));
                float sum = a[i * n + k] * b[k * n + j];

                fprintf(file, "R, 0x%08x, \n", (uint32_t)((uintptr_t)&result[i * n + j] & 0xFFFFFFFF));
                float sum2 = result[i * n + j] + sum;

                fprintf(file, "W, 0x%08x, %u \n", (uint32_t)((uintptr_t)&result[i * n + j] & 0xFFFFFFFF), (unsigned int) sum2);
                result[i * n + j] = sum2;
            }
        }
    }

    // Flush the file buffer to ensure all data is written to the file
    if (fflush(file) != 0) {
        fprintf(stderr, "Failed to flush file\n");
        exit(EXIT_FAILURE);
    }

    // Close the file
    if (fclose(file) != 0) {
        fprintf(stderr, "Failed to close file\n");
        exit(EXIT_FAILURE);
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
void matrix_multiplication_ijk_opt1(const float* a, const float* b, float* result, int n) {
    // w+ is "write and read", file is created if it doesn't exist or truncated if exist
    FILE* file = fopen("examples/ijk/ijk_opt1.csv", "w+");
    if (file == NULL) {
        fprintf(stderr, "Failed to open file\n");
        exit(EXIT_FAILURE);
    }

    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            float sum = 0.0f;
            for (int k = 0; k < n; k++) {
                fprintf(file, "R, 0x%08x, \n", (uint32_t)((uintptr_t)&a[i * n + k] & 0xFFFFFFFF));
                fprintf(file, "R, 0x%08x, \n", (uint32_t)((uintptr_t)&b[k * n + j] & 0xFFFFFFFF));
                sum += a[i * n + k] * b[k * n + j];
            }
            fprintf(file, "W, 0x%08x, %u \n", (uint32_t)((uintptr_t)&result[i * n + j] & 0xFFFFFFFF), (unsigned int) sum);
            result[i * n + j] = sum;
        }
    }

    // Flush the file buffer to ensure all data is written to the file
    if (fflush(file) != 0) {
        fprintf(stderr, "Failed to flush file\n");
        exit(EXIT_FAILURE);
    }

    // Close the file
    if (fclose(file) != 0) {
        fprintf(stderr, "Failed to close file\n");
        exit(EXIT_FAILURE);
    }
}

/**
 * @brief A[i][k] * B[k][j] = C[i][j]
 * @author Lie Leon Alexius
 */
void matrix_multiplication_ikj_opt1(const float* a, const float* b, float* result, int n) {
    // w+ is "write and read", file is created if it doesn't exist or truncated if exist
    FILE* file = fopen("examples/ikj/ikj_opt1.csv", "w+");
    if (file == NULL) {
        fprintf(stderr, "Failed to open file\n");
        exit(EXIT_FAILURE);
    }

    for (int i = 0; i < n; i++) {
        for (int k = 0; k < n; k++) {
            fprintf(file, "R, 0x%08x, \n", (uint32_t)((uintptr_t)&a[i * n + k] & 0xFFFFFFFF));
            float rowA = a[i * n + k];
            for (int j = 0; j < n; j++) {
                fprintf(file, "R, 0x%08x, \n", (uint32_t)((uintptr_t)&b[k * n + j] & 0xFFFFFFFF));
                fprintf(file, "R, 0x%08x, \n", (uint32_t)((uintptr_t)&result[i * n + j] & 0xFFFFFFFF));
                float sum2 = result[i * n + j] + rowA * b[k * n + j];

                fprintf(file, "W, 0x%08x, %u \n", (uint32_t)((uintptr_t)&result[i * n + j] & 0xFFFFFFFF), (unsigned int) sum2);
                result[i * n + j] = sum2;
            }
        }
    }

    // Flush the file buffer to ensure all data is written to the file
    if (fflush(file) != 0) {
        fprintf(stderr, "Failed to flush file\n");
        exit(EXIT_FAILURE);
    }

    // Close the file
    if (fclose(file) != 0) {
        fprintf(stderr, "Failed to close file\n");
        exit(EXIT_FAILURE);
    }
}

/**
 * @brief A[i][k] * B[k][j] = C[i][j]
 * @author Lie Leon Alexius
 */
void matrix_multiplication_jik_opt1(const float* a, const float* b, float* result, int n) {
    // w+ is "write and read", file is created if it doesn't exist or truncated if exist
    FILE* file = fopen("examples/jik/jik_opt1.csv", "w+");
    if (file == NULL) {
        fprintf(stderr, "Failed to open file\n");
        exit(EXIT_FAILURE);
    }

    for (int j = 0; j < n; j++) {
        for (int i = 0; i < n; i++) {
            float sum = 0.0f;
            for (int k = 0; k < n; k++) {
                fprintf(file, "R, 0x%08x, \n", (uint32_t)((uintptr_t)&a[i * n + k] & 0xFFFFFFFF));
                fprintf(file, "R, 0x%08x, \n", (uint32_t)((uintptr_t)&b[k * n + j] & 0xFFFFFFFF));
                sum += a[i * n + k] * b[k * n + j];
            }
            fprintf(file, "W, 0x%08x, %u \n", (uint32_t)((uintptr_t)&result[i * n + j] & 0xFFFFFFFF), (unsigned int) sum);
            result[i * n + j] = sum;
        }
    }

    // Flush the file buffer to ensure all data is written to the file
    if (fflush(file) != 0) {
        fprintf(stderr, "Failed to flush file\n");
        exit(EXIT_FAILURE);
    }

    // Close the file
    if (fclose(file) != 0) {
        fprintf(stderr, "Failed to close file\n");
        exit(EXIT_FAILURE);
    }
}

/**
 * @brief A[i][k] * B[k][j] = C[i][j]
 * @author Lie Leon Alexius
 */
void matrix_multiplication_jki_opt1(const float* a, const float* b, float* result, int n) {
    // w+ is "write and read", file is created if it doesn't exist or truncated if exist
    FILE* file = fopen("examples/jki/jki_opt1.csv", "w+");
    if (file == NULL) {
        fprintf(stderr, "Failed to open file\n");
        exit(EXIT_FAILURE);
    }

    for (int j = 0; j < n; j++) {
        for (int k = 0; k < n; k++) {
            fprintf(file, "R, 0x%08x, \n", (uint32_t)((uintptr_t)&b[k * n + j] & 0xFFFFFFFF));
            float colB = b[k * n + j];
            for (int i = 0; i < n; i++) {
                fprintf(file, "R, 0x%08x, \n", (uint32_t)((uintptr_t)&a[i * n + k] & 0xFFFFFFFF));
                fprintf(file, "R, 0x%08x, \n", (uint32_t)((uintptr_t)&result[i * n + j] & 0xFFFFFFFF));
                float sum2 = result[i * n + j] + a[i * n + k] * colB;

                fprintf(file, "W, 0x%08x, %u \n", (uint32_t)((uintptr_t)&result[i * n + j] & 0xFFFFFFFF), (unsigned int) sum2);
                result[i * n + j] = sum2;
            }
        }
    }

    // Flush the file buffer to ensure all data is written to the file
    if (fflush(file) != 0) {
        fprintf(stderr, "Failed to flush file\n");
        exit(EXIT_FAILURE);
    }

    // Close the file
    if (fclose(file) != 0) {
        fprintf(stderr, "Failed to close file\n");
        exit(EXIT_FAILURE);
    }
}

/**
 * @brief A[i][k] * B[k][j] = C[i][j]
 * @author Lie Leon Alexius
 */
void matrix_multiplication_kij_opt1(const float* a, const float* b, float* result, int n) {
    // w+ is "write and read", file is created if it doesn't exist or truncated if exist
    FILE* file = fopen("examples/kij/kij_opt1.csv", "w+");
    if (file == NULL) {
        fprintf(stderr, "Failed to open file\n");
        exit(EXIT_FAILURE);
    }

    for (int k = 0; k < n; k++) {
        for (int i = 0; i < n; i++) {
            fprintf(file, "R, 0x%08x, \n", (uint32_t)((uintptr_t)&a[i * n + k] & 0xFFFFFFFF));
            float colA = a[i * n + k];
            for (int j = 0; j < n; j++) {
                fprintf(file, "R, 0x%08x, \n", (uint32_t)((uintptr_t)&b[k * n + j] & 0xFFFFFFFF));
                fprintf(file, "R, 0x%08x, \n", (uint32_t)((uintptr_t)&result[i * n + j] & 0xFFFFFFFF));
                float sum2 = result[i * n + j] + colA * b[k * n + j];

                fprintf(file, "W, 0x%08x, %u \n", (uint32_t)((uintptr_t)&result[i * n + j] & 0xFFFFFFFF), (unsigned int) sum2);
                result[i * n + j] = sum2;
            }
        }
    }

    // Flush the file buffer to ensure all data is written to the file
    if (fflush(file) != 0) {
        fprintf(stderr, "Failed to flush file\n");
        exit(EXIT_FAILURE);
    }

    // Close the file
    if (fclose(file) != 0) {
        fprintf(stderr, "Failed to close file\n");
        exit(EXIT_FAILURE);
    }
}

/**
 * @brief A[i][k] * B[k][j] = C[i][j]
 * @author Lie Leon Alexius
 */
void matrix_multiplication_kji_opt1(const float* a, const float* b, float* result, int n) {
    // w+ is "write and read", file is created if it doesn't exist or truncated if exist
    FILE* file = fopen("examples/kji/kji_opt1.csv", "w+");
    if (file == NULL) {
        fprintf(stderr, "Failed to open file\n");
        exit(EXIT_FAILURE);
    }

    for (int k = 0; k < n; k++) {
        for (int j = 0; j < n; j++) {
            fprintf(file, "R, 0x%08x, \n", (uint32_t)((uintptr_t)&b[k * n + j] & 0xFFFFFFFF));
            float rowB = b[k * n + j];
            for (int i = 0; i < n; i++) {
                fprintf(file, "R, 0x%08x, \n", (uint32_t)((uintptr_t)&a[i * n + k] & 0xFFFFFFFF));
                fprintf(file, "R, 0x%08x, \n", (uint32_t)((uintptr_t)&result[i * n + j] & 0xFFFFFFFF));
                float sum2 = result[i * n + j] + a[i * n + k] * rowB;

                fprintf(file, "W, 0x%08x, %u \n", (uint32_t)((uintptr_t)&result[i * n + j] & 0xFFFFFFFF), (unsigned int) sum2);
                result[i * n + j] = sum2;
            }
        }
    }

    // Flush the file buffer to ensure all data is written to the file
    if (fflush(file) != 0) {
        fprintf(stderr, "Failed to flush file\n");
        exit(EXIT_FAILURE);
    }

    // Close the file
    if (fclose(file) != 0) {
        fprintf(stderr, "Failed to close file\n");
        exit(EXIT_FAILURE);
    }
}

// ===================================== Optimized 2 Matrix Multiplication =====================================

/*
    We transpose either A or B for better Stride + use inner variable to reduce the number of memory accesses

    Combinations:
    ijk
    ikj - not applicable since everything is already accessed by row-wise
    jik
    jki - is basically ikj for matrix that is saved column-wise
    kij
    kji
*/

/**
 * @brief A[i][k] * B_T[j][k] = C[i][j]
 * @note B is transposed
 * @author Lie Leon Alexius
 */
void matrix_multiplication_ijk_opt2(const float* a, const float* b, float* result, int n) {
    // w+ is "write and read", file is created if it doesn't exist or truncated if exist
    FILE* file = fopen("examples/ijk/ijk_opt2.csv", "w+");
    if (file == NULL) {
        fprintf(stderr, "Failed to open file\n");
        exit(EXIT_FAILURE);
    }

    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            float sum = 0.0f;
            for (int k = 0; k < n; k++) {
                fprintf(file, "R, 0x%08x, \n", (uint32_t)((uintptr_t)&a[i * n + k] & 0xFFFFFFFF));
                fprintf(file, "R, 0x%08x, \n", (uint32_t)((uintptr_t)&b[j * n + k] & 0xFFFFFFFF));
                sum += a[i * n + k] * b[j * n + k];
            }
            fprintf(file, "W, 0x%08x, %u \n", (uint32_t)((uintptr_t)&result[i * n + j] & 0xFFFFFFFF), (unsigned int) sum);
            result[i * n + j] = sum;
        }
    }

    // Flush the file buffer to ensure all data is written to the file
    if (fflush(file) != 0) {
        fprintf(stderr, "Failed to flush file\n");
        exit(EXIT_FAILURE);
    }

    // Close the file
    if (fclose(file) != 0) {
        fprintf(stderr, "Failed to close file\n");
        exit(EXIT_FAILURE);
    }
}

/**
 * @brief A[i][k] * B[k][j] = C[i][j]
 * @note B and C is transposed
 * @author Lie Leon Alexius
 */
void matrix_multiplication_jik_opt2(const float* a, const float* b, float* result, int n) {
    // w+ is "write and read", file is created if it doesn't exist or truncated if exist
    FILE* file = fopen("examples/jik/jik_opt2.csv", "w+");
    if (file == NULL) {
        fprintf(stderr, "Failed to open file\n");
        exit(EXIT_FAILURE);
    }

    for (int j = 0; j < n; j++) {
        for (int i = 0; i < n; i++) {
            float sum = 0.0f;
            for (int k = 0; k < n; k++) {
                fprintf(file, "R, 0x%08x, \n", (uint32_t)((uintptr_t)&a[i * n + k] & 0xFFFFFFFF));
                fprintf(file, "R, 0x%08x, \n", (uint32_t)((uintptr_t)&b[j * n + k] & 0xFFFFFFFF));
                sum += a[i * n + k] * b[j * n + k];
            }
            fprintf(file, "W, 0x%08x, %u \n", (uint32_t)((uintptr_t)&result[j * n + i] & 0xFFFFFFFF), (unsigned int) sum);
            result[j * n + i] = sum;
        }
    }

    // Flush the file buffer to ensure all data is written to the file
    if (fflush(file) != 0) {
        fprintf(stderr, "Failed to flush file\n");
        exit(EXIT_FAILURE);
    }

    // Close the file
    if (fclose(file) != 0) {
        fprintf(stderr, "Failed to close file\n");
        exit(EXIT_FAILURE);
    }
}

/**
 * @brief A[i][k] * B[k][j] = C[i][j]
 * @note A, B, and C is transposed
 * @author Lie Leon Alexius
 */
void matrix_multiplication_jki_opt2(const float* a, const float* b, float* result, int n) {
    // w+ is "write and read", file is created if it doesn't exist or truncated if exist
    FILE* file = fopen("examples/jki/jki_opt2.csv", "w+");
    if (file == NULL) {
        fprintf(stderr, "Failed to open file\n");
        exit(EXIT_FAILURE);
    }

    for (int j = 0; j < n; j++) {
        for (int k = 0; k < n; k++) {
            fprintf(file, "R, 0x%08x, \n", (uint32_t)((uintptr_t)&b[j * n + k] & 0xFFFFFFFF));
            float colB = b[j * n + k];
            for (int i = 0; i < n; i++) {
                // result[j * n + i] += a[k * n + i] * colB;

                fprintf(file, "R, 0x%08x, \n", (uint32_t)((uintptr_t)&a[k * n + i] & 0xFFFFFFFF));
                fprintf(file, "R, 0x%08x, \n", (uint32_t)((uintptr_t)&result[j * n + i] & 0xFFFFFFFF));
                float sum2 = result[j * n + i] + a[k * n + i] * colB;

                fprintf(file, "W, 0x%08x, %u \n", (uint32_t)((uintptr_t)&result[j * n + i] & 0xFFFFFFFF), (unsigned int) sum2);
                result[j * n + i] = sum2;
            }
        }
    }

    // Flush the file buffer to ensure all data is written to the file
    if (fflush(file) != 0) {
        fprintf(stderr, "Failed to flush file\n");
        exit(EXIT_FAILURE);
    }

    // Close the file
    if (fclose(file) != 0) {
        fprintf(stderr, "Failed to close file\n");
        exit(EXIT_FAILURE);
    }
}

/**
 * @brief A[i][k] * B[k][j] = C[i][j]
 * @note A is transposed
 * @author Lie Leon Alexius
 */
void matrix_multiplication_kij_opt2(const float* a, const float* b, float* result, int n) {
    // w+ is "write and read", file is created if it doesn't exist or truncated if exist
    FILE* file = fopen("examples/kij/kij_opt2.csv", "w+");
    if (file == NULL) {
        fprintf(stderr, "Failed to open file\n");
        exit(EXIT_FAILURE);
    }

    for (int k = 0; k < n; k++) {
        for (int i = 0; i < n; i++) {
            fprintf(file, "R, 0x%08x, \n", (uint32_t)((uintptr_t)&a[k * n + i] & 0xFFFFFFFF));
            float colA = a[k * n + i];
            for (int j = 0; j < n; j++) {
                // result[i * n + j] += colA * b[k * n + j];

                fprintf(file, "R, 0x%08x, \n", (uint32_t)((uintptr_t)&b[k * n + j] & 0xFFFFFFFF));
                fprintf(file, "R, 0x%08x, \n", (uint32_t)((uintptr_t)&result[i * n + j] & 0xFFFFFFFF));
                float sum2 = result[i * n + j] + colA * b[k * n + j];

                fprintf(file, "W, 0x%08x, %u \n", (uint32_t)((uintptr_t)&result[i * n + j] & 0xFFFFFFFF), (unsigned int) sum2);
                result[i * n + j] = sum2;
            }
        }
    }

    // Flush the file buffer to ensure all data is written to the file
    if (fflush(file) != 0) {
        fprintf(stderr, "Failed to flush file\n");
        exit(EXIT_FAILURE);
    }

    // Close the file
    if (fclose(file) != 0) {
        fprintf(stderr, "Failed to close file\n");
        exit(EXIT_FAILURE);
    }
}

/**
 * @brief A[i][k] * B[k][j] = C[i][j]
 * @note A and C is transposed
 * @author Lie Leon Alexius
 */
void matrix_multiplication_kji_opt2(const float* a, const float* b, float* result, int n) {
    // w+ is "write and read", file is created if it doesn't exist or truncated if exist
    FILE* file = fopen("examples/kji/kji_opt2.csv", "w+");
    if (file == NULL) {
        fprintf(stderr, "Failed to open file\n");
        exit(EXIT_FAILURE);
    }

    for (int k = 0; k < n; k++) {
        for (int j = 0; j < n; j++) {
            fprintf(file, "R, 0x%08x, \n", (uint32_t)((uintptr_t)&b[k * n + j] & 0xFFFFFFFF));
            float rowB = b[k * n + j];
            for (int i = 0; i < n; i++) {
                // result[j * n + i] += a[k * n + i] * rowB;

                fprintf(file, "R, 0x%08x, \n", (uint32_t)((uintptr_t)&a[k * n + i] & 0xFFFFFFFF));
                fprintf(file, "R, 0x%08x, \n", (uint32_t)((uintptr_t)&result[j * n + i] & 0xFFFFFFFF));
                float sum2 = result[j * n + i] + a[k * n + i] * rowB;

                fprintf(file, "W, 0x%08x, %u \n", (uint32_t)((uintptr_t)&result[j * n + i] & 0xFFFFFFFF), (unsigned int) sum2);
                result[j * n + i] = sum2;
            }
        }
    }

    // Flush the file buffer to ensure all data is written to the file
    if (fflush(file) != 0) {
        fprintf(stderr, "Failed to flush file\n");
        exit(EXIT_FAILURE);
    }

    // Close the file
    if (fclose(file) != 0) {
        fprintf(stderr, "Failed to close file\n");
        exit(EXIT_FAILURE);
    }
}

// =========================================== Run Creator Here ===========================================

/**
 * @brief Creates .csv for each of the patterns
 * @author Lie Leon Alexius
 */
int create_csv() {
    // for csv, we set matrix also as 1000 x 1000, total size of matrix = 4MB
    const int n = 1000;

    // Allocate Memory
    float* a = (float*) malloc(n * n * sizeof(float));
    if (a == NULL) {
        fprintf(stderr, "Failed allocating memory for matrix A\n");
        exit(EXIT_FAILURE);
    }

    float* b = (float*) malloc(n * n * sizeof(float));
    if (b == NULL) {
        free(a);
        a = NULL;
        fprintf(stderr, "Failed allocating memory for matrix B\n");
        exit(EXIT_FAILURE);
    }

    float* result = (float*) malloc(n * n * sizeof(float));
    if (result == NULL) {
        free(a);
        a = NULL;
        free(b);
        b = NULL;
        fprintf(stderr, "Failed allocating memory for matrix Result\n");
        exit(EXIT_FAILURE);
    }

    float* result_transposed = (float*) malloc(n * n * sizeof(float));
    if (result_transposed == NULL) {
        free(a);
        a = NULL;
        free(b);
        b = NULL;
        free(result);
        result = NULL;
        fprintf(stderr, "Failed allocating memory for matrix Result_T\n");
        exit(EXIT_FAILURE);
    }

    float* a_transposed = (float*) malloc(n * n * sizeof(float));
    if (a_transposed == NULL) {
        free(a);
        a = NULL;
        free(b);
        b = NULL;
        free(result);
        result = NULL;
        free(result_transposed);
        result_transposed = NULL;
        fprintf(stderr, "Failed allocating memory for matrix A_T\n");
        exit(EXIT_FAILURE);
    }

    float* b_transposed = (float*) malloc(n * n * sizeof(float));
    if (b_transposed == NULL) {
        free(a);
        a = NULL;
        free(b);
        b = NULL;
        free(result);
        result = NULL;
        free(result_transposed);
        result_transposed = NULL;
        free(a_transposed);
        a_transposed = NULL;
        fprintf(stderr, "Failed allocating memory for matrix B_T\n");
        exit(EXIT_FAILURE);
    }

    // Fill the array
    init_matrix(a, n);
    init_matrix(b, n);

    // Transpose the matrix
    transpose_matrix(a, a_transposed, n);
    transpose_matrix(b, b_transposed, n);

    // ====================================================================================================

    // call here one by one
    printf("| %-15s | %-10s |\n", "Method", "Time (s)");
    printf("|-----------------|------------|\n");

    char* methods[] = {
            "ijk", "ikj", "jik", "jki", "kij", "kji",
            "ijk_opt1", "ikj_opt1", "jik_opt1", "jki_opt1", "kij_opt1", "kji_opt1",
            "ijk_opt2", "jik_opt2", "jki_opt2", "kij_opt2", "kji_opt2"
    };
    void (*funcs[]) (const float*, const float*, float*, int) = {
            matrix_multiplication_ijk,
            matrix_multiplication_ikj,
            matrix_multiplication_jik,
            matrix_multiplication_jki,
            matrix_multiplication_kij,
            matrix_multiplication_kji,
            matrix_multiplication_ijk_opt1,
            matrix_multiplication_ikj_opt1,
            matrix_multiplication_jik_opt1,
            matrix_multiplication_jki_opt1,
            matrix_multiplication_kij_opt1,
            matrix_multiplication_kji_opt1,
            matrix_multiplication_ijk_opt2,
            matrix_multiplication_jik_opt2,
            matrix_multiplication_jki_opt2,
            matrix_multiplication_kij_opt2,
            matrix_multiplication_kji_opt2
    };

    for (int m = 0; m < 17; m++) {
        if (m == 6 || m == 12) {
            printf("|-----------------|------------|\n");
        }

        // Reset result matrix
        reset_matrix(result, n);
        reset_matrix(result_transposed, n);

        // Invoke the function pointer
        if (strcmp(methods[m], "ijk_opt2") == 0) {
            funcs[m](a, b_transposed, result, n);
        }
        else if (strcmp(methods[m], "jik_opt2") == 0) {
            funcs[m](a, b_transposed, result_transposed, n);
            transpose_matrix(result_transposed, result, n);
        }
        else if (strcmp(methods[m], "jki_opt2") == 0) {
            funcs[m](a_transposed, b_transposed, result_transposed, n);
            transpose_matrix(result_transposed, result, n);
        }
        else if (strcmp(methods[m], "kij_opt2") == 0) {
            funcs[m](a_transposed, b, result, n);
        }
        else if (strcmp(methods[m], "kji_opt2") == 0) {
            funcs[m](a_transposed, b, result_transposed, n);
            transpose_matrix(result_transposed, result, n);
        }
        else {
            funcs[m](a, b, result, n);
        }

        printf("| %-15s | %-10s |\n", methods[m], "done");
    }

    // ====================================================================================================

    // Free allocated memory
    free(a);
    free(b);
    free(a_transposed);
    free(b_transposed);
    free(result);
    free(result_transposed);

    return 0;
}
