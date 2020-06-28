#include <limits.h>
#include <assert.h>
#include <stdlib.h>
#include <stdio.h>

#include "mlp.h"
#include "structures.h"
#include "createstructures.h"
#include "geneticutils.h"
#include "float.h"

/*
 * Function: calculate_fitness
 * ---------------------------
 *  Calculates the fitness of one particular MLP
 *  by just inverting the cost error so that the smaller
 *  the error, the greater the fitness.
 *
 *  mlp: MLP to calculate the fitness for
 *  targets: target outputs
 *  inputs: inputs from the dataset
 *  no_inputs: number of the given inputs
 *
 *  return: fitness of the MLP
 */
double calculate_fitness(MLP *mlp, double **targets, double **inputs,
                         int no_inputs) {
    return 1 / cost(mlp, targets, inputs, no_inputs);
}

/*
 * Function: calculate_fittest
 * ---------------------------
 * Calculates the fittest chromosome of the given state,
 * updating all the 'fittest' attributes of both the state
 * and the generation (while FREEING the old fittest_individual
 * if that's necessary).
 *
 * state: state to find the fittest chromosome for
 */
void calculate_fittest(GeneticState *state, double **targets, double **inputs,
                       int no_inputs) {
    assert(state);
    double (*fitness_function)(MLP *, double **, double **, int) =
        state->fitness_function;
    Generation *generation = state->current_generation;

    double max_fitness = -DBL_MAX;

    for (int i = 0; i < generation->population_size; ++i) {
        generation->population[i]->fitness = fitness_function(
            generation->population[i]->mlp, targets, inputs, no_inputs);
        if (generation->population[i]->fitness > max_fitness) {
            max_fitness = generation->population[i]->fitness;
            generation->fittest = generation->population[i];
        }
    }

    assert(generation->fittest);
    state->fittest_individual_currently = generation->fittest;
    if (!state->fittest_individual ||
        generation->fittest->fitness > state->fittest_individual->fitness) {
        free_chromosome(state->fittest_individual);
        state->fittest_individual = generation->fittest;
    }
}

/*
 * Function: compare_fitness
 * -------------------------
 * Compares 2 Chromosomes based on their fitness value.
 *
 * c1, c2: Chromosomes to be compared
 *
 * return: It returns a value < 0 if c1 < c2, == 0 if c1 = c2
 * 		   and > 0 if c1 > c2.
 */
static int compare_fitness(const void *c1, const void *c2) {
    return ((Chromosome *)c1)->fitness - ((Chromosome *)c2)->fitness;
}

/*
 * Function: get_parents
 * ---------------------
 * Gets the parents for the next generation based on the roulette wheel
 * method.
 *
 * state: genetic state for which the parents are fetched
 * number_of_pairs: the number of pairs of parents which should be fetched
 *
 * return: a malloced array of size 2 * number_of_pairs where for example
 * 		   the first pair of parents is at the indexes 0 and 1, the
 * second at 2, 3 etc.
 *
 * N.B: The return array should be FREED but not its elements since
 * they are pointers to the same structs used in state. As long as
 * the current generation is FREED everything will be ok. Also, the function
 * changes the order of the elements in state->current_generation->population
 * by sorting them.
 */
Chromosome **get_parents(GeneticState *state, int number_of_pairs) {
    assert(state);
    assert(number_of_pairs > 0);

    Chromosome **result =
        (Chromosome **)malloc(2 * number_of_pairs * sizeof(Chromosome *));
    assert(result);

    qsort(state->current_generation->population,
          state->current_generation->population_size, sizeof(Chromosome *),
          compare_fitness);

    double probabilities[state->current_generation->population_size];
    int n = state->current_generation->population_size;
    double sum = 0;

    for (int i = 0; i < n; ++i) {
        sum += state->current_generation->population[i]->fitness;
    }

    double sum_of_probs = 0;
    for (int i = 0; i < n; ++i) {
        probabilities[i] =
            sum_of_probs +
            state->current_generation->population[i]->fitness / sum;
        sum_of_probs = probabilities[i];
    }

    for (int i = 0; i < 2 * number_of_pairs; ++i) {
        int index;
        double random_rank = double_rand_interval(0, 1);

        for (index = 0; index < n && probabilities[index] < random_rank;
             ++index)
            ;
        if (index == n) index--;

        result[i] = state->current_generation->population[index];
    }

    return result;
}
