#include "graph.h"

void ts(int **G, int ntasks, int v) {
     int i;
     label[v] = 0;
     for (i = 0; i < ntasks; ++i) {
          if (G[v][i] > 0 && label[i] == -1) {
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

void find_path(int **G, int ntasks, int vertex, int pos, int *curr) {
     int i, flag = 0;
     curr[pos] = vertex;
     ++pos;
     for (i = 0; i < ntasks; ++i) {
          if (G[vertex][i] > 0) {
               flag = 1;
               find_path(G, ntasks, i, pos, curr);
          }
     }
     if (flag == 0) {
          for (i = 0; i < ntasks; ++i) {
               paths[pathid][i] = curr[i];
          }
          ++pathid;
     }
}

void gen_paths(int **G, int ntasks) {
     int i, j, source;
     int *curr;
     curr = (int *)malloc(sizeof(int) * ntasks);
     gen_ts(G, ntasks);
     source = topological_order[0];
     paths = (int **)malloc((ntasks*ntasks) * sizeof(int *));
     for (i = 0; i < (ntasks*ntasks); ++i) {
          paths[i] = (int *)malloc(ntasks * sizeof(int));
     }
     for (i = 0; i < (ntasks*ntasks); ++i) {
          for (j = 0; j < ntasks; ++j) {
               paths[i][j] = -1;
               curr[j] = -1;
          }
          paths[i][source] = 0;
     }
     pathid = 0;
     for (int i = 0; i < ntasks; ++i) {
          for (int j = 0; j < ntasks; ++j) {
               curr[j] = -1;
          }
          curr[0] = source;
          if (G[source][i] > 0) {
               find_path(G, ntasks, i, 1, curr);
          }
     }
}
