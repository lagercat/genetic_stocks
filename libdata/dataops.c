#include <assert.h>
#include <math.h>
#include <stdlib.h>
#include <float.h>
#include <stdio.h>

#include "structures.h"
#include "createstructures.h"
#include "dataops.h"

/*
 * Function: get_min
 * ----------------------------
 * Compute the minimum value of a given matrix, on a given column.
 *
 * matrix: target matrix
 * length: number of rows
 * index: index of the target column
 *
 * result: minimum value of the elements from the indexth column
 */
double get_min(double **matrix, double length, int index) {
    assert(matrix);
    double result = DBL_MAX;

    for (int i = 0; i < length; ++i) {
        result = result > matrix[i][index] ? matrix[i][index] : result;
    }
    return result;
}

/*
 * Function: get_max
 * ----------------------------
 * Compute the maximum value of a given matrix, on a given column.
 *
 * matrix: target matrix
 * length: number of rows
 * index: index of the target column
 *
 * result: maximum value of the elements from the indexth column
 */
double get_max(double **matrix, double length, int index) {
    assert(matrix);
    double result = -DBL_MAX;

    for (int i = 0; i < length; ++i) {
        result = result < matrix[i][index] ? matrix[i][index] : result;
    }
    return result;
}

/*
 * Function: normalise
 * -------------------
 * Normalise a given matrix column-wise.
 *
 * matrix: matrix to be normalised
 * rows: number of rows
 * columns: number of columns
 *
 * return: normalised matrix(has to be FREED!), note that the original matrix
 * 		   is not altered by the function so it can be used for the
 * rescale() function
 */
double **normalise(double **matrix, int rows, int columns) {
    double **normalised_matrix = (double **)malloc(rows * sizeof(double *));

    for (int i = 0; i < columns; ++i) {
        double current_min = get_min(matrix, rows, i);
        double current_max = get_max(matrix, rows, i);

        for (int j = 0; j < rows; ++j) {
            if (i == 0) {
                normalised_matrix[j] =
                    (double *)malloc(columns * sizeof(double));
            }
            normalised_matrix[j][i] =
                (matrix[j][i] - current_min) / (current_max - current_min);
        }
    }

    return normalised_matrix;
}

/*
 * Function: rescale
 * -----------------
 * Rescale a column of matrix normalised by the normalise() 
 * function defined above, by overwriting.
 *
 * matrix: *normalised* matrix
 * min: minimum value from the given column
 * max: maximum value from the given column
 * rows: number of rows
 * index: index of the target column
 */
void rescale(double **matrix, double min, double max, int rows, int index) {
    for (int j = 0; j < rows; ++j) {
        matrix[j][index] = matrix[j][index] * (max - min) + min;
    }
}

/*
 * Function: format_nn_features
 * ----------------------------
 * Reformat the given features columns so that each row contains 5 days worth
 * of data making the prediction more accurate at a low "cost".
 *
 * data: data features matrix
 * no_of_rows: number of rows
 * no_of_cols: number of columns
 *
 * return: formatted feautures columns (has to be FREED!)
 */
double **format_nn_features(double **data, int no_of_rows, int no_of_cols) {
    double **data_formatted =
        (double **)calloc(no_of_rows / NO_OF_DAYS, sizeof(double *));

    int row = 0;
    int col = 0;
    int data_row = 0;
    while (data_row < no_of_rows && row < (no_of_rows / NO_OF_DAYS)) {
        if (!data_formatted[row]) {
            data_formatted[row] =
                (double *)calloc(no_of_cols * NO_OF_DAYS, sizeof(double));
        }

        assert(data_formatted[row]);

        for (int i = 0; i < no_of_cols; ++i) {
            data_formatted[row][col + i] = data[data_row][i];
        }

        data_row++;
        col += no_of_cols;
        if ((data_row + 1) % NO_OF_DAYS == 0) {
            data_row++;
            row++;
            col = 0;
        }
    }

    return data_formatted;
}

/*
 * Function: format_targets
 * ------------------------
 * Reformat the target values column so that it includes only every
 * 6th day from that dataset (because of the batching described above).
 *
 * targets: target values matrix where the actual values or on the first column
 * no_of_rows: number of rows
 *
 * return: formatted target values (has to be FREED!)
 */
double **format_targets(double **targets, int no_of_rows) {
    double **targets_formatted =
        (double **)calloc(no_of_rows / NO_OF_DAYS, sizeof(double *));

    int targets_row = 5;
    while (targets_row < (no_of_rows / NO_OF_DAYS) * NO_OF_DAYS) {
        targets_formatted[targets_row / NO_OF_DAYS] =
            (double *)calloc(1, sizeof(double));
        targets_formatted[targets_row / NO_OF_DAYS][0] =
            targets[targets_row][0];
        targets_row += 6;
    }

    return targets_formatted;
}
