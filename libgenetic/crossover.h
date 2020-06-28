#ifndef CROSSOVER
#define CROSSOVER

extern bool mutate(Chromosome *chromosome, double mutation_probability);
extern Chromosome *crossover(Chromosome *parent1, Chromosome *parent2,
                             double mutation_probability);

#endif
