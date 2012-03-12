/*
 *  Author: Cesar G. Chaves A. <cesarchaves1@gmail.com>
 *  Date:  05/08/2010
 */

#ifndef X_DAG
#define X_DAG

#include<stdio.h>
#include<stdlib.h>


/* Parameters related to Tasks */
int      n1; /* Amount of tasks of the unmodified DAG */
double * I1; /* Processing demand of tasks (Number of instructions) */
double **B1; /* Number of byte transmitted between tasks */
int    **D1; /* Set of arcs between tasks */

/* Parameters related to Hosts */
int    * C1;  /* Number of processing cores per host */
double * TI1; /* Time each host takes to execute one instruction */
double **TB1; /* Time for transmitting 1 byte between hosts */
int    **N1;  /* Set of links between hosts */


double infinite; //Numeric value for infinite


int mdfDagOpc1();
int mdfDagOpc2();

int *  tasksxVMa; /* Amount of tasks that will use each VM */
int ** tasksxVM;  /* Stores the task ID of tasks that will use each VM */

int idSet;
int *   setsxVMa; /* Amount of sets per VM */
int **  instxVMa; /* Amount of instantiations per VM per set partition */
int *** setsxVM;  /* Sets per VM */

int * c; /* Codeword */
void SP(int m, int p, int idPart, int n); /* Calculates the groups of tasks */
void storeSet(int partition); /* Prints the groups of tasks */
void calcSetsCartProd(); /* Calculates the Cartesian Product of the VM partition sets */
void createDAG(int * setsxVMi); /* Creates a new DAG with for a given Cartesian Product result */
void printData();

#endif
