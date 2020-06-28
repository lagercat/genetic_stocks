#include <stdlib.h>
#include <assert.h>

#include "structures.h"
#include "geneticutils.h"

/*
 * Function: create_chromosome
 * ---------------------------
 *  Returns a heap-allocated chromosome, all attributes of which are 0/NULL
 */
Chromosome *create_chromosome(void) {
    Chromosome *chromosome = calloc(1, sizeof(Chromosome));
    assert(chromosome);

    return chromosome;
}

/*
 * Function: create_generation
 * ---------------------------
 *  Returns a heap-allocated generation, all attributes of which are 0/NULL
 */
Generation *create_generation(void) {
    Generation *generation = calloc(1, sizeof(Generation));
    assert(generation);

    return generation;
}

/*
 * Function: create_genetic_state
 * ------------------------------
 *  Returns a heap-allocated state for the genetic algorithm, initialised to
 *  0/NULL.
 */
GeneticState *create_genetic_state(void) {
    GeneticState *state = calloc(1, sizeof(GeneticState));
    assert(state);

    return state;
}

/*
 * Function: init_population
 * -------------------------
 * Initialises a population(used for the very first generation).
 *
 * state: the genetic state where the population should be attributed
 * population_size: size of the new population
 */
void init_population(GeneticState *state, int population_size) {
    assert(state);

    state->current_generation = create_generation();
    Chromosome **new_population =
        (Chromosome **)malloc(population_size * sizeof(Chromosome *));
    assert(new_population);

    for (int i = 0; i < population_size; ++i) {
        new_population[i] = create_chromosome();
        new_population[i]->learning_rate =
            double_rand_interval(LEARNING_RATE_LOWER, LEARNING_RATE_UPPER);
        new_population[i]->hidden_layers =
            int_rand_interval(HIDDEN_LAYERS_LOWER, HIDDEN_LAYERS_UPPER);

        new_population[i]->nodes_per_layer =
            int_rand_interval(NODES_PER_LAYER_LOWER, NODES_PER_LAYER_UPPER);

        const int hidden_layers = new_population[i]->hidden_layers;
        int nodes[HIDDEN_LAYERS_UPPER + 2];
        nodes[0] = NO_FEATURES;
        for (int j = 1; j <= hidden_layers; ++j) {
            nodes[j] = new_population[i]->nodes_per_layer;
        }
        nodes[hidden_layers + 1] = NO_OUTPUTS;
        new_population[i]->mlp =
            mlp_initialise(nodes, new_population[i]->hidden_layers + 2);
    }

    state->current_generation->population_size = population_size;
    state->current_generation->population = new_population;
}

/*
 * Function: free_chromosome
 * -------------------------
 *  Removes the given chromosome from the heap
 */
void free_chromosome(Chromosome *chromosome) {
    if (chromosome) {
        mlp_free(chromosome->mlp);
        free(chromosome);
    }
}

/*
 * Function: free_generation
 * -------------------------
 *  Removes the given generation from the heap.
 */
void free_generation(Generation *generation, Chromosome *best_chromosome) {
    if (generation) {
        if (generation->population) {
            for (int i = 0; i < generation->population_size; ++i) {
                if (generation->population[i] != best_chromosome) {
                    free_chromosome(generation->population[i]);
                }
            }
            free(generation->population);
        }

        free(generation);
    }
}

/*
 * Function: free_genetic_state
 * ----------------------------
 *  Removes the given genetic algorithm state from the heap
 */
void free_genetic_state(GeneticState *state) {
    if (state) {
        free_generation(state->current_generation, state->fittest_individual);
        free_chromosome(state->fittest_individual);
        free(state);
    }
}
