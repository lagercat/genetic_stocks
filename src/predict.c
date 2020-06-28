#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <assert.h>
#include <limits.h>
#include <string.h>
#include <time.h>

#include "files.h"
#include "structures.h"
#include "createstructures.h"
#include "geneticutils.h"
#include "mlp.h"
#include "dataops.h"
#include "managenn.h"

/*
 * Function: save_prediction
 * -------------------------
 * This function creates a csv file. If actual is true then it creates a file with
 * only 1 column of the prediction. Otherwise it creates 3 columns of: Date, Actual close
 * and Predicted close.
 *
 * actual - if this is to create a graph or an actual prediction
 * predictions - the predictions created
 * file - the file to save the predictions to
 * rows - the number of predictions there are
 */
void save_predictions(bool actual, double **predictions, char file[],
                      int rows) {
    assert(file != NULL);
    FILE *f = fopen(file, "w+");

    if (actual) {
        fprintf(f, "Predictions\n");
        for (int i = 0; i < rows; i++) {
            fprintf(f, "%lf\n", predictions[i][0]);
        }
    } else {
        fprintf(f, "Date,Close,Predictions\n");
        const char *cols[] = {"Close"};
        int rows_data;
        double **og_data = load_csv("misc_csv/data.csv", cols, 1, &rows_data);
        for (int i = 0; i < rows; i++) {
            fprintf(f, "%i,%lf,%lf\n", i, og_data[5 + 6 * i][0],
                    predictions[i][0]);
        }

        free_pointer_matrix((void **)og_data, rows_data);
    }

    fclose(f);
}

/*
 * Function: main
 * --------------
 * Loads a nn and data to be used to create predictions and then 
 * saves those predictions in a file labelled "predictions.csv".
 *
 * file_name - path to data to be predicted, defaults to data.csv
 * load_name - path to the neural network to be loaded
 */
int main(int argc, char **argv) {
    assert(argc == 2 || argc == 3);
    bool actual;
    char *load_name;
    char *file_name;
    if (argc == 3) {
        printf("Predicting using %s\n", argv[1]);
        file_name = argv[1];
        load_name = argv[2];
        actual = true;
    } else {
        printf("Test Predict\n");
        file_name = "misc_csv/data.csv";
        load_name = argv[1];
        actual = false;
    }

    const char *columns[] = {"Open", "High", "Low", "Close"};
    int no_rows = 0;
    double **data = load_csv(file_name, columns, 4, &no_rows);
    double **data_formatted = format_nn_features(data, no_rows, 4);
    free_pointer_matrix((void **)data, no_rows);

    printf("Data loaded...\n");

    int rows = no_rows / NO_OF_DAYS;
    double **normalised = normalise(data_formatted, rows, NO_FEATURES);

    printf("Data normalised...\n");

    double min;
    double max;
    MLP *mlp = load_net(load_name, &min, &max);

    printf("MLP Loaded...\n");

    printf("Predicting...\n");
    double **predictions = (double **)calloc(rows, sizeof(double *));
    for (int i = 0; i < rows; i++) {
        predictions[i] = (double *)calloc(rows, sizeof(double));
    }
    for (int i = 0; i < rows; i++) {
        forward_prop(mlp, normalised[i]);
        predictions[i][0] = mlp->output_layer->outputs[0];
    }

    rescale(predictions, min, max, rows, 0);

    save_predictions(actual, predictions, "predictions.csv", rows);

    printf("Please look at \"predictions.csv\" for the predictions.\n");

    // Free everything
    free_pointer_matrix((void **)data_formatted, rows);
    free_pointer_matrix((void **)predictions, rows);
    free_pointer_matrix((void **)normalised, rows);
    mlp_free(mlp);

    return EXIT_SUCCESS;
}
