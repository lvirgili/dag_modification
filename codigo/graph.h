#ifndef GRAPH
#define GRAPH

#include <stdio.h>
#include <stdlib.h>

int *topological_order, *label;
int count;

static void ts(int **G, int ntasks, int v);
void gen_ts(int **G, int ntasks);

#endif
