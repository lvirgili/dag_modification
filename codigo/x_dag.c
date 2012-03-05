/*
 *  Autor: Cesar G. Chaves A. <cesarchaves1@gmail.com>
 *  Data:  05/08/2010
 */

#include "x_io.h"
#include "x_dag.h"

int main(int argc, char **argv) {

     infinite=999999;

     time(&start_t);
     timeUnit = 1;
     /* Reading the input file or files */
     if (argc == 6) {
          if(readInputFile(argv[4])) {
               return(2);
          }
          else {
               time(&end_t);
               progExec_t = difftime(end_t,start_t);
               //printf("\n--> Finished reading input data! (t=%lf)\n", progExec_t);
               //calcTmax();
               //printf("\nTmax: %.4lf\n\n", TmaxReal);
          }
          strcpy(pathName, argv[5]);
     }
     else if (argc == 8) {
          if(readInputFiles(argv[4], argv[5], argv[6])) {
               return(2);
          }
          else {
               time(&end_t);
               progExec_t = difftime(end_t,start_t);
               //printf("\n--> Finished reading input data files! (t=%lf)\n", progExec_t);
               //calcTmax();
               //printf("\nTmax: %f\n\n", TmaxReal);
          }
          /* Read output path */
          strcpy(pathName, argv[7]);
     }

     //printApp();
     mdfDagOpc2();
     //printApp();
     printTaskInfo();
     printf("---------\n");

}

int mdfDagOpc1() {
     int i, j;

     n1=2*n+1;

     /* Allocating memory space for vector vector 'I1' */
     I1 = (double *)malloc(n1*sizeof(double));
     if (!I1) {
          fprintf(stderr,"Error allocating memory [I1] :'(.\n");
          return(2);
     }
     i=0;
     /* Filling values of vector 'I1' */
     I1[i] = 0;
     while (++i<=n)
          I1[i] = 50; /*TODO: assign a variable*/
     i--;
     while (++i < n1)
          I1[i] = I[i-n-1];

    
     /* Allocating memory space for matrix 'B1' */
     B1 = (double **)malloc(n1*sizeof(double *));
     if (!B1) {
          fprintf(stderr,"Error allocating memory [B1] :'(.\n");
          return(2);
     }
     for (i=0;i<n1;i++) {
          *(B1+i)=(double *)malloc(n1*sizeof(double));
          if (! (*(B1+i)) ) {
               fprintf(stderr,"Error allocating memory [B1+i] :'(.\n");
               return(2);
          }
     }
     /* Filling values of array 'B1' */
     /* First line (MV Repository)*/
     i=0;
     B1[0][i] = 0;
     while (++i<=n)
          B1[0][i] = BV[S[i-1]-1];
     i--;
     while (++i < n1)
          B1[0][i] = 0;
     /* VMs association */
     for (j=1;j<=n;j++) {
          i=0;
          while (i<n1)
               if(i-n==j)
                    B1[j][i++] = infinite;
               else
                    B1[j][i++] = 0;
     }
     /* Copy values of B to B1 */
     for (;j<n1;j++) {
          i=0;
          while (i<=n)
               B1[j][i++] = 0;
          i--;
          while (i<n1) {
               B1[j][i] = B[j-n-1][i-n-1];
               i++;
          }
     }


     // Alocando espaço para a matriz 'D1'
     D1 = (int **)malloc(n1*sizeof(int *));
     if (!D1) {
          fprintf(stderr,"Error allocating memory [D1] :'(.\n");
          return(2);
     }
     for (i=0;i<n1;i++) {
          *(D1+i)=(int *)malloc(n1*sizeof(int));
          if (! (*(D1+i)) ) {
               fprintf(stderr,"Error allocating memory [D1+i] :'(.\n");
               return(2);
          }
     }

     i=0;
     /* Filling values of array 'D1' */
     /* First line (MV Repository)*/
     D1[0][i] = 0;
     while (++i<=n)
          D1[0][i] = 1;
     i--;
     while (++i < n1)
          D1[0][i] = 0;

     /* VMs association */
     for (j=1;j<=n;j++) {
          i=0;
          while (i<n1)
               if(i-n==j)
                    D1[j][i++] = 1;
               else
                    D1[j][i++] = 0;
     }

     /* Copy values of D to D1 */
     for (;j<n1;j++) {
          i=0;
          while (i<=n)
               D1[j][i++] = 0;
          //i--;
          while (i<n1) {
               D1[j][i] = D[j-n-1][i-n-1];
               i++;
          }
     }



//    free(I);
//    free(B);
//    free(D);

     n=n1;
     I=I1;
     B=B1;
     D=D1;

     return 0;
}


int mdfDagOpc2() {
     int i, j;

     tasksxVMa = (int *) malloc(o*sizeof(int));
     setsxVMa  = (int *) malloc(o*sizeof(int));
     instxVMa  = (int **)malloc(o*sizeof(int *));

     /* Initializes the tasksxVM and setsxVMa vectors */
     for (i=0; i<o; i++) {
          tasksxVMa[i] = 0;
          instxVMa[i] = 0;
     }

     /* Counts the amount of tasks per VM */
     for (i=0; i<n; i++)
          tasksxVMa[ S[i]-1 ] ++;

     tasksxVM = (int **)malloc(o*sizeof(int *));
     /* Allocates the necessary memory for matrix TasksxVM */
     for (i=0;i<o;i++)
          if (tasksxVMa[i] > 0) {
               *(tasksxVM+i)=(int *)malloc(tasksxVMa[i]*sizeof(int));
               for (j=0; j<tasksxVMa[i]; j++)
                    tasksxVM[i][j] = 0;
               tasksxVMa[i] = 0;
          }

     /* Fills the TasksxVM matrix */
     for (i=0; i<n; i++)
          tasksxVM[ S[i]-1 ] [tasksxVMa[ S[i]-1 ]++] = i;

     /* Allocates memory for the VM dimension of the setsxVM 3D matrix */
     setsxVM = (int ***)malloc(o*sizeof(int **));
     if (!setsxVM) {
          fprintf(stderr,"Error allocating memory [setsxMV] :'(.\n");
          return(2);
     }
     /* Allocates memory for the othr two dimensions of the setsxVM 3D matrix
      * and stores values
      */
     for (i=0;i<o;i++) {
          idSet = -1;
          setsxVMa[i] = 0;

          if (tasksxVMa[i] > 0) {
               c = (int *)malloc(tasksxVMa[i]*sizeof(int));
               SP(0, 1, i, tasksxVMa[i]);
               free(c);
               //printf("\t      # Sets: %d\n", setsxVMa[i]);
          }
     }

     n1=n;
     I1=I;
     B1=B;
     D1=D;

//    printData();
/*
  char ans;
  printf("Do you want to see the combinations? (y/N) ");
  scanf("%c", &ans);
  if (ans=='y')
*/
     calcSetsCartProd();


/*
  printf("\n\n");
  for (j=0; j<n; j++) {
  for (i=0; i<n; i++)
  printf("%.0lf ",B[j][i]);
  printf("\n");
  }

  printf("\n\n");

  for (j=0; j<n; j++) {
  for (i=0; i<n; i++)
  printf("%d ",D[j][i]);
  printf("\n");
  }
*/
     return 0;
}


/* Function: void SP(int m, int p)
 *
 * Description: a recursive algorithm for generating set partitions
 *              of a group with n elements.
 *
 * Algorithm found on paper: M. C. Er, "A Fast Algorithm for
 *                           Generating Set Partitions", Comput. J.,
 *                           vol. 31, no. 3, pp. 283-284, 1988
 *
 * Paper links: http://comjnl.oxfordjournals.org/cgi/reprint/31/3/283
 *              http://portal.acm.org/citation.cfm?id=48673
 */
void SP(int m, int p, int idPart, int n) {
     int i;
     if (p < n) {
          for (i=1; i<=m; i++) {
               c[p]=i;
               SP(m, p+1, idPart, n);
          }
          c[p]=m+1;
          SP(m+1, p+1, idPart, n);
     }
     else if (p==n)
          for (i=1;i<=m+1;i++) {
               c[p]=i;
               storeSet(idPart);
          }
}

void storeSet(int partition) {
     int i;

     idSet++;
     instxVMa[partition] = (int *) realloc( instxVMa[partition], (idSet+1)*sizeof(int) );

     instxVMa[partition][idSet]=1;
     setsxVMa[partition]++;


     /* Allocates memory for a new task partition set of the setsxVM 3D matrix */
     setsxVM[partition] = (int **) realloc( setsxVM[partition], (idSet+1)*sizeof(int *) );
     setsxVM[partition][idSet] = (int *) malloc(tasksxVMa[partition]*sizeof(int));
    
//    printf("%d\t    ", partition);
     for (i=0;i<tasksxVMa[partition];i++) {
//        printf(" %2d",c[i+1]);

          setsxVM[partition][idSet][i] = c[i+1];
                
          if (instxVMa[partition][idSet] < c[i+1])
               instxVMa[partition][idSet] = c[i+1];
     }
    
//    printf(" -- %2d instantiations\n", instxVMa[partition]);
}

/* Calculates the Cartesian Product of the VM partition sets  */
void calcSetsCartProd() {
     int i;
     int minVal = 0;
     int digit;
     int increment;
     int * setsxVMi; /* Index of sets per VM */


     setsxVMi = (int *)malloc(o*sizeof(int));

     /* Initializes the index of VM sets*/
     for (i=0; i<o; i++)
          if (setsxVMa[i]==0)
               setsxVMi[i]=-1;
          else
               setsxVMi[i]=minVal;

//	printf("\n\n------------------------------\n");
     int cont=0;

     while(setsxVMi[0]<setsxVMa[0]) {

//		printf("%d -- ", ++cont);

          digit = o-1;
          increment = 1;

          /* Creates a new DAG with for a given Cartesian Product result */
          createDAG(setsxVMi);

          //printApp();
          char aux_name[50], file_name[50];
          strcpy(file_name, "");
          strcpy(aux_name, pathName);
          sprintf(file_name,"/zz%06d.dat",++cont);
          strcat(aux_name, file_name);
          printApp2File(aux_name);

          free(I);
          free(B);
          free(D);
//exit(0);
          while (increment) {
               if ( (setsxVMi[digit] < setsxVMa[digit]-1) || (digit==0) ) {
                    setsxVMi[digit]++;
                    increment=0;
               }
               else {
                    if (digit != 0) {
                         if (setsxVMa[digit]==0)
                              setsxVMi[digit]=-1;
                         else
                              setsxVMi[digit]=minVal;
                         digit--;
                    }
               }
          }
          //printf("\n");
     }
}

void createDAG(int * setsxVMi) {
     int i, j, v;
     int instCurSet; /* Amount of VM instantiations on the current set */
     int instCount; /* VM instantiation counter */

     instCurSet=0;
     for (v=0; v<o; v++) {
//		printf("VM%d{", v);
          for (i=0;i<tasksxVMa[v];i++)
//		    printf(" %d:%d ",setsxVM[v][setsxVMi[v]][i]+instCurSet,tasksxVM[v][i]);
//		printf("}; ");

               if (setsxVMi[v] >= 0)
                    instCurSet += instxVMa[v][setsxVMi[v]];
     }
//	printf("\n");
     n = n1 + instCurSet + 1;


     instCurSet = 0;
     for (v=0; v<o; v++)
          if (tasksxVMa[v]>0)
               instCurSet += instxVMa[v][setsxVMi[v]];
     n = n1 + instCurSet + 1;

     /*********************************************
     Allocates memory space for the new 'I' vector
     *********************************************/
     I = (double *)malloc(n*sizeof(double));
     if (!I) {
          fprintf(stderr,"Error allocating memory for the new 'I' vector :'(.\n");
          //return(2);
          exit(0);
     }
     /***************************	
	 Filling values of vector 'I'
     ****************************/
     i=0;
     I[i] = 0;
     while (++i<=instCurSet)
          I[i] = 50; /*TODO: assign a variable like IV[v]*/
     i--;
     while (++i < n)
          I[i] = I1[i-instCurSet-1];

     /**********************************************
     Allocates memory space for the new 'B' matrix
     **********************************************/
     B = (double **)malloc(n*sizeof(double *));
     if (!B) {
          fprintf(stderr,"Error allocating memory for the new 'B' matrix :'(.\n");
          //return(2);
     }
     for (i=0;i<n;i++) {
          *(B+i)=(double *)malloc(n*sizeof(double));
          if (! (*(B+i)) ) {
               fprintf(stderr,"Error allocating memory [B+i] :'(.\n");
               //return(2);
          }
     }
	
     /***************************	
	 Filling values of array 'B'
     ****************************/
     for (j=0; j<n; j++)
          for (i=0; i<n; i++)
               B[j][i]=0;
	
     /* First line (MV Repository)*/
     i=0;
     for (v=0; v<o; v++)
          if (tasksxVMa[v]>0)
               for(j=0; j<instxVMa[v][setsxVMi[v]]; j++)
                    B[0][++i] = BV[v];
    
     /* VMs association */
     //infinite=1;
     instCount=0;
     for (v=0; v<o; v++)
          if (tasksxVMa[v]>0) {
               for(i=0; i<tasksxVMa[v]; i++) {
                    j = setsxVM [v] [setsxVMi[v]] [i] + instCount;
                    B[j][tasksxVM[v][i]+instCurSet+1] = infinite;
//printf("B[%d][%d], ", j, tasksxVM[v][i]+instCurSet+1);
               }
               instCount += instxVMa[v][setsxVMi[v]];
          }
//    printf("\n");
//    printf("inst: %d\n", instCount);

     /* Copy values of B to B1 */
     for (j=instCurSet+1;j<n;j++) {
          i=instCurSet+1;
          while (i<n) {
               B[j][i] = B1[j-instCurSet-1][i-instCurSet-1];
               i++;
          }
     }
	
     /**********************************************
     Allocates memory space for the new 'D' matrix
     **********************************************/
     D = (int **)malloc(n*sizeof(int *));
     if (!D) {
          fprintf(stderr,"Error allocating memory for the new 'D' matrix :'(.\n");
          //return(2);
     }
     for (i=0;i<n;i++) {
          *(D+i)=(int *)malloc(n*sizeof(int));
          if (! (*(D+i)) ) {
               fprintf(stderr,"Error allocating memory [D+i] :'(.\n");
               //return(2);
          }
     }
	
     /***************************	
	 Filling values of array 'B'
     ****************************/
     for (j=0; j<n; j++)
          for (i=0; i<n; i++)
               if(B[j][i] == 0)
	            D[j][i] = 0;
               else
	            D[j][i] = 1;
}

void printData() {
     int v,i,s;

     printf("\n------------------------------\n");
     printf("The application has %d tasks\n", n);

     for(v=0; v<o; v++) {
          printf("\nMV %2d (%2d): ", v, tasksxVMa[v]);

          /* Prints the tasks that will use this VM */
          for(i=0; i<tasksxVMa[v]; i++)
               printf(" %2d", tasksxVM [v][i]);
          printf(" -- %2d sets\n",setsxVMa[v]);

          /* Prints the partition sets for this VM */
          for(s=0; s<setsxVMa[v]; s++) {
               printf("\t    ");
               for(i=0; i<tasksxVMa[v]; i++)
                    printf(" %2d", setsxVM[v][s][i]);
               printf(" -- %2d instantiations\n", instxVMa[v][s]);
          }
     }
}
