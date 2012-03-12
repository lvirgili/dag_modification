#ifndef GRAPH
#define GRAPH

#include <stdio.h>
#include <stdlib.h>

int *topological_order, *label;
int count;

static void ts(int **G, int ntasks, int v) {
     int i;
     label[v] = 0;
     for (i = 0; i < ntasks; ++i) {
          if (G[v][i] == 1 && label[i] == -1) {
               ts(G, ntasks, i);
          }
     }
     topological_order[count] = v;
     --count;
}

void gen_ts(int **G, int ntasks) {
     int i;
     topological_order = (int *)malloc(ntasks * sizeof(int));
     label = (int *)malloc(ntasks * sizeof(int));
     count = ntasks - 1;
     for (i = 0; i < ntasks; ++i) {
          topological_order[i] = label[i] = -1;
     }
     for (i = 0; i < ntasks; ++i) {
          if (label[i] == -1) {
               ts(G, ntasks, i);
          }
     }
}

#endif
