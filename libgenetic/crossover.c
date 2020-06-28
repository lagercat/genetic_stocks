#include <stdlib.h>
#include <stdbool.h>
#include <assert.h>

#include "structures.h"
#include "createstructures.h"
#include "geneticutils.h"
#include "mlp.h"

/*
 * Function: mutate
 * ----------------
 *  Parameter: chromosome - the chromosome to be mutated
 *  	mutation_probability - the chance (from 0 to 1) of the chromosome being
 *mutated. Returns: true iff the chromosome is mutated.
 *
 *	The chromosome is mutated by assigning a random value between 0 and 1 to
 *the learning rate, and swapping two random positions in the number of nodes
 *per layer array.
 */
bool mutate(Chromosome *chromosome, double mutation_probability) {
    if (double_rand_interval(0, 1) < mutation_probability) {
        // assign random value (within normal range) to learning rate
        chromosome->learning_rate =
            double_rand_interval(LEARNING_RATE_LOWER, LEARNING_RATE_UPPER);

        // flip a coin to determine whether to alter hidden layers or nodes per
        // layer
        if (double_rand_interval(0, 1) < 0.5) {
            // set random value to hidden_layers
            chromosome->hidden_layers =
                int_rand_interval(HIDDEN_LAYERS_LOWER, HIDDEN_LAYERS_UPPER);
        } else {
            chromosome->nodes_per_layer =
                int_rand_interval(NODES_PER_LAYER_LOWER, NODES_PER_LAYER_UPPER);
        }

        return true;
    }

    return false;
}

/*
 * Function: crossover
 * -------------------
 *  Parameter: parent1 - the first parent chromosome to be used in the crossover
 *  		parent2 - the second parent chromosome to be used in the
 * crossover mutation_probability - the chance of the child being mutation (from
 * 0 to 1) Returns: A child chromosome containing attributes from both parent
 * chromosomes
 *
 *  The function uses uniform crossover for the learning rate and number of
 * hidden layers, but for the number of nodes per layer it then uses an
 * adaptation of the single point crossover (adapted to accommodate the fact
 * that the parents could have a different number of hidden layers).
 */
Chromosome *crossover(Chromosome *parent1, Chromosome *parent2,
                      double mutation_probability) {
    assert(parent1 && parent2);  // checking the pointers aren't NULL

    Chromosome *child = create_chromosome();

    // set learning rate
    child->learning_rate = double_rand_interval(0, 1) < 0.5
                               ? parent1->learning_rate
                               : parent2->learning_rate;

    // set number of hidden layers
    child->hidden_layers = double_rand_interval(0, 1) < 0.5
                               ? parent1->hidden_layers
                               : parent2->hidden_layers;

    // set number of nodes per layer
    child->nodes_per_layer = double_rand_interval(0, 1) < 0.5
                                 ? parent1->nodes_per_layer
                                 : parent2->nodes_per_layer;

    mutate(child, mutation_probability);

    const int hidden_layers = child->hidden_layers;
    int nodes[HIDDEN_LAYERS_UPPER + 2];
    nodes[0] = NO_FEATURES;
    for (int j = 1; j <= hidden_layers; ++j) {
        nodes[j] = child->nodes_per_layer;
    }
    nodes[hidden_layers + 1] = NO_OUTPUTS;

    child->mlp = mlp_initialise(nodes, hidden_layers + 2);
    return child;
}
