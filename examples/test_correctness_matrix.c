// Lie Leon Alexius
#include <stdio.h>
#include <stdlib.h>

#define RAND_MAX_VAL 100

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
 * @brief A[i][k] * B[k][j] = C[i][j]
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

/**
 * @brief Do test on various cache behavior
 * @author Lie Leon Alexius
 */
int main() {
    // for testing correctness, we have square_matrix of size 5
    int n = 5;

    // Allocate Memory
    float* a = (float*) malloc(n * n * sizeof(float));
    float* b = (float*) malloc(n * n * sizeof(float));
    float* result = (float*) calloc(n * n, sizeof(float)); // Initialize to 0

    // Fill the array
    init_matrix(a, n);
    init_matrix(b, n);

    // print a, b
    printf("Matrix A\n");
    print_matrix(a, n);
    printf("\nMatrix B\n");
    print_matrix(b, n);
    printf("\n");

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

        // Invoke the function pointer
        funcs[m](a, b, result, n);

        // print result matrix
        printf("Result of %s\n", methods[m]);
        print_matrix(result, n);
        printf("\n");
    }

    // Free allocated memory
    free_matrix(a);
    free_matrix(b);
    free_matrix(result);

    return 0;
}
