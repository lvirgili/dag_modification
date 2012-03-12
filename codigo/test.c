#include "graph.h"

int main() {
     int **G, n, i, j;
     n = 6;
     G = (int **)malloc(n * sizeof(int *));
     for (i = 0; i < n; ++i) {
          G[i] = (int *)malloc(n * sizeof(int));
     }

     for (i = 0; i < n; ++i) {
          for (j = 0; j < n; ++j) {
               G[i][j] = 0;
          }
     }
     G[0][1] = 1; G[0][4] = 1;
     G[2][0] = 1; G[2][3] = 1; G[2][4] = 1;
     G[3][4] = 1; G[3][5] = 1;
     G[4][1] = 1; G[4][5] = 1;
     G[5][1] = 1;

     /* G[2][1] = 1; G[2][4] = 1; */
     /* G[4][0] = 1; G[4][3] = 1; */

     for (i = 0; i < n; ++i) {
          for (j = 0; j < n; ++j) {
               printf("%d ", G[i][j]);
          }
          printf("\n");
     }
     printf("\n");
     gen_paths(G, n);
     for (i = 0; i < n; ++i) {
          printf("%d ", topological_order[i]);
     }
     printf("\n\n");

     for (i = 0; i < pathid; ++i) {
          for (j = 0; j < n; ++j) {
               printf("%2d ", paths[i][j]);
               if (paths[i][j] == topological_order[n-1]) break;
          }
          printf("\n");
     }
     return 0;
}
