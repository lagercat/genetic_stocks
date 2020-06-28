#ifndef CSV_H
#define CSV_H

#define STANDARD_ROWS_NUMBER 512
#define MAX_CSV_LINE_LENGTH 256

double **load_csv(const char *filename, const char **columns, int no_of_columns,
                  int *no_of_rows);
#endif
