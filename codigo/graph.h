#ifndef GRAPH
#define GRAPH

#include <stdio.h>
#include <stdlib.h>

int **paths;
int *topological_order, *label;
int count, pathid;

static void ts(int **G, int ntasks, int v);
static void gen_ts(int **G, int ntasks);
static void find_path(int **G, int ntasks, int vertex, int pos, int *curr);
void gen_paths(int **G, int ntasks);

#endif
