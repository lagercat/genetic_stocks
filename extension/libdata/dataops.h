#ifndef DATA_OPS_H
#define DATA_OPS_H

#define NO_OF_DAYS 6

extern double get_min(double **matrix, double length, int index);

extern double get_max(double **matrix, double length, int index);

extern double **normalise(double **matrix, int rows, int columns);

extern void rescale(double **matrix, double min, double max, int rows,
                    int index);

extern double **format_nn_features(double **data, int no_of_rows,
                                   int no_of_cals);

extern double **format_targets(double **targets, int no_of_rows);

#endif
