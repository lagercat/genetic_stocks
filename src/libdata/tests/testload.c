#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>

#include "structures.h"
#include "createstructures.h"
#include "mlp.h"
#include "dataops.h"
#include "managenn.h"

void test_load_save(void) {
    printf("Testing save and load...\n");
    int num_layers = 5;
    int *num_nodes = malloc(num_layers * sizeof(int));

    for (int i = 0; i < num_layers; i++) {
        num_nodes[i] = 4;
    }

    MLP *mlp_1 = mlp_initialise(num_nodes, num_layers);
    Chromosome *chr = create_chromosome();
    chr->mlp = mlp_1;
    chr->hidden_layers = 3;
    chr->nodes_per_layer = 4;

    double tar1[] = {1, 2, 3};
    double tar2[] = {3, 4, 5};
    double tar3[] = {7, 8, 10};

    double *targs[] = {tar1, tar2, tar3};

    save_nn(chr, "nn.csv", targs, 3);

    double d1;
    double d2;

    MLP *mlp_2 = load_net("nn.csv", &d1, &d2);
    int i;
    Layer *layer1 = mlp_1->input_layer->next_layer;
    Layer *layer2 = mlp_2->input_layer->next_layer;
    for (i = 1; i < num_layers; i++) {
        bool same = true;
        int k;
        int j;
        //Test the weights for each layer
        for (k = 0; k < layer1->num_inputs; k++) {
            for (j = 0; j < layer1->num_outputs; j++) {
                same = same ^ (layer1->weights[k][j] == layer2->weights[k][j]);
            }
        }
        //Test the biases for each layer
        for (j = 0; j < layer1->num_outputs; j++) {
            same = same ^ (layer1->biases[j] == layer2->biases[j]);
        }
        if (same) {
            printf("Layer %i matches original\n", i);
        } else {
            printf("Layer %i does not match original\n", i);
        }
        layer1 = layer1->next_layer;
        layer2 = layer2->next_layer;
    }

    free(num_nodes);
    free_chromosome(chr);
    mlp_free(mlp_2);
}

int main(void) {
    test_load_save();
    return EXIT_SUCCESS;
}
