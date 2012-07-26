#include <stdio.h>
#include <stdlib.h>

int main(int argc, char **argv) {

     FILE *f;
     unsigned long lvl, i, j, n;
     char **M;

     if (argc == 1) {
          printf("Usage: %s Lvl2_width [/path/to/output/file]", argv[0]);
          return 0;
     } else if (argc == 3) {
          f = fopen(argv[2], "w");
     } else {
          f = stdout;
     }

     lvl = strtoul(argv[1], NULL, 0);
     n = 6 * lvl + 1;

     M = (char **)malloc((n + 1) * sizeof(char *));
     for (i = 0; i <= n; ++i) {
          M[i] = (char *)malloc((n + 1) * sizeof(char));
          for (j = 0; j <= n; ++j) {
               M[i][j] = '0';
          }
     }

     for (i = 1; i <= lvl; ++i) {
          M[0][i] = '1';
          M[i][i + lvl] = '1';
          M[i][2 * lvl + i - 1] = '1';
          M[i][5 * lvl + i] = '1';
          M[5 * lvl][5 * lvl + i] = '1';
          M[5 * lvl + i][6 * lvl + 1] = '1';
     }

     for (i = 1; i <= 2 * lvl - 1; ++i) {
          M[i + lvl][3 * lvl + i - 1] = '1';
          M[3 * lvl + i - 1][5 * lvl - 1] = '1';
     }

     for (i = 0; i <= n; ++i) {
          for (j = 0; j <= n; ++j) {
               fprintf(f, "%c ", M[i][j]);
          }
          fprintf(f, "%c\n", M[i][j + 1]);
     }

     for (i = 0; i <= n; ++i) {
          free(M[i]);
     }
     free(M);
     if (argc == 3) {
          fclose(f);
     }

     return 0;

}
