#include "mlp.h"

#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <assert.h>
#include <string.h>

#define MEAN 0
#define STD_DEV (4 / 3)

/*
 * Function: activation functions
 * ------------------------------
 * Parameters:	x - in all function x is the variable being passed in
 *
 * Two sets of function: the activation function and its derivative
 * for feedforward and backpropagation
 */
double sigmoid(double x) { return 1 / (1 + exp(x * -1)); }

double sigmoid_prime(double x) { return x * (1 - x); }

double relu(double x) { return x >= 0 ? x : 0.05 * x; }

double relu_prime(double x) { return x >= 0 ? 1 : 0.05; }

/*
 * Function: random_deviation_num
 * --------------------
 * Parameters:	none
 *
 * Utility function for getting random floats for weight init
 */
double random_deviation_num() {
    // Generate 2 random numbers between 0 and 1
    double x1 = ((double)rand() + 1) / ((double)RAND_MAX + 1);
    double x2 = ((double)rand() + 1) / ((double)RAND_MAX + 1);
    // Transform randoms into a normally distributed random value
    double normal_rand = sqrt(-2 * log(x1)) * cos(2 * M_PI * x2);
    return normal_rand * STD_DEV + MEAN;
}

/*
 * Function: create_layer
 * ----------------------
 * Parameters: none
 * Creates a layer allocation on the heap
 */
Layer *create_layer(void) {
    Layer *l = calloc(1, sizeof(Layer));
    if (!l) {
        perror("Memory allocation failed");
        exit(EXIT_FAILURE);
    }
    return l;
}

/*
 * Function: layer_initialise
 * --------------------------
 * Parameters:	layer - layer in question being initialised
 *				num_outputs - the number of outputs this layer will
 *have previous_layer - the previous layer to be linked
 *
 * Initialises a layer using the number of outputs, the previous layer and
 * the blank layer itself
 */
void layer_initialise(Layer *layer, int num_outputs, Layer *previous_layer) {
    layer->num_outputs = num_outputs;
    layer->outputs = calloc(num_outputs, sizeof(double));
    if (!layer->outputs) {
        perror("Memory allocation failure");
        exit(EXIT_FAILURE);
    }

    // If it is the input layer than it has no weights or biases and so this
    // must be checked first
    if (previous_layer) {
        layer->num_inputs = previous_layer->num_outputs;
        layer->previous_layer = previous_layer;
        previous_layer->next_layer = layer;
        layer->biases = calloc(num_outputs, sizeof(double));
        layer->errors = calloc(num_outputs, sizeof(double));
        layer->weights = calloc(layer->num_inputs, sizeof(double *));

        if (!layer->weights || !layer->errors || !layer->biases) {
            perror("Memory allocation failure");
            exit(EXIT_FAILURE);
        }

        int i;
        for (i = 0; i < layer->num_inputs; i++) {
            layer->weights[i] = malloc(num_outputs * sizeof(double));
            if (!layer->weights[i]) {
                perror("Memory allocation failure");
                exit(EXIT_FAILURE);
            }
            int j;
            for (j = 0; j < layer->num_outputs; j++) {
                layer->weights[i][j] = random_deviation_num();
            }
        }
    }
}

/*
 * Function: back_prop
 * -------------------
 * Parameters: 	mlp - MLP being trained with back propagation
 * 				target - target values for this iteration of
 * training learning_rate - hyperparameter for the algorithm
 *
 * Backpropagation algorithm implemented
 * Takes in the network, the target values and the learning rate
 * Then depending on the layer will use relu prime or sigmoid prime
 */
void back_prop(MLP *mlp, double *target, double learning_rate) {
    assert(mlp != NULL);
    assert(target != NULL);
    // We first have to work out the change in weights for each weight and
    // then we need to update the weights
    Layer *output_l = mlp->output_layer;

    // Compute the errors for the output layer using ReLU prime
    for (int i = 0; i < output_l->num_outputs; i++) {
        output_l->errors[i] = relu_prime(output_l->outputs[i]) *
                              (target[i] - output_l->outputs[i]);
    }

    // Then compute the errors for each previous layer using sigmoid prime
    Layer *current_l = output_l->previous_layer;
    while (current_l != mlp->input_layer) {
        for (int i = 0; i < current_l->num_outputs; i++) {
            double delta_sum = 0;
            for (int j = 0; j < current_l->next_layer->num_outputs; j++) {
                delta_sum += current_l->next_layer->weights[i][j] *
                             current_l->next_layer->errors[j];
            }
            current_l->errors[i] =
                sigmoid_prime(current_l->outputs[i]) * delta_sum;
        }
        current_l = current_l->previous_layer;
    }

    // Then go back through the network and update the weights and biases
    current_l = output_l;
    while (current_l != mlp->input_layer) {
        for (int i = 0; i < current_l->num_inputs; i++) {
            for (int j = 0; j < current_l->num_outputs; j++) {
                current_l->weights[i][j] +=
                    learning_rate * current_l->errors[j] *
                    current_l->previous_layer->outputs[i];
            }
        }

        for (int i = 0; i < current_l->num_outputs; ++i) {
            current_l->biases[i] += learning_rate * current_l->errors[i];
        }

        current_l = current_l->previous_layer;
    }
}

/*
 * Function: output_calc
 * ---------------------
 * Parameters:	layer - layer to be fedforward and calculate all the outputs
 *				use_sigmoid - if the activation function will be ReLU
 *or sigmoid Feedforward for one layer using either sigmoid or ReLU depending on
 *the layer
 */
void output_calc(Layer *layer, bool use_sigmoid) {
    assert(layer != NULL);
    int j;
    for (j = 0; j < layer->num_outputs; j++) {
        double sum = 0;
        int i;
        for (i = 0; i < layer->num_inputs; i++) {
            sum += layer->weights[i][j] * layer->previous_layer->outputs[i];
        }
        if (use_sigmoid) {
            layer->outputs[j] = sigmoid(layer->biases[j] + sum);
        } else {
            layer->outputs[j] = relu(layer->biases[j] + sum);
        }
    }
}

/*
 * Funciton: forward_prop
 * ----------------------
 * Parameters:	mlp - entire multi-layer perceptron network
 *				input_vals - the inputs to the first layer
 *
 * Feedforward for entire network
 * Takes in the network and the inputs to be fed through
 */
void forward_prop(MLP *mlp, double *input_vals) {
    assert(mlp != NULL);
    assert(input_vals != NULL);
    int i;
    for (i = 0; i < mlp->input_layer->num_outputs; i++) {
        mlp->input_layer->outputs[i] = input_vals[i];
    }
    Layer *curr = mlp->input_layer->next_layer;
    while (curr) {
        output_calc(curr, curr != mlp->output_layer);
        curr = curr->next_layer;
    }
}

/*
 * Function: train
 * ---------------
 * Parameters:	mlp - network being used for training
 * input_vals - the entire dataset for training
 * num_inputs - the number of inputs
 * targets - the entire dataset for the target values
 * learning_rate - hyperparameter for back propagation
 * epochs - the number of training iterations
 *
 * Training function
 * Takes in the network to be trained, the 2d array of training data, the number
 * of input sets and the target values for each training set
 */
void train(MLP *mlp, double **input_vals, int num_inputs, double **targets,
           double learning_rate, int epochs) {
    assert(mlp != NULL);
    assert(input_vals != NULL);
    assert(targets != NULL);
    for (int i = 0; i < epochs; i++) {
        // printf("Epoch: %i\n", (i + 1));
        for (int j = 0; j < num_inputs; j++) {
            forward_prop(mlp, input_vals[j]);
            back_prop(mlp, targets[j], learning_rate);
            // printf("  Input: %i\n", j);
            // printf("	Output: %f, Expected: %f\n",
            // mlp->output_layer->outputs[0], targets[j][0]);
        }
        // printf("  Error: %f\n", cost(mlp, targets, input_vals, num_inputs));
    }
}

/*
 * Function: cost
 * --------------
 * Parameters:	mlp - the MLP whose error is being calculated
 *				targets - targets to compare the MLP outputs to
 *				inputs - the inputs required to compare to the
 *targets no_rows - integer to use the loop for number of outputs
 *
 * Cost function for the network given the desired targets using MSE
 */
double cost(MLP *mlp, double **targets, double **inputs, int no_rows) {
    assert(mlp != NULL);
    assert(targets != NULL);
    assert(inputs != NULL);
    double error = 0;
    for (int j = 0; j < no_rows; j++) {
        forward_prop(mlp, inputs[j]);
        for (int i = 0; i < mlp->output_layer->num_outputs; i++) {
            error += (targets[j][i] - mlp->output_layer->outputs[i]) *
                     (targets[j][i] - mlp->output_layer->outputs[i]);
        }
    }
    error *= 0.5 / (double)no_rows;
    return error;
}

/*
 * Function: layer_free
 * --------------------
 * Parameters:	layer - the layer to be freed from the heap
 *
 * Utility functions to free the layer off the heap
 */
void layer_free(Layer *layer) {
    assert(layer != NULL);
    int i;
    for (i = 0; i < layer->num_inputs; i++) {
        free(layer->weights[i]);
    }
    free(layer->weights);
    free(layer->biases);
    free(layer->errors);
    free(layer->outputs);
    free(layer);
}

/*
 * Function: mlp_free
 * -----------------
 * Parameters:	mlp - the MLP to be freed
 *
 * Frees an MLP off of the heap
 */

void mlp_free(MLP *mlp) {
    Layer *curr = mlp->input_layer;
    Layer *next;
    while (curr) {
        next = curr->next_layer;
        layer_free(curr);
        curr = next;
    }
    free(mlp);
}

/*
 * Function: mlp_initialise
 * ------------------------
 * Parameters:	num_nodes - the nodes in each layer of the MLP
 *				num_layers - the number of layers in the whole
 *MLP
 *
 * Given a list of the number of nodes in each layer including the input and
 * output, and the number of layers, it initialises a blank network on the heap
 */
MLP *mlp_initialise(int *num_nodes, int num_layers) {
    assert(num_nodes != NULL);
    MLP *mlp_net = malloc(sizeof(MLP));
    if (!mlp_net) {
        perror("Memory allocation fail");
        exit(EXIT_FAILURE);
    }
    mlp_net->input_layer = create_layer();
    layer_initialise(mlp_net->input_layer, num_nodes[0], NULL);
    Layer *prev = mlp_net->input_layer;
    int i;
    for (i = 1; i < num_layers; i++) {
        Layer *current = create_layer();
        layer_initialise(current, num_nodes[i], prev);
        prev = current;
    };

    mlp_net->output_layer = prev;
    return mlp_net;
}
