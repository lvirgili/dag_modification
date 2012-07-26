#include <stdio.h>
#include <stdlib.h>

int main(int argc, char **argv) {

     FILE *f;
     unsigned long lvl, i, j;

     if (argc == 1) {
          printf("Usage: %s Lvl2_width [/path/to/output/file]", argv[0]);
          return 0;
     } else if (argc == 3) {
          f = fopen(argv[2], "w");
     } else {
          f = stdout;
     }

     lvl = strtoul(argv[1], NULL, 0);

     fprintf(f, "0 ");
     for (i = 0; i < lvl; ++i) {
          fprintf(f, "1 ");
     }
     fprintf(f, "0\n");
     for (i = 0; i < lvl; ++i) {
          for (j = 0; j <= lvl; ++j) {
               fprintf(f, "0 ");
          }
          fprintf(f, "1\n");
     }
     for (i = 0; i <= lvl; ++i) {
          fprintf(f, "0 ");
     }
     fprintf(f, "0\n");

     if(argc == 3) {
          fclose(f);
     }

     return 0;
}
