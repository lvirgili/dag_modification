#include <stdio.h>
#include <stdlib.h>

int main(int argc, char **argv) {

     FILE *f;
     unsigned long lvl, i, j, n, mod;

     if (argc == 1) {
          printf("Usage: %s Lvl2_width [/path/to/output/file]", argv[0]);
          return 0;
     } else if (argc == 3) {
          f = fopen(argv[2], "w");
     } else {
          f = stdout;
     }

     lvl = strtoul(argv[1], NULL, 0);
     n = 3 * lvl + 3;
     mod = lvl + 1;

     for (i = 0; i <= n; ++i) {
         if (i % mod == 0) {
             for (j = 0; j <= n; ++j) {
                 if (j >= i + 1 && j < i + 1 + lvl) {
                     fprintf(f, "1");
                 } else {
                     fprintf(f, "0");
                 }
                 if (j == n) {
                     fprintf(f, "\n");
                 } else {
                     fprintf(f, " ");
                 }
             }
         } else {
             for (j = 0; j <= n; ++j) {
                 if (j == (i / mod + 1) * mod) {
                     fprintf(f, "1");
                 } else {
                     fprintf(f, "0");
                 }
                 if (j == n) {
                     fprintf(f, "\n");
                 } else {
                     fprintf(f, " ");
                 }
             }
         }
     }

     if (argc == 3) {
         fclose(f);
     }

     return 0;

}
