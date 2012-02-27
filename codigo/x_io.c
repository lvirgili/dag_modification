#include "x_io.h"

int readInputFile(char* nome) {

     FILE * arquivo;
     int i,j, aux;

     //open input file
     arquivo = fopen(nome,"r");
     if (!arquivo) {
          fprintf(stderr,"Error opening input file :'(.\n");
          return(1);
     }

     // Loading parameters from file ------------------------------

     // Loading values of 'n', 'm' and 'o'
     fscanf(arquivo,"n: %d\n",&n);
     fscanf(arquivo,"m: %d\n",&m);
     fscanf(arquivo,"o: %d\n",&o);

     // Alocando espaço para o vetor 'I'
     I = (double *)malloc(n*sizeof(double));
     if (!I) {
          fprintf(stderr,"Error allocating memory [I] :'(.\n");
          fclose(arquivo);
          return(2);
     }
     // Reading vector 'I'
     fscanf(arquivo,"I: [ (1) %lf ",I);
     for (i=1;i<n;i++)
          fscanf(arquivo,"%lf",I+i);
     fscanf(arquivo," ]\n");

     // Alocando espaço para o vetor 'S'
     S = (int *)malloc(n*sizeof(int));
     if (!S) {
          fprintf(stderr,"Error allocating memory [S] :'(.\n");
          fclose(arquivo);
          return(2);
     }
     // Reading vector 'S'
     fscanf(arquivo,"S: [ (1) %d ",S);
     for (i=1;i<n;i++)
          fscanf(arquivo,"%d",S+i);
     fscanf(arquivo," ]\n");

     // Alocando espaço para a matriz 'B'
     B = (double **)malloc(n*sizeof(double *));
     if (!B) {
          fprintf(stderr,"Error allocating memory [B] :'(.\n");
          fclose(arquivo);
          return(2);
     }
     for (i=0;i<n;i++) {
          *(B+i)=(double *)malloc(n*sizeof(double));
          if (! (*(B+i)) ) {
               fprintf(stderr,"Error allocating memory [B+i] :'(.\n");
               fclose(arquivo);
               return(2);
          }
     }
     // Lendo a matriz 'B'
     for (i=0;i<n;i++)
          for (j=0;j<n;j++) {
               if (i==0 && j==0)
                    fscanf(arquivo,"B: [ (%d 1) %lf",&aux,*B);
               else {
                    if (j==0)
                         fscanf(arquivo,"\n     (%d 1) %lf",&aux,*(B+i));
                    else
                         fscanf(arquivo,"%lf",*(B+i)+j);
               }
          }
     fscanf(arquivo,"\n   ]\n");

     // Alocando espaço para a matriz 'D'
     D = (int **)malloc(n*sizeof(int *));
     if (!D) {
          fprintf(stderr,"Error allocating memory [D] :'(.\n");
          fclose(arquivo);
          return(2);
     }
     for (i=0;i<n;i++) {
          *(D+i)=(int *)malloc(n*sizeof(int));
          if (! (*(D+i)) ) {
               fprintf(stderr,"Error allocating memory [D+i] :'(.\n");
               fclose(arquivo);
               return(2);
          }
     }
     // Lendo a matriz 'D'
     for (i=0;i<n;i++)
          for (j=0;j<n;j++) {
               if (i==0 && j==0)
                    fscanf(arquivo,"D: [ (%d 1) %d",&aux,*D);
               else {
                    if (j==0)
                         fscanf(arquivo,"\n     (%d 1) %d",&aux,*(D+i));
                    else
                         fscanf(arquivo,"%d",*(D+i)+j);
               }
          }
     fscanf(arquivo,"\n   ]\n");

     // Loading value of 'Tmax'
     fscanf(arquivo,"Tmax: %d\n",&Tmax);
     Tmax = ceil(Tmax/timeUnit);

     // Alocando espaço para o vetor 'TI'
     TI = (double *)malloc(m*sizeof(double));
     if (!TI) {
          fprintf(stderr,"Error allocating memory [TI] :'(.\n");
          fclose(arquivo);
          return(2);
     }
     // Reading vector 'TI'
     fscanf(arquivo,"TI: [ (1) %lf ",TI);
     TI[0] /= timeUnit;

     for (i=1;i<m;i++) {
          fscanf(arquivo,"%lf",TI+i);
          TI[i]/=timeUnit;
     }
     fscanf(arquivo," ]\n");

     // Alocando espaço para o vetor 'C'
     C = (int *)malloc(m*sizeof(int));
     if (!C) {
          fprintf(stderr,"Error allocating memory [C] :'(.\n");
          fclose(arquivo);
          return(2);
     }
     // Reading vector 'C'
     fscanf(arquivo,"C: [ (1) %d ",C);
     for (i=1;i<m;i++)
          fscanf(arquivo,"%d",C+i);
     fscanf(arquivo," ]\n");

     // Alocando espaço para a matriz 'TB'
     TB = (double **)malloc(m*sizeof(double *));
     if (!TB) {
          fprintf(stderr,"Error allocating memory [TB] :'(.\n");
          fclose(arquivo);
          return(2);
     }
     for (i=0;i<m;i++) {
          *(TB+i)=(double *)malloc(m*sizeof(double));
          if (! (*(TB+i)) ) {
               fprintf(stderr,"Error allocating memory [TB+i] :'(.\n");
               fclose(arquivo);
               return(2);
          }
     }
     // Lendo a matriz 'TB'
     for (i=0;i<m;i++)
          for (j=0;j<m;j++) {
               if (i==0 && j==0)
                    fscanf(arquivo,"TB: [ (%d 1) %lf",&aux,*TB);
               else {
                    if (j==0)
                         fscanf(arquivo,"\n     (%d 1) %lf",&aux,*(TB+i));
                    else
                         fscanf(arquivo,"%lf",*(TB+i)+j);
               }
               TB[i][j]/=timeUnit;
          }
     fscanf(arquivo,"\n   ]\n");

     // Alocando espaço para a matriz 'N'
     N = (int **)malloc(m*sizeof(int *));
     if (!N) {
          fprintf(stderr,"Error allocating memory [N] :'(.\n");
          fclose(arquivo);
          return(2);
     }
     for (i=0;i<m;i++) {
          *(N+i)=(int *)malloc(m*sizeof(int));
          if (! (*(N+i)) ) {
               fprintf(stderr,"Error allocating memory [N+i] :'(.\n");
               fclose(arquivo);
               return(2);
          }
     }
     // Lendo a matriz 'N'
     for (i=0;i<m;i++)
          for (j=0;j<m;j++) {
               if (i==0 && j==0)
                    fscanf(arquivo,"N: [ (%d 1) %d",&aux,*N);
               else {
                    if (j==0)
                         fscanf(arquivo,"\n     (%d 1) %d",&aux,*(N+i));
                    else
                         fscanf(arquivo,"%d",*(N+i)+j);
               }
          }
     fscanf(arquivo,"\n   ]\n");

     // Alocando espaço para o vetor 'SV'
     SV = (int *)malloc(o*sizeof(int));
     if (!SV) {
          fprintf(stderr,"Error allocating memory [SV] :'(.\n");
          fclose(arquivo);
          return(2);
     }
     // Reading vector 'SV'
     fscanf(arquivo,"SV: [ (1) %d ",SV);
     for (i=1;i<o;i++)
          fscanf(arquivo,"%d",SV+i);
     fscanf(arquivo," ]\n");

     // Alocando espaço para o vetor 'BV'
     BV = (double *)malloc(o*sizeof(double));
     if (!BV) {
          fprintf(stderr,"Error allocating memory [BV] :'(.\n");
          fclose(arquivo);
          return(2);
     }
     // Reading vector 'BV'
     fscanf(arquivo,"BV: [ (1) %lf ",BV);
     for (i=1;i<o;i++)
          fscanf(arquivo,"%lf",BV+i);
     fscanf(arquivo," ]\n");

     // Alocando espaço para o vetor 'TR'
     TR = (double *)malloc(m*sizeof(double));
     if (!TR) {
          fprintf(stderr,"Error alocating memory [TR] :'(.\n");
          fclose(arquivo);
          return(2);
     }
     // Reading vector 'TR'
     fscanf(arquivo,"TR: [ (1) %lf ",TR);
     TR[0] /= timeUnit;
     for (i=1;i<m;i++) {
          fscanf(arquivo,"%lf",TR+i);
          TR[i] /= timeUnit;
     }
     fscanf(arquivo," ]\n");

     // Alocando espaço para o vetor 'TV'
     TV = (double *)malloc(o*sizeof(double));
     if (!TV) {
          fprintf(stderr,"Error alocating memory [TV] :'(.\n");
          fclose(arquivo);
          return(2);
     }
     // Reading vector 'TV'
     fscanf(arquivo,"TV: [ (1) %lf ",TV);
     TV[0] /= timeUnit;

     for (i=1;i<o;i++) {
          fscanf(arquivo,"%lf",TV+i);
          TV[i] /= timeUnit;
     }
     fscanf(arquivo," ]\n");


     fclose(arquivo);

     return(0);
}

int readInputFiles(char* appFile, char* gridFile, char* vmrFile) {

     FILE * arquivo;
     int i,j, aux;

     /*******************************/
     /* open application input file */
     /*******************************/
     arquivo = fopen(appFile,"r");
     if (!arquivo) {
          fprintf(stderr,"Error opening application input file :'(.\n");
          return(1);
     }

     /* Reading the amount of tasks (n) */
     fscanf(arquivo,"n: %d\n",&n);

     /* Alocando espaço para o vetor 'I'*/
     I = (double *)malloc(n*sizeof(double));
     if (!I) {
          fprintf(stderr,"Error alocating memory [I] :'(.\n");
          fclose(arquivo);
          return(2);
     }
     /* Reading vector 'I' */
     fscanf(arquivo,"I: [ (1) %lf ",I);
     for (i=1;i<n;i++)
          fscanf(arquivo,"%lf",I+i);
     fscanf(arquivo," ]\n");

     /* Alocando espaço para o vetor 'S' */
     S = (int *)malloc(n*sizeof(int));
     if (!S) {
          fprintf(stderr,"Error alocating memory [S] :'(.\n");
          fclose(arquivo);
          return(2);
     }
     /* Reading vector 'S' */
     fscanf(arquivo,"S: [ (1) %d ",S);
     for (i=1;i<n;i++)
          fscanf(arquivo,"%d",S+i);
     fscanf(arquivo," ]\n");

     /* Alocando espaço para a matriz 'B' */
     B = (double **)malloc(n*sizeof(double *));
     if (!B) {
          fprintf(stderr,"Error alocating memory [B] :'(.\n");
          fclose(arquivo);
          return(2);
     }
     for (i=0;i<n;i++) {
          *(B+i)=(double *)malloc(n*sizeof(double));
          if (! (*(B+i)) ) {
               fprintf(stderr,"Error alocating memory [B+i] :'(.\n");
               fclose(arquivo);
               return(2);
          }
     }
     /* Lendo a matriz 'B' */
     for (i=0;i<n;i++)
          for (j=0;j<n;j++) {
               if (i==0 && j==0)
                    fscanf(arquivo,"B: [ (%d 1) %lf",&aux,*B);
               else {
                    if (j==0)
                         fscanf(arquivo,"\n     (%d 1) %lf",&aux,*(B+i));
                    else
                         fscanf(arquivo,"%lf",*(B+i)+j);
               }
          }
     fscanf(arquivo,"\n   ]\n");

     // Alocando espaço para a matriz 'D'
     D = (int **)malloc(n*sizeof(int *));
     if (!D) {
          fprintf(stderr,"Error alocating memory [D] :'(.\n");
          fclose(arquivo);
          return(2);
     }
     for (i=0;i<n;i++) {
          *(D+i)=(int *)malloc(n*sizeof(int));
          if (! (*(D+i)) ) {
               fprintf(stderr,"Error alocating memory [D+i] :'(.\n");
               fclose(arquivo);
               return(2);
          }
     }
     // Lendo a matriz 'D'
     for (i=0;i<n;i++)
          for (j=0;j<n;j++) {
               if (i==0 && j==0)
                    fscanf(arquivo,"D: [ (%d 1) %d",&aux,*D);
               else {
                    if (j==0)
                         fscanf(arquivo,"\n     (%d 1) %d",&aux,*(D+i));
                    else
                         fscanf(arquivo,"%d",*(D+i)+j);
               }
          }
     //fscanf(arquivo,"\n   ]\n");

     /* Closing the input file */
     fclose(arquivo);

     /************************/
     /* open grid input file */
     /************************/
     arquivo = fopen(gridFile,"r");
     if (!arquivo) {
          fprintf(stderr,"Error opening application input file :'(.\n");
          return(2);
     }

     /* Reading the amount of hosts (m) */
     fscanf(arquivo,"m: %d\n",&m);

     // Alocando espaço para o vetor 'TI'
     TI = (double *)malloc(m*sizeof(double));
     if (!TI) {
          fprintf(stderr,"Error alocating memory [TI] :'(.\n");
          fclose(arquivo);
          return(2);
     }
     // Reading vector 'TI'
     fscanf(arquivo,"TI: [ (1) %lf ",TI);
     TI[0] /= timeUnit;

     for (i=1;i<m;i++) {
          fscanf(arquivo,"%lf",TI+i);
          TI[i]/=timeUnit;
     }
     fscanf(arquivo," ]\n");

     // Alocando espaço para o vetor 'C'
     C = (int *)malloc(m*sizeof(int));
     if (!C) {
          fprintf(stderr,"Error alocating memory [C] :'(.\n");
          fclose(arquivo);
          return(2);
     }
     // Reading vector 'C'
     fscanf(arquivo,"C: [ (1) %d ",C);
     for (i=1;i<m;i++)
          fscanf(arquivo,"%d",C+i);
     fscanf(arquivo," ]\n");

     // Alocando espaço para a matriz 'TB'
     TB = (double **)malloc(m*sizeof(double *));
     if (!TB) {
          fprintf(stderr,"Error alocating memory [TB] :'(.\n");
          fclose(arquivo);
          return(2);
     }
     for (i=0;i<m;i++) {
          *(TB+i)=(double *)malloc(m*sizeof(double));
          if (! (*(TB+i)) ) {
               fprintf(stderr,"Error alocating memory [TB+i] :'(.\n");
               fclose(arquivo);
               return(2);
          }
     }
     // Lendo a matriz 'TB'
     for (i=0;i<m;i++)
          for (j=0;j<m;j++) {
               if (i==0 && j==0)
                    fscanf(arquivo,"TB: [ (%d 1) %lf",&aux,*TB);
               else {
                    if (j==0)
                         fscanf(arquivo,"\n     (%d 1) %lf",&aux,*(TB+i));
                    else
                         fscanf(arquivo,"%lf",*(TB+i)+j);
               }
               TB[i][j]/=timeUnit;
          }
     fscanf(arquivo,"\n   ]\n");

     // Alocando espaço para a matriz 'N'
     N = (int **)malloc(m*sizeof(int *));
     if (!N) {
          fprintf(stderr,"Error alocating memory [N] :'(.\n");
          fclose(arquivo);
          return(2);
     }
     for (i=0;i<m;i++) {
          *(N+i)=(int *)malloc(m*sizeof(int));
          if (! (*(N+i)) ) {
               fprintf(stderr,"Error alocating memory [N+i] :'(.\n");
               fclose(arquivo);
               return(2);
          }
     }
     // Lendo a matriz 'N'
     for (i=0;i<m;i++)
          for (j=0;j<m;j++) {
               if (i==0 && j==0)
                    fscanf(arquivo,"N: [ (%d 1) %d",&aux,*N);
               else {
                    if (j==0)
                         fscanf(arquivo,"\n     (%d 1) %d",&aux,*(N+i));
                    else
                         fscanf(arquivo,"%d",*(N+i)+j);
               }
          }
     fscanf(arquivo,"\n   ]\n");

     // Alocando espaço para o vetor 'TR'
     TR = (double *)malloc(m*sizeof(double));
     if (!TR) {
          fprintf(stderr,"Error alocating memory [TR] :'(.\n");
          fclose(arquivo);
          return(2);
     }
     // Reading vector 'TR'
     fscanf(arquivo,"TR: [ (1) %lf ",TR);
     TR[0] /= timeUnit;
     for (i=1;i<m;i++) {
          fscanf(arquivo,"%lf",TR+i);
          TR[i] /= timeUnit;
     }
     //fscanf(arquivo," ]\n");

     /* Closing the input file */
     fclose(arquivo);


     /*********************************/
     /* open VM repository input file */
     /*********************************/
     arquivo = fopen(vmrFile,"r");
     if (!arquivo) {
          fprintf(stderr,"Error opening application input file :'(.\n");
          return(3);
     }

     /* Reading the amount of virtual machines (o) */
     fscanf(arquivo,"o: %d\n",&o);

     // Alocando espaço para o vetor 'SV'
     SV = (int *)malloc(o*sizeof(int));
     if (!SV) {
          fprintf(stderr,"Error alocating memory [SV] :'(.\n");
          fclose(arquivo);
          return(2);
     }
     // Reading vector 'SV'
     fscanf(arquivo,"SV: [ (1) %d ",SV);
     for (i=1;i<o;i++)
          fscanf(arquivo,"%d",SV+i);
     fscanf(arquivo," ]\n");

     // Alocando espaço para o vetor 'BV'
     BV = (double *)malloc(o*sizeof(double));
     if (!BV) {
          fprintf(stderr,"Error alocating memory [BV] :'(.\n");
          fclose(arquivo);
          return(2);
     }
     // Reading vector 'BV'
     fscanf(arquivo,"BV: [ (1) %lf ",BV);
     for (i=1;i<o;i++)
          fscanf(arquivo,"%lf",BV+i);
     fscanf(arquivo," ]\n");

     // Alocando espaço para o vetor 'TV'
     TV = (double *)malloc(o*sizeof(double));
     if (!TV) {
          fprintf(stderr,"Error alocating memory [TV] :'(.\n");
          fclose(arquivo);
          return(2);
     }
     // Reading vector 'TV'
     fscanf(arquivo,"TV: [ (1) %lf ",TV);
     TV[0] /= timeUnit;

     for (i=1;i<o;i++) {
          fscanf(arquivo,"%lf",TV+i);
          TV[i] /= timeUnit;
     }
     //fscanf(arquivo," ]\n");
     /* Closing the input file */
     fclose(arquivo);
     return(0);
}

void calcTmax(void) {
     int i, k, v, mink;
     double temp;
     /* Total amount of instructions */
     double totalI=0;
     /* Total amount of MB that have to be transmitted for VM instantiation */
     double totalBV=0;
     /* Total amount of time needed to boot all the VMs that will be used */
     double totalTV=0;
     /*  */
     double minTI;

     int* Vi;
     Vi = (int *)malloc(o*sizeof(int));
     for (v=0; v<o; v++) {
          Vi[v]=0;
     }

     for (i=0; i<n; i++) {
          v=S[i]-1;
          totalI += I[i];
          totalTV += TV[v];

          if(Vi[v] == 0)
               totalBV += BV[v];
          Vi[v]++;

     }
     minTI = TI[0]*totalI + TR[0]*totalBV;
     for (k=1; k<m; k++) {
          temp = TI[k]*totalI + TR[k]*totalBV;
          if(minTI > temp) {
               minTI = temp;
               mink = k;

          }
     }
     minTI += totalTV;
     //printf("\n%d --> %lf -- %lf\n\n", mink, minTI, minTI*timeUnit);
     Tmax = ceil(minTI);
     TmaxReal = minTI*timeUnit;
}

void calcTmaxOld(void) {
     int i, k, mink;
     double temp;
     /* Total amount of instructions */
     double totalI=0;
     /* Total amount of MB that have to be transmitted for VM instantiation */
     double totalBV=0;
     double totalTV=0;
     /*  */
     double minTI;

     for (i=0; i<n; i++) {
          totalI += I[i];
          totalBV += BV[S[i]-1];
          totalTV += TV[S[i]-1];
     }
     minTI = TI[0]*totalI + TR[0]*totalBV;
     for (k=1; k<m; k++) {
          temp = TI[k]*totalI + TR[k]*totalBV;
          if(minTI > temp) {
               minTI = temp;
               mink = k;

          }
     }
     minTI += totalTV;
     //printf("\n%d --> %lf -- %lf\n\n", mink, minTI, minTI*timeUnit);
     Tmax = ceil(minTI);
     TmaxReal = minTI*timeUnit;
}

//Prints value of parameters
void printVars(void) {

     int i,j;

     printf("\n**********************************************************\n\n");
     printf("n = %d m = %d o = %d Tmax= %d\n",n,m,o,Tmax);


     //Parameters related to Tasks ------------------------------

     //Processing demand of tasks (Number of instructions)
     for (i=0;i<n;i++) {
          printf("I[%d] = %lf ",i,I[i]);
     }
     printf("\n");

     //Software demand of tasks (VM's ID)
     for (i=0;i<n;i++) {
          printf("S[%d] = %d ",i,S[i]);
     }
     printf("\n");

     //Number of byte transmitted between tasks
     for (i=0;i<n;i++) {
          for(j=0;j<n;j++) {
               printf("B[%d,%d] = %lf ",i,j,B[i][j]);
          }
          printf("\n");
     }

     //Set of arcs between tasks
     for (i=0;i<n;i++) {
          for(j=0;j<n;j++) {
               printf("D[%d,%d] = %d ",i,j,D[i][j]);
          }
          printf("\n");
     }


     //Parameters related to Hosts ------------------------------

     //Time each host takes to execute one instruction
     for (i=0;i<m;i++) {
          printf("TI[%d] = %lf ",i,TI[i]);
     }
     printf("\n");

     //Number of processing cores per host
     for (i=0;i<m;i++) {
          printf("C[%d] = %d ",i,C[i]);
     }
     printf("\n");

     //Time for transmitting 1 byte between hosts
     for (i=0;i<m;i++) {
          for(j=0;j<m;j++) {
               printf("TB[%d,%d] = %lf ",i,j,TB[i][j]);
          }
          printf("\n");
     }

     //Set of links between hosts
     for (i=0;i<m;i++) {
          for(j=0;j<m;j++) {
               printf("N[%d,%d] = %d ",i,j,N[i][j]);
          }
          printf("\n");
     }


     //Parameters related to Virtual Machines -------------------

     //Software ofering of virtual Machines
     for (i=0;i<o;i++) {
          printf("SV[%d] = %d ",i,SV[i]);
     }
     printf("\n");

     //Size of virtual Machines
     for (i=0;i<o;i++) {
          printf("BV[%d] = %lf ",i,BV[i]);
     }
     printf("\n");

     //Time for transmitting 1 byte between the VMs repository and a hosts
     for (i=0;i<m;i++) {
          printf("TR[%d] = %lf ",i,TR[i]);
     }
     printf("\n");

     //Time taken for booting the virtual Machines
     for (i=0;i<o;i++) {
          printf("TV[%d] = %lf ",i,TV[i]);
     }
     printf("\n");

     printf("\n**********************************************************\n\n");
}

void printApp(void) {
     int i, j;
     printf("n: %d\n",n);

     printf("I: [ (1)");
     for(i=0;i<n;i++)
          printf(" %.0lf",I[i]);
     printf("]\n");

     for (i=0;i<n;i++)
          for (j=0;j<n;j++) {
               if (i==0 && j==0)
                    printf("B: [ (%d 1) %.0lf",i,B[i][j]);
               else {
                    if (j==0)
                         printf("\n     (%d 1) %.0lf",i,B[i][j]);
                    else
                         printf(" %.0lf",B[i][j]);
               }
          }
     printf("\n   ]\n");

     for (i=0;i<n;i++)
          for (j=0;j<n;j++) {
               if (i==0 && j==0)
                    printf("D: [ (%d 1) %d",i,D[i][j]);
               else {
                    if (j==0)
                         printf("\n     (%d 1) %d",i,D[i][j]);
                    else
                         printf(" %d",D[i][j]);
               }
          }
     printf("\n   ]\n");
}

void printApp2File(char * fileName) {
     int i, j;
     char strline[10000]="";
     FILE *outFile;

     outFile = fopen(fileName,"w");

     sprintf(strline, "n: %d\n",n);
     fputs(strline, outFile);

     sprintf(strline, "I: [ (1)");
     for(i=0;i<n;i++)
          sprintf(strline, "%s %.6lf",strline,I[i]);
     sprintf(strline, "%s]\n",strline);
     fputs(strline, outFile);

     for (i=0;i<n;i++)
          for (j=0;j<n;j++) {
               if (i==0 && j==0)
                    sprintf(strline, "B: [ (%d 1) %.6lf",i,B[i][j]);
               else {
                    if (j==0) {
                         sprintf(strline, "%s\n",strline);
                         fputs(strline, outFile);
                         sprintf(strline, "     (%d 1) %.6lf",i,B[i][j]);
                    }
                    else
                         sprintf(strline, "%s %.6lf",strline,B[i][j]);
               }
          }
     sprintf(strline, "%s\n   ]\n",strline);
     fputs(strline, outFile);

     for (i=0;i<n;i++)
          for (j=0;j<n;j++) {
               if (i==0 && j==0)
                    sprintf(strline, "D: [ (%d 1) %d",i,D[i][j]);
               else {
                    if (j==0) {
                         sprintf(strline, "%s\n",strline);
                         fputs(strline, outFile);
                         sprintf(strline, "     (%d 1) %d",i,D[i][j]);
                    }
                    else
                         sprintf(strline, "%s %d",strline,D[i][j]);
               }
          }
     sprintf(strline, "%s\n   ]\n",strline);
     fputs(strline, outFile);

     fclose(outFile);
}

void freeVars(void) {
     int i;

     free(I);
     free(S);

     for (i=0;i<n;i++) {
          free(*(B+i));
     }
     free(B);

     for (i=0;i<n;i++) {
          free(*(D+i));
     }
     free(D);

     free(TI);
     free(C);

     for (i=0;i<m;i++) {
          free(*(TB+i));
     }
     free(TB);

     for (i=0;i<m;i++) {
          free(*(N+i));
     }
     free(N);

     free(SV);
     free(BV);
     free(TR);
     free(TV);
}
