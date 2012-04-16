#include "graph.h"

int main() {
     int **G, *S, n, i, j, nvm;
     double *TV, *I;
     n = 5;
     nvm = 2;
     G = (int **)malloc(n * sizeof(int *));
     S = (int *)malloc(n * sizeof(int));
     I = (double *)malloc(n * sizeof(double));
     TV = (double *)malloc(nvm * sizeof(double));
     for (i = 0; i < n; ++i) {
          I[i] = 0;
          G[i] = (int *)malloc(n * sizeof(int));
     }

     for (i = 0; i < n; ++i) {
          for (j = 0; j < n; ++j) {
               G[i][j] = 0;
          }
     }
     G[0][1] = 2; G[0][2] = 3; G[0][3] = 1;
     G[1][4] = 5;
     G[2][4] = 7;
     G[3][4] = 2;

     S[0] = 0; S[1] = 0; S[2] = 1;
     S[3] = 1; S[4] = 1;

     TV[0] = 12.3; TV[1] = 15.3;


     mdfDagOpc3(G, S, TV, n, nvm);

     free(S); free(TV); free(path_weight); free(I);
     for (i = 0; i < n; ++i) {
          free(G[i]);
     }
     free(G);
     for (i = 0; i < n*n; ++i) {
          free(paths[i]);
     }
     free(paths);
     return 0;
}
