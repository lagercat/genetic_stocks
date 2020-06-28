#ifndef SELECTION_H
#define SELECTION_H

extern double calculate_fitness(MLP *mlp, double **targets, double **inputs,
                                int no_inputs);

extern void calculate_fittest(GeneticState *state, double **targets,
                              double **inputs, int no_inputs);

extern Chromosome **get_parents(GeneticState *state, int number_of_pairs);

#endif
