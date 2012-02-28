/*  Descrição: Formulação do problema de escalonamento de tarefas em uma grade
 *
 *  Para compilar: make
 *
 *  Script  ./opt/xpressmp/bin/xpvars.sh
 *
 *  Autor: Cesar G. Chaves A. <cesarchaves1@gmail.com>
 *  Data:  17/11/2009
 */

// ./tvms 0 int 20  ../cenarios_icc/applications/montage02.dat ../cenarios_icc/grid/hosts/n20.dat ../cenarios_icc/vpr_10.dat

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "x_io.h"
#include "x_form.h"
#include "x_relax.h"

char progName[10] = "";

/** Return codes:
 *  0: Everything is right
 *  1: Few patameters
 *  2: Unable to read the input file
 *  3: Unable to alloc decision variables
 *  4: Unable to initialize the problem
 *  5: Unable to set constraints
 *  6: Unable to set objective function
 *  7: Incorrect solution algorithm
 **/

void usage();

int main(int argc, char **argv) {

     time(&start_t);

     strcpy(progName, argv[0]);

     /* Verifying the arguments input by the user */
     if (argc < 5 || argc > 7) {
          usage();
          return(1);
     }

     /* Storing the type of method */
     method = atoi(argv[1]);
     if ((method != OPTIMIZER) && (method != BCL)) {
          fprintf(stderr,"Opção inexistente para o método. As opções possíveis são: 0 para o BCL e 1  para o Optimizer\n");
          //return(2);
     }

     timeUnit = atoi(argv[3]);

     /* Determines if only the first 4 constraints of the formulation should be considered*/
     if( !strcmp(argv[2], "dint") || !strcmp(argv[2], "drr") )
          dbatista = 1;
     else
          dbatista = 0;

     /* Reading the input file or files */
     if (argc == 5) {
          if(readInputFile(argv[4])) {
               return(2);
          }
          else {
               time(&end_t);
               progExec_t = difftime(end_t,start_t);
               printf("\n--> Finished reading input data! (t=%lf)\n", progExec_t);
               calcTmax();
               printf("\nTmax: %.4lf\n\n", TmaxReal);
          }
     }
     else if (argc == 7) {
          if(readInputFiles(argv[4], argv[5], argv[6]) ) {
               return(2);
          }
          else {
               time(&end_t);
               progExec_t = difftime(end_t,start_t);
               printf("\n--> Finished reading input data files! (t=%lf)\n", progExec_t);
               calcTmax();
               printf("\nTmax: %f\n\n", TmaxReal);
          }
     }

//	printVars();
//exit(0);
     /* Allocating the decision variables */
     if(allocDecisionVariables()) {
          return(3);
     }
     else {
          time(&end_t);
          progExec_t = difftime(end_t,start_t);
          printf("--> Finished allocating decision variables! (t=%lf)\n", progExec_t);
     }

     /* Initializing the problem */
     if(initializeProblem()) {
          return(4);
     }
     else {
          time(&end_t);
          progExec_t = difftime(end_t,start_t);
          printf("--> Finished initializing the problem! (t=%lf)\n", progExec_t);
     }

     /* Setting problem constraints */
     printf("--> Starting to set problem constraints! (t=%lf)\n", progExec_t);
     if(setConstraints()) {
          return(5);
     }
     else {
          time(&end_t);
          progExec_t = difftime(end_t,start_t);
          printf("--> Finished setting problem constraints! (t=%lf)\n", progExec_t);
     }

     /* Setting objective function */
     if(setObjective()) {
          return(6);
     }
     else {
          time(&end_t);
          progExec_t = difftime(end_t,start_t);
          printf("--> Finished setting objective function! (t=%lf)\n", progExec_t);
     }

     /* Printing input variables just to verify */
     //printVars();

     //printf("============== PROBLEMA ===========\n");
     //XPRBprintprob(problema);
     //printf("===================================\n");

     printf("\n--------------------------------------------------\n");

     XPRSprob problem_optimizer;
     XPRBloadmat(problema);
     problem_optimizer = XPRBgetXPRSprob(problema);
//	XPRSsetintcontrol(problem_optimizer,XPRS_MAXTIME, 1800);
     XPRSsetintcontrol(problem_optimizer,XPRS_MAXTIME, -1200);

     /* If an algorithm from Xpress has been especified, use it!! */
     if ( !strcmp(argv[2], "") || !strcmp(argv[2], "d") || !strcmp(argv[2], "p") || !strcmp(argv[2], "b") || !strcmp(argv[2], "n") || !strcmp(argv[2], "l") || !strcmp(argv[2], "g") ) {
          XPRBminim(problema,argv[2]);
          printResults();
     }
     else if ( !strcmp(argv[2], "int") || !strcmp(argv[2], "INT") )
          intSolve();
     else if ( !strcmp(argv[2], "rr") || !strcmp(argv[2], "RR") || !strcmp(argv[2], "drr"))
          randomizedRnd(1000);

     else if ( !strcmp(argv[2], "irr") || !strcmp(argv[2], "IRR") )
          randomizedRndIt(5);
     else if ( !strcmp(argv[2], "irre") || !strcmp(argv[2], "IRRE") )
          randomizedRndIt_En();
     else if ( !strcmp(argv[2], "mpr") || !strcmp(argv[2], "MPR") )
          maxProbRnd();
     else if ( !strcmp(argv[2], "mpre") || !strcmp(argv[2], "MPRE") )
          maxProbRnd_En();
     else if ( !strcmp(argv[2], "stre") || !strcmp(argv[2], "STRE") )
          shortTimeRnd_En();
     else if ( !strcmp(argv[2], "dint") || !strcmp(argv[2], "DINT") )
          dIntSolve();
     else {
          usage();
          return(7);
     }

     /* Calculates program execution time*/
     time(&end_t);
     progExec_t = difftime(end_t,start_t);
     printf("\nTotal execution time: %lf\n",progExec_t);

     /* Freeing memory resources */
     freeMemory();

     return(0);
}

void usage() {
     fprintf(stderr,"\nUse : %s METHOD ALGORITHM TIME_UNIT SCENERY\n",progName);
     fprintf(stderr,"      %s METHOD ALGORITHM TIME_UNIT APP_FILE GRID_FILE VMR_FILE\n",progName);

     fprintf(stderr,"\nExamples: %s 0 int 20 scenery01\n",progName);
     fprintf(stderr,"          %s 0 rr 20 scenery01\n",progName);
     fprintf(stderr,"          %s 0 int 20 app01 grid01 vmr01\n",progName);

     fprintf(stderr,"\nWhere: METHOD      0 for BCL and 1 for Optimizer\n");
     fprintf(stderr,"       ALGORITHM   int,rr,irr,irre,mpr,mpre,\"\",d,p,b,n,l,g\n");
     fprintf(stderr,"       TIME_UNIT Size of the time slot in seconds\n");
     fprintf(stderr,"       SCENERY     The input file describing the scenery\n");
     fprintf(stderr,"       APP_FILE    The input file describing the application\n");
     fprintf(stderr,"       GRID_FILE   The input file describing the grid infrastructure\n");
     fprintf(stderr,"       VMR_FILE    The input file describing the virtual machine repository\n");


     fprintf(stderr,"\nPossible values of the ALGORITHM flag\n");

     fprintf(stderr,"  int  -  solve the problem using the MIP/MIQP algorithm, and fix time values.\n");

     fprintf(stderr,"  rr   -  solve the initial LP/QP problem using the MIP/MIQP algorithm, and generate an\n");
     fprintf(stderr,"            Integer solution using the Randomized Rounding algorithm.\n");

     fprintf(stderr,"  irr  -  solve the initial LP/QP problem using the MIP/MIQP algorithm, and generate an\n");
     fprintf(stderr,"            Integer solution using the Iterative Randomized Rounding algorithm.\n");

     fprintf(stderr,"  irre -  solve the initial LP/QP problem using the MIP/MIQP algorithm, and generate an\n");
     fprintf(stderr,"            Integer solution using the Iterative Randomized Rounding Enhanced algorithm.\n");

     fprintf(stderr,"  mpr  -  solve the initial LP/QP problem using the MIP/MIQP algorithm, and generate an\n");
     fprintf(stderr,"            Integer solution using the Maximum Probability Rounding algorithm.\n");		

     fprintf(stderr,"  mpre -  solve the initial LP/QP problem using the MIP/MIQP algorithm, and generate an\n");
     fprintf(stderr,"            Integer solution using the Maximum Probability Rounding Enhanced algorithm.\n");


     fprintf(stderr,"  \"\"   -  solve the problem using the recommended LP/QP algorithm (MIP problems\n");
     fprintf(stderr,"             remain in presolved state)\n");
     fprintf(stderr,"  d    -  solve the problem using the dual simplex algorithm\n");
     fprintf(stderr,"  p    -  solve the problem using the primal simplex algorithm\n");
     fprintf(stderr,"  b    -  solve the problem using the Newton barrier algorithm\n");
     fprintf(stderr,"  n    -  use the network solver (LP only)\n");
     fprintf(stderr,"  l    -  relax all global entities (integer variables etc) in a MIP/MIQP problem and\n");
     fprintf(stderr,"            solve it as a LP problem (problem is postsolved)\n");
     fprintf(stderr,"  g    -  solve the problem using the MIP/MIQP algorithm. If a MIP/MIQP problem\n");
     fprintf(stderr,"            is solved without this flag, only the initial LP/QP problem will be solved.\n");
}
