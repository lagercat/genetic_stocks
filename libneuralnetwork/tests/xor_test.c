#include <stdio.h>
#include <stdlib.h>

#include "mlp.h"

int main(void) {
    int layers[] = {2, 3, 1};
    MLP *net = mlp_initialise(layers, 3);

    double input1[] = {0, 0};
    double input2[] = {0, 1};
    double input3[] = {1, 0};
    double input4[] = {1, 1};

    double *inputs[] = {input1, input2, input3, input4};

    double output1[] = {0};
    double output2[] = {1};
    double output3[] = {1};
    double output4[] = {0};

    double *outputs[] = {output1, output2, output3, output4};

    printf("Training...\n");
    train(net, inputs, 4, outputs, 1.0, 25000);

    printf("Predicting...\n");
    forward_prop(net, input1);
    printf("Expecting: %f\n", output1[0]);
    printf("Result: %f\n", net->output_layer->outputs[0]);

    printf("Predicting...\n");
    forward_prop(net, input2);
    printf("Expecting: %f\n", output2[0]);
    printf("Result: %f\n", net->output_layer->outputs[0]);

    mlp_free(net);

    return EXIT_SUCCESS;
}
