#ifndef GRAPH
#define GRAPH

#include <stdio.h>
#include <stdlib.h>

int **paths;
int *path_weight;
int pathid;

void ts(int **G, int ntasks, int v);
void gen_ts(int **G, int ntasks);
void mdfDagOpc3(int **G, int *S, double *TV, int ntasks, int nvm);

#endif
