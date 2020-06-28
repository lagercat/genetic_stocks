#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "structures.h"
#include "dataops.h"
#include "assert.h"

/*
 * Function: load_net
 * ------------------
 * Parameters:	filename - the csv file containing a previously
 *						   trained MLP
 *              min - a pointer to a double variable to hold the minimum
 *                    value of the training data for denormalisation
 *
 *              max - a pointer to a double variable to hold the maximum
 *                    value of the training data for denormalisation
 * Loads an MLP's hyperparameters and weights and biases from a csv
 */
MLP *load_net(const char *filename, double *min, double *max) {
    assert(filename != NULL);
    //Open the file
    FILE *file = fopen(filename, "r");
    if (file == NULL) {
        perror("Could not open the given CSV file");
        exit(EXIT_FAILURE);
    }

    //Read the first line
    char c = fgetc(file);
    int count = 0;
    int buffer_size = 256;
    char *buf = calloc(buffer_size + 1, sizeof(char));
    while ((c != EOF) && (c != '\n')) {
        if (count >= buffer_size - 1) {
            buffer_size *= 1;
            buf = realloc(buf, buffer_size);
        }
        buf[count] = c;
        c = fgetc(file);
        count++;
    }

    //Set the min and max values
    *min = atof(strtok(buf, ","));
    *max = atof(strtok(NULL, ","));
    free(buf);

    //Read the second line
    c = fgetc(file);
    count = 0;
    buffer_size = 256;
    buf = calloc(buffer_size + 1, sizeof(char));
    while ((c != EOF) && (c != '\n')) {
        if (count >= buffer_size - 1) {
            buffer_size *= 2;
            buf = realloc(buf, buffer_size);
        }
        buf[count] = c;
        c = fgetc(file);
        count++;
    }
    //Get the number of layers and the number of nodes for each layer
    char layers[count + 1];
    strncpy(layers, buf, count);
    layers[count] = 0;

    char *token = strtok(layers, ",");
    int num_layers = 0;
    int n = 0;
    int *num_nodes = malloc(num_layers * sizeof(int));
    while (token != NULL) {
        if (n >= num_layers - 1) {
            num_layers += 1;
            num_nodes = realloc(num_nodes, num_layers * sizeof(int));
        }
        num_nodes[n++] = atoi(token);
        token = strtok(NULL, ",");
    }
    free(buf);

    //Initialise an MLP with the correct number of layers and nodes
    MLP *mlp_net = mlp_initialise(num_nodes, num_layers);

    free(num_nodes);

    //Read line by line for each layer
    Layer *layer = mlp_net->input_layer->next_layer;
    int i;
    char *line;
    for (i = 1; i < num_layers; i++) {
        count = 0;
        buffer_size = 256;
        c = fgetc(file);
        line = malloc(buffer_size);
        while ((c != EOF) && (c != '\n')) {
            if (count >= buffer_size - 1) {
                buffer_size *= 2;
                line = realloc(line, buffer_size);
            }
            line[count] = c;
            c = fgetc(file);
            count++;
        }
        char weights[count + 1];
        strncpy(weights, line, count);
        weights[count] = 0;
        //Assign the weights for each layer
        char *weight_tok = strtok(weights, ",");
        int j;
        int k;
        for (j = 0; j < layer->num_outputs; j++) {
            for (k = 0; k < layer->num_inputs; k++) {
                layer->weights[k][j] = atof(weight_tok);
                weight_tok = strtok(NULL, ",");
            }
        }
        layer = layer->next_layer;
        free(line);
    }

    layer = mlp_net->input_layer->next_layer;

    //Read line by line again for each layer
    for (i = 1; i < num_layers; i++) {
        count = 0;
        buffer_size = 256;
        line = calloc(buffer_size + 1, sizeof(char));
        c = fgetc(file);
        while ((c != EOF) && (c != '\n')) {
            if (count >= buffer_size - 1) {
                buffer_size *= 2;
                line = realloc(line, buffer_size);
            }
            line[count] = c;
            c = fgetc(file);
            count++;
        }

        //Assign the biases for each layer
        char *biases = strtok(line, ",");
        int j;
        for (j = 0; j < layer->num_outputs; j++) {
            layer->biases[j] = atof(biases);
            biases = strtok(NULL, ",");
        }
        layer = layer->next_layer;
        free(line);
    }

    fclose(file);
    return mlp_net;
}

/*
 * Function: save_nn
 * -----------------
 * Parameters:	c - chromosome that contains the mlp to be saved
 *				name - the name of the file that will be saved to
 *				targets - the targets of the nn required for min and max
 *				no_targets - the number of targets required for min and max
 *
 * This function takes in a NN and will save its contents each on a seperate line:
 * 1. min, max
 * 2. Number of nodes in each layer
 * 3. All the weights, each layer on a seperate line
 * 4. All the biases, each layer on a seperate line
 */
void save_nn(Chromosome *c, char name[], double **targets, int no_targets) {
    FILE *nn;
    nn = fopen(name, "w+");

    // Print the min and max value in that order
    fprintf(nn, "%lf,%lf\n", get_min(targets, no_targets, 0),
            get_max(targets, no_targets, 0));

    // Print no. nodes in each layer
    fprintf(nn, "%i,", c->mlp->input_layer->num_outputs);
    for (int i = 0; i < c->hidden_layers; i++) {
        fprintf(nn, "%i,", c->nodes_per_layer);
    }
    fprintf(nn, "%i\n", c->mlp->output_layer->num_outputs);

    // Print the weights in each layer on seperate lines
    Layer *curr = c->mlp->input_layer->next_layer;
    while (curr) {
        for (int j = 0; j < curr->num_outputs; j++) {
            for (int i = 0; i < curr->num_inputs; i++) {
                if ((j == curr->num_outputs - 1) &&
                    (i == curr->num_inputs - 1)) {
                    fprintf(nn, "%lf", curr->weights[i][j]);
                } else {
                    fprintf(nn, "%lf,", curr->weights[i][j]);
                }
            }
        }
        fprintf(nn, "\n");
        curr = curr->next_layer;
    }

    // Print the biases in each layer on seperate lines
    curr = c->mlp->input_layer->next_layer;
    while (curr) {
        for (int j = 0; j < curr->num_outputs; j++) {
            if (j == curr->num_outputs - 1) {
                fprintf(nn, "%lf", curr->biases[j]);
            } else {
                fprintf(nn, "%lf,", curr->biases[j]);
            }
        }
        fprintf(nn, "\n");
        curr = curr->next_layer;
    }

    fclose(nn);
}
