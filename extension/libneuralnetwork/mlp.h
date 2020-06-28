#ifndef MLP_H
#define MLP_H

#include <stdbool.h>

typedef struct mlp_layer {
    int num_inputs, num_outputs;
    struct mlp_layer *previous_layer, *next_layer;
    double *outputs;
    double *biases;
    double *errors;
    double **weights;
} Layer;

typedef struct mlp_net {
    struct mlp_layer *input_layer;
    struct mlp_layer *output_layer;
} MLP;

extern double sigmoid(double x);

extern double sig_prime(double x);

extern double relu(double x);

extern double relu_prime(double x);

extern double random_deviation_num();

extern Layer *create_layer(void);

extern void layer_initialise(Layer *layer, int num_outputs,
                             Layer *previous_layer);

extern void back_prop(MLP *mlp, double *target, double learning_rate);

extern void train(MLP *mlp, double **input_vals, int num_inputs,
                  double **targets, double learning_rate, int epochs);

extern double cost(MLP *mlp, double **targets, double **inputs, int no_rows);

extern void layer_free(Layer *layer);

extern void mlp_free(MLP *mlp);

extern MLP *mlp_initialise(int *num_nodes, int num_layers);

extern void output_calc(Layer *layer, bool use_sigmoid);

extern void forward_prop(MLP *mlp, double *input_vals);

#endif
