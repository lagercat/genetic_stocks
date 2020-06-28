#ifndef CREATE_GENETIC_STRUCTURES
#define CREATE_GENETIC_STRUCTURES

extern Chromosome *create_chromosome(void);
extern Generation *create_generation(void);
extern GeneticState *create_genetic_state(void);
extern void init_population(GeneticState *state, int population_size);

extern void free_chromosome(Chromosome *chromosome);
extern void free_generation(Generation *generation,
                            Chromosome *best_chromosome);
extern void free_genetic_state(GeneticState *state);

#endif
