#include "graph.h"

static void find_path(int **G, int ntasks, int vertex, int pos, int *curr) {
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

static void calculate_weights(int **G, int ntasks) {
     int i, j;
     path_weight = (int *)malloc(sizeof(int) * (ntasks * pathid));
     for (i = 0; i < pathid; ++i) {
          path_weight[i] = 0;
          for (j = 0; j < ntasks; ++j) {
               path_weight[i] += G[paths[i][j]][paths[i][j+1]];
               if (paths[i][j+1] == ntasks-1) {
                    break;
               }
          }
     }
}

static void gen_paths(int **G, int ntasks) {
     int i, j, source;
     int *curr;
     curr = (int *)malloc(sizeof(int) * ntasks);
     source = 0;
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
     calculate_weights(G, ntasks);
}

void mdfDagOpc3(int **G, int *S, double *TV, int ntasks, int nvm) {
     int **mdfG, i, j, s, task;
     int **Ph;
     double *new_I;
     gen_paths(G, ntasks);
     mdfG = (int **)malloc((ntasks*nvm + 1) * sizeof(int *));
     for (i = 0; i < ntasks*nvm + 1; ++i) {
          mdfG[i] = (int *)malloc(ntasks * sizeof(int));
     }
     /* pathid * nvm sets */
     /* Ph[pathid+mv][x] == 1 if the task x in the set P_{pathid,mv} */
     Ph = (int **)malloc((pathid*nvm) * sizeof(int *));
     for (i = 0; i < pathid*nvm; ++i) {
          Ph[i] = calloc(ntasks, sizeof(int));
     }
     for (i = 0; i < pathid; ++i) {
          for (j = 0; j < ntasks; ++j) {
               task = paths[i][j];
               s = S[task];
               Ph[i+i+s][task] = 1;
               if (paths[i][j] == ntasks-1) {
                    break;
               }
          }
     }
     for (i = 0; i < pathid*nvm; ++i) {
          for (j = 0; j < ntasks; ++j) {
               printf("%d ", Ph[i][j]);
          }
          printf("\n");
     }
     new_I = (double *)malloc(sizeof(double));

     for (i = 0; i < ntasks*nvm + 1; ++i) {
          free(mdfG[i]);
     }
     free(mdfG);
     for (i = 0; i < pathid*nvm; ++i) {
          free(Ph[i]);
     }
     free(Ph);
     free(new_I);

}
