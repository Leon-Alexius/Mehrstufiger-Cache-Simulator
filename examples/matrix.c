// Lie Leon Alexius
#include "matrix.h"
#include <time.h>

/**
 * @brief Do run time analysis on different matrix multiplication accesses
 * @warning We count the time only the algorithm, not included transposing the matrix.
 * @author Lie Leon Alexius
 */
int analyze_runtime() {
    // for testing correctness, we have square_matrix of size 5
    int n = 1000;

    // Allocate Memory
    float* a = (float*) malloc(n * n * sizeof(float));
    float* b = (float*) malloc(n * n * sizeof(float));
    float* result = (float*) malloc(n * n * sizeof(float));
    float* result_transposed = (float*) malloc(n * n * sizeof(float));

    // Fill the array
    init_matrix(a, n);
    init_matrix(b, n);

    // Analyze how long does the transpose take
    clock_t sT, eT;
    double cpu_time_usedT;

    // transposed matrix
    float* a_transposed = (float*) malloc(n * n * sizeof(float));
    float* b_transposed = (float*) malloc(n * n * sizeof(float));
    sT = clock();
    transpose_matrix(a, a_transposed, n);
    eT = clock();
    cpu_time_usedT = ((double) (eT - sT)) / CLOCKS_PER_SEC;
    sT = clock();
    transpose_matrix(b, b_transposed, n);
    eT = clock();
    cpu_time_usedT += ((double) (eT - sT)) / CLOCKS_PER_SEC;
    cpu_time_usedT /= 2;
    printf("Transpose needed: %f\n", cpu_time_usedT);

    // For Analyzing
    clock_t start, end;
    double cpu_time_used;

    // ====================================================================================================

    // Analyze here
    printf("| %-15s | %-10s |\n", "Method", "Time (s)");
    printf("|-----------------|------------|\n");

    char* methods[] = {
            "ijk", "ikj", "jik", "jki", "kij", "kji",
            "ijk_opt1", "ikj_opt1", "jik_opt1", "jki_opt1", "kij_opt1", "kji_opt1",
            "ijk_opt2", "jik_opt2", "jki_opt2", "kij_opt2", "kji_opt2"
    };
    void (*funcs[]) (float*, float*, float*, int) = {
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
        if (m == 16 || m == 12) {
            printf("|-----------------|------------|\n");
        }

        // Reset result matrix
        reset_matrix(result, n);
        reset_matrix(result_transposed, n);

        // Invoke the function pointer
        if (strcmp(methods[m], "ijk_opt2") == 0) {
            start = clock();
            funcs[m](a, b_transposed, result, n);
            end = clock();
        }
        else if (strcmp(methods[m], "jik_opt2") == 0) {
            start = clock();
            funcs[m](a, b_transposed, result_transposed, n);
            end = clock();
            transpose_matrix(result_transposed, result, n);
        }
        else if (strcmp(methods[m], "jki_opt2") == 0) {
            start = clock();
            funcs[m](a_transposed, b_transposed, result_transposed, n);
            end = clock();
            transpose_matrix(result_transposed, result, n);
        }
        else if (strcmp(methods[m], "kij_opt2") == 0) {
            start = clock();
            funcs[m](a_transposed, b, result, n);
            end = clock();
        }
        else if (strcmp(methods[m], "kji_opt2") == 0) {
            start = clock();
            funcs[m](a_transposed, b, result_transposed, n);
            end = clock();
            transpose_matrix(result_transposed, result, n);
        }
        else {
            start = clock();
            funcs[m](a, b, result, n);
            end = clock();
        }

        cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;
        printf("| %-15s | %-10f |\n", methods[m], cpu_time_used);
    }

    // ====================================================================================================

    // Free allocated memory
    free_matrix(a);
    free_matrix(b);
    free_matrix(a_transposed);
    free_matrix(b_transposed);
    free_matrix(result);

    return 0;
}
