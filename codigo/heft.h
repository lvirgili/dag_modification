#ifndef HEFT
#define HEFT

#include <stdio.h>

double meanTI; /* Mean time to process one instruction on the grid*/
double meanTB; /* Mean time to transmit one MB on the grid */

double * rankU; /* Length of the critical path from a task to the end task */
double * rankD; /* Length of the critical path to a task from the entry task */

int * rankUindex; /* Sorts tasks according to task ranks */

double * AST; /* Starting time of tasks */
double * AFT; /* Finishing time of tasks */
double * avail; /* Time slot when hosts become available */
int * hostxTask;

int allocVars(void);
void calcMeans(void);
double calcRankU(int task);
double calcRankD(int task);
double EFT(int task, int host);
double EST(int task, int host);

void mergeSort(int * index, double * vector, int beg, int end);
void merge(int * index, double * vector, int beg, int med, int end);

#endif
