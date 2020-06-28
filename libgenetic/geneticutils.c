#include <stdlib.h>
#include <assert.h>

/*
 * Function: double_rand_interval
 * -------------------------------
 *  Generates and returns a random double between any interval
 */
double double_rand_interval(double min, double max) {
    double scale = (double)rand() / (double)RAND_MAX;
    return min + scale * (max - min);
}

/*
 * Function: int_rand_interval
 * ----------------------------
 *  Generates and returns a random int between any interval
 */
int int_rand_interval(int min, int max) {
    return (rand() % (max - min + 1)) + min;
}

/*
 * Function: free_pointer_matrix
 * -----------------------------
 * Frees a matrix passed as a double pointer given the number
 * of rows
 *
 * matrix: matrix to be freed
 * no_rows: number of rows to be freed
 */
void free_pointer_matrix(void **matrix, int no_rows) {
    assert(matrix);

    for (int i = 0; i < no_rows; ++i) {
        free(matrix[i]);
    }
    free(matrix);
}
