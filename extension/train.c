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
#include "crossover.h"
#include "selection.h"
#include "geneticutils.h"
#include "mlp.h"
#include "dataops.h"
#include "managenn.h"

#define MLP_TRAINING_EPOCHS 500
#define VALIDATION_RATIO 0.2

/*
 * Function: iteration_printing
 * ----------------------------
 * Does routine training based on the current genetic state.
 *
 * state: current state of the algorithm
 */
void iteration_printing(GeneticState *state) {
    printf("Working...\n Generation: %d\n", state->generation_number);
    printf("Fitness of the fittest individual so far: %lf, cost: %lf\n",
           state->fittest_individual->fitness,
           1 / state->fittest_individual->fitness);

    printf("Fittest indivdual so far:\n");
    printf("Learning rate: %lf\n", state->fittest_individual->learning_rate);
    printf("Hidden layers: %d\n", state->fittest_individual->hidden_layers);
    printf("Nodes per layer: %d\n", state->fittest_individual->nodes_per_layer);
    printf("-----------------\n");

    printf(
        "Fitness individual of this generation: %lf, cost: "
        "%lf\n",
        state->fittest_individual_currently->fitness,
        1 / state->fittest_individual_currently->fitness);

    printf("Fittest of this generation:\n");
    printf("Learning rate: %lf\n",
           state->fittest_individual_currently->learning_rate);
    printf("Hidden layers: %d\n",
           state->fittest_individual_currently->hidden_layers);
    printf("Nodes per layer: %d\n",
           state->fittest_individual_currently->nodes_per_layer);
    printf("-----------------\n");
}

/*
 * Function terminate_genetic
 * --------------------------
 *  Does the final printing, saves the best neural network so far and
 *  frees the state.
 *
 *  state: current genetic state
 *  targets: *DESCALED* target values
 *  num_targets: number of targets
 */
void terminate_genetic(GeneticState *state, double **targets, int num_targets) {
    // output NN here
    printf(
        "After %d generations, with mutation probability set to %lf, the "
        "fittest individual had a fitness of %lf, and this individual's "
        "cost was: %lf\n",
        state->generation_number, state->mutation_probability,
        state->fittest_individual->fitness,
        1 / state->fittest_individual->fitness);

    // Save NN
    save_nn(state->fittest_individual, "nn.csv", targets, num_targets);
    // free everything
    free_genetic_state(state);
}

/*
 * Function: main
 * --------------
 * This does the training, it takes a CSV with financial data of one company
 * from Yahoo Finance and tries to find the best neural network, adjusting the
 * hyperparmeters as described in the report. The number of epochs 
 * the networks train for and the pecentage of the dataset used for evaluation
 * (that is calculating the fitness for the selection) are both controlled
 * by macros defined at the top of this file. At the end the best neural
 * network is saved to a local file "nn.csv". The following command line
 * arguments are required:
 *
 * dataset_csv          - path to the location of the Yahoo Finance dataset,
 * 						  must have the number of rows >= number of days
 * 						  in a batch (currently set to 6 in dataops.h)
 * 						  * 2, the rows which can't fit in a batch
 * 						  are discarded.
 * number_generations   - the number of generations the genetic algorithm
 * 						  should run for
 * population_size      - the size of the population, how many networks are
 * 						  trained in 1 generation
 * mutation_probability - a float between 0 and 1 that is the chance of 
 * 						  something random happening at crossover, 
 * 						  improves diversity
 */
int main(int argc, char **argv) {
    assert(argc == 5);

    char *filename = argv[1];
    int number_generations = atoi(argv[2]);
    int population_size = atoi(argv[3]);
    double mutation_probability = strtod(argv[4], NULL);
    double (*fitness_function)(MLP *, double **, double **, int) =
        calculate_fitness;

    assert(fitness_function);
    assert(number_generations > 0);
    assert(population_size > 1);
    assert(mutation_probability >= MUTATION_LOWER &&
           mutation_probability <= MUTATION_UPPER);

    srand(time(NULL));

    // load data from CSV
    int no_of_rows = 0;
    const char *columns[] = {"Open", "High", "Low", "Close"};
    int no_of_cols = 4;
    double **data = load_csv(filename, columns, no_of_cols, &no_of_rows);
    double **data_formatted = format_nn_features(data, no_of_rows, no_of_cols);
    free_pointer_matrix((void **)data, no_of_rows);

    // load targets from CSV
    int no_targets = 0;
    const char *targetcols[] = {"Close"};
    int no_target_cols = 1;
    double **targets_aux =
        load_csv(filename, targetcols, no_target_cols, &no_targets);
    double **targets_formatted = format_targets(targets_aux, no_targets);
    free_pointer_matrix((void **)targets_aux, no_targets);

    const int formatted_rows = no_of_rows / NO_OF_DAYS;
    assert(formatted_rows > 5);
    assert(no_targets == no_of_rows);

    // normalise data
    double **denormalised_data = data_formatted;
    data_formatted = normalise(data_formatted, formatted_rows, NO_FEATURES);

    // normalise features
    double **denormalised_targets = targets_formatted;
    targets_formatted = normalise(targets_formatted, formatted_rows, 1);

    //split into training and validation
    const int validation_rows = VALIDATION_RATIO * (double)formatted_rows;
    const int training_rows = formatted_rows - validation_rows;

    double **training_data = data_formatted + validation_rows;
    double **validation_data = data_formatted;

    double **training_targets = targets_formatted + validation_rows;
    double **validation_targets = targets_formatted;

    // assign parameter values to state
    GeneticState *state = create_genetic_state();
    state->mutation_probability = mutation_probability;
    state->fitness_function = fitness_function;

    // population initalisation
    init_population(state, population_size);

    // evolution process
    while (state->generation_number < number_generations) {
        // train networks
        for (int i = 0; i < state->current_generation->population_size; ++i) {
            Chromosome *chr;
            chr = state->current_generation->population[i];
            train(chr->mlp, training_data, training_rows, training_targets,
                  chr->learning_rate, MLP_TRAINING_EPOCHS);
        }

        // apply fitness function to generation
        calculate_fittest(state, validation_targets, validation_data,
                          validation_rows);

        // create new generation
        Generation *generation = create_generation();
        generation->population_size = population_size;
        Chromosome **population =
            (Chromosome **)calloc(population_size, sizeof(Chromosome *));

        // selection & crossover
        Chromosome **parents = get_parents(state, population_size);

        for (int i = 0; i < population_size; ++i) {
            population[i] = crossover(parents[2 * i], parents[2 * i + 1],
                                      mutation_probability);
        }

        iteration_printing(state);
        generation->population = population;
        free(parents);

        // set new generation in state
        free_generation(state->current_generation, state->fittest_individual);
        state->current_generation = generation;
        state->generation_number += 1;
    }

    // free the state and the csv file
    terminate_genetic(state, denormalised_targets, formatted_rows);
    free_pointer_matrix((void **)denormalised_data, formatted_rows);
    free_pointer_matrix((void **)denormalised_targets, formatted_rows);

    free_pointer_matrix((void **)data_formatted, formatted_rows);
    free_pointer_matrix((void **)targets_formatted, formatted_rows);

    return EXIT_SUCCESS;
}
