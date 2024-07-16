// Lie Leon Alexius
#include "matrix.h"

#include <string.h>

/**
 * @brief Do test on standard matrix multiplication patterns for correctness
 * @author Lie Leon Alexius
 */
int test_standard(float* a, float* b, float* result, float control[25], int n) {

    char* methods[] = {"ijk", "ikj", "jik", "jki", "kij", "kji"};
    void (*funcs[]) (const float*, const float*, float*, int) = {
            matrix_multiplication_ijk,
            matrix_multiplication_ikj,
            matrix_multiplication_jik,
            matrix_multiplication_jki,
            matrix_multiplication_kij,
            matrix_multiplication_kji
    };

    for (int m = 0; m < 6; m++) {
        // Reset result matrix
        reset_matrix(result, n);

        // Invoke the function pointer
        funcs[m](a, b, result, n);

        // Compare result with control matrix
        if (compare_matrices(result, control, n) != 0) {
            printf("Error in method %s\n", methods[m]);

            // print result matrix
            printf("received result of %s:\n", methods[m]);
            print_matrix(result, n);
            printf("\n");

            break;
        }
        else {
            printf("Method %s is correct\n", methods[m]);
        }
    }

    return 0;
}

/**
 * @brief Do test on optimized1 matrix multiplication patterns for correctness
 * @author Lie Leon Alexius
 */
int test_optimized1(float* a, float* b, float* result, float control[25], int n) {

    char* methods[] = {"ijk_opt1", "ikj_opt1", "jik_opt1", "jki_opt1", "kij_opt1", "kji_opt1"};
    void (*funcs[]) (const float*, const float*, float*, int) = {
            matrix_multiplication_ijk_opt1,
            matrix_multiplication_ikj_opt1,
            matrix_multiplication_jik_opt1,
            matrix_multiplication_jki_opt1,
            matrix_multiplication_kij_opt1,
            matrix_multiplication_kji_opt1
    };

    for (int m = 0; m < 6; m++) {
        // Reset result matrix
        reset_matrix(result, n);

        // Invoke the function pointer
        funcs[m](a, b, result, n);

        // Compare result with control matrix
        if (compare_matrices(result, control, n) != 0) {
            printf("Error in method %s\n", methods[m]);

            // print result matrix
            printf("received result of %s:\n", methods[m]);
            print_matrix(result, n);
            printf("\n");

            break;
        }
        else {
            printf("Method %s is correct\n", methods[m]);
        }
    }

    return 0;
}

/**
 * @brief Do test on optimized2 matrix multiplication patterns for correctness
 * @author Lie Leon Alexius
 */
int test_optimized2(float* a, float* a_transposed, float* b, float* b_transposed, float* result, float* result_transposed, float control[25], int n) {

    char* methods[] = {"ijk_opt2", "jik_opt2", "jki_opt2", "kij_opt2", "kji_opt2"};
    void (*funcs[]) (const float*, const float*, float*, int) = {
            matrix_multiplication_ijk_opt2,
            matrix_multiplication_jik_opt2,
            matrix_multiplication_jki_opt2,
            matrix_multiplication_kij_opt2,
            matrix_multiplication_kji_opt2
    };

    for (int m = 0; m < 5; m++) {
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
        else {
            funcs[m](a_transposed, b, result_transposed, n);
            transpose_matrix(result_transposed, result, n);
        }

        // Compare result with control matrix
        if (compare_matrices(result, control, n) != 0) {
            printf("Error in method %s\n", methods[m]);

            // print result matrix
            printf("received result of %s:\n", methods[m]);
            print_matrix(result, n);
            printf("\n");

            break;
        }
        else {
            printf("Method %s is correct\n", methods[m]);
        }
    }

    return 0;
}

/**
 * @brief Test all matrix multiplication patterns for correctness
 * @author Lie Leon Alexius
 */
int test() {
    // for testing correctness, we have square_matrix of size 5
    int n = 5;

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

    // transpose
    transpose_matrix(a, a_transposed, n);
    transpose_matrix(b, b_transposed, n);

    // Control matrix
    float control[25] = {
            11335.00, 7710.00, 9538.00, 8131.00, 16406.00,
            10612.00, 9724.00, 13732.00, 10864.00, 21992.00,
            7482.00, 9500.00, 12559.00, 8759.00, 17313.00,
            14816.00, 8968.00, 13349.00, 9517.00, 23567.00,
            13721.00, 6504.00, 4116.00, 8485.00, 19234.00
    };

    // print a, b
    printf("Matrix A\n");
    print_matrix(a, n);
    printf("\nMatrix B\n");
    print_matrix(b, n);
    printf("\n");

    // do tests
    test_standard(a, b, result, control, n);
    test_optimized1(a, b, result, control, n);
    test_optimized2(a, a_transposed, b, b_transposed, result, result_transposed, control, n);

    // Free allocated memory
    free(a);
    free(b);
    free(a_transposed);
    free(b_transposed);
    free(result);

    return 0;
}
