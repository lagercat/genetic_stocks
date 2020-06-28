#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include <float.h>

#include "testutils.h"
#include "dataops.h"

void test_min() {
    double **test1 = (double **)calloc(2, sizeof(double *));
    test1[0] = (double *)calloc(1, sizeof(double));
    test1[1] = (double *)calloc(1, sizeof(double));

    test1[0][0] = -2;
    test1[1][0] = 2;

    testdouble(get_min(test1, 2, 0), -2, "Test min 1");

    free(test1[0]);
    free(test1[1]);
    free(test1);

    double **test2 = (double **)calloc(3, sizeof(double *));
    test2[0] = (double *)calloc(1, sizeof(double));
    test2[1] = (double *)calloc(1, sizeof(double));
    test2[2] = (double *)calloc(1, sizeof(double));

    test2[0][0] = 0.1;
    test2[1][0] = 0.03;
    test2[2][0] = -0.00001;

    testdouble(get_min(test2, 3, 0), -0.00001, "Test min 2");

    free(test2[0]);
    free(test2[1]);
    free(test2[2]);
    free(test2);
}

void test_max() {
    double **test1 = (double **)calloc(1, sizeof(double *));
    test1[0] = (double *)calloc(1, sizeof(double));

    test1[0][0] = -DBL_MAX;

    testdouble(get_min(test1, 1, 0), -DBL_MAX, "Test max 1");

    free(test1[0]);
    free(test1);

    double **test2 = (double **)calloc(3, sizeof(double *));
    test2[0] = (double *)calloc(1, sizeof(double));
    test2[1] = (double *)calloc(1, sizeof(double));
    test2[2] = (double *)calloc(1, sizeof(double));

    test2[0][0] = 0;
    test2[1][0] = DBL_MAX - 1;
    test2[2][0] = -0.00001;

    testdouble(get_max(test2, 3, 0), DBL_MAX - 1, "Test max 2");

    free(test2[0]);
    free(test2[1]);
    free(test2[2]);
    free(test2);
}

int main(void) {
    test_min();
    test_max();
    return EXIT_SUCCESS;
}
