#ifndef GENETIC_STRUCTURES
#define GENETIC_STRUCTURES

// Define boundaries for Neural Network genes
#define MUTATION_LOWER 0
#define MUTATION_UPPER 1

#define LEARNING_RATE_LOWER 0.001
#define LEARNING_RATE_UPPER 0.99

#define HIDDEN_LAYERS_LOWER 1
#define HIDDEN_LAYERS_UPPER 10

#define NODES_PER_LAYER_LOWER 5
#define NODES_PER_LAYER_UPPER 60

#define NO_FEATURES 20
#define NO_OUTPUTS 1

#include "mlp.h"

/*
 * typedef struct: chromosome
 * --------------------------
 * A structure representing an individual in a population
 * fitness - the fitness of the individual calculated using the fitness function
 * learning_rate - the learning rate hyperparameter for the mlp network
 * hidden_layers - the number of hidden layers in the mlp network. This
 * should be between 2 and 10 nodes_per_layer - number of nodes in each hidden
 * layer mlp_network - the mlp network for the individual
 */
typedef struct chromosome {
    double fitness;
    double learning_rate;
    int hidden_layers;
    int nodes_per_layer;
    MLP *mlp;
} Chromosome;

/*
 * typedef struct: generation
 * -------------------------
 * A structure representing a generation (iteration) of the algorithm
 * population_size - the size of the population
 * fittest - the fittest indivual from the current generation
 * population - a list of chromosomes representing the individuals of the
 * population
 */
typedef struct generation {
    int population_size;
    Chromosome *fittest;
    Chromosome **population;
} Generation;

/*
 * typedef struct: genetic_algorithm_state
 * ---------------------------------------
 * A structure storing the current state of the genetic algorithm
 * generation_number - the number of the current iteration of the algorithm
 * mutation_probability - the probability (between 0 and 1) of mutation occuring
 * 						  during crossover
 * fittest_individual - the fittest individual made by the
 * 						algorithm so far
 * fittest_individual_currently - the fittest individual from the current
 * 								  generation
 * fitness_function - a function pointer to the function used
 * to calculate the fitness of individuals current_generation - the current
 * generation of mlp networks
 */
typedef struct genetic_algorithm_state {
    int generation_number;
    double mutation_probability;
    Chromosome *fittest_individual;
    Chromosome *fittest_individual_currently;
    double (*fitness_function)(MLP *, double **, double **, int);
    Generation *current_generation;
} GeneticState;

#endif
