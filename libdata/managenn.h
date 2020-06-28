#ifndef MANAGE_NN_H
#define MANAGE_NN_H

extern MLP *load_net(const char *filename, double *min, double *max);

extern void save_nn(Chromosome *c, char name[], double **targets,
                    int no_targets);

#endif
