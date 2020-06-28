#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <assert.h>

#include "files.h"

/*
 * Function: load_csv
 * ------------------
 * Loads a CSV file with column names on the first row. Assumes that
 * all the given columns can be converted to doubles(!!)
 *
 * filename: path to the csv file
 * columns: array of strings with the names of the columns that are to be
 * 		    loaded (they have to be in the order in which they appear in
 * the CSV file!) no_of_columns: no of columns to be loaded no_of_rows: the
 * function will set the value at this pointer to the number of rows read from
 * the file result: a matrix of doubles where result[i][j] means the value from
 * 		   the ith row and the jth column(as passed in the columns
 * array). It has to be FREED! Both the matrix itself and the rows.
 */
double **load_csv(const char *filename, const char **columns, int no_of_columns,
                  int *no_of_rows) {
    assert(filename);
    assert(columns);
    assert(no_of_rows);

    FILE *file;
    if (!(file = fopen(filename, "r"))) {
        perror("Could not open the given CSV file");
        exit(EXIT_FAILURE);
    }

    int max_rows = STANDARD_ROWS_NUMBER;
    double **result = (double **)calloc(max_rows, sizeof(double *));
    assert(result);

    char line[MAX_CSV_LINE_LENGTH];
    int column_indexes[no_of_columns];
    int rows = 0;

    if (fgets(line, MAX_CSV_LINE_LENGTH, file)) {
        int i = 0;
        int j = 0;
        char *column_name = strtok(line, ",");
        while (column_name) {
            if (j < no_of_columns && strcmp(column_name, columns[j]) == 0) {
                column_indexes[j] = i;
                j++;
            }

            i++;
            column_name = strtok(NULL, ",");
        }
    } else {
        perror("Could not read the column row from the given CSV file");
        exit(EXIT_FAILURE);
    }

    while (fgets(line, MAX_CSV_LINE_LENGTH, file)) {
        int current_col = 0;
        int cell_col = 0;

        if (rows == max_rows) {
            result = realloc(result, 2 * max_rows * sizeof(double *));
            max_rows *= 2;
        }

        result[rows] = (double *)malloc(no_of_columns * sizeof(double));
        assert(result[rows]);
        char *current_cell = strtok(line, ",");
        while (current_cell) {
            if (current_col < no_of_columns &&
                cell_col == column_indexes[current_col]) {
                result[rows][current_col] = strtod(current_cell, NULL);
                current_col++;
            }

            cell_col++;
            current_cell = strtok(NULL, ",");
        }

        rows++;
    }

    fclose(file);
    *no_of_rows = rows;
    return result;
}
