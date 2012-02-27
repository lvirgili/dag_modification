#ifndef X_FORM
#define X_FORM

#define BCL 0
#define OPTIMIZER 1

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <xprb.h>
#include <xprs.h>
#include "x_io.h"

/* Input method. Either BCL or OPTIMIZER */
int method;

/* Boolean variable that indicates that only the first 4 constraints
 * should be considered.
 * */
int dbatista;

char buffer[10000];

/* ----------------- Variables to store the solution ---------------- */

/* Variable that assumes a value of 1 if task i finished on time t, on host k */
XPRBvar ***x;
/* x: array(1..n, 1..T, 1..m) */

/* Variable that assumes a value of 1 if VM v is ready to be used on time t, on host k */
XPRBvar ***y;
/* x: array(1..o, 1..T, 1..m) */

/* problem to be solved */
XPRBprob problema;
/* Restrictions for problem solution */
XPRBctr ctr;

/* Other variables */
/* Vector X stores the host where task i will be executed */
int * X;
/* Vector f stores the finishing time of every task */
double * f;
/* Vector f1 stores the calculated finishing time of every task */
double * f1;
/* Vector Y stores the virtual machine where task i will be executed */
int * Y;
/* Time needed to copy the image file of the virtual machine that
 * will execute task i to the host that will run it
 */
double * d;
/* Times at which the virtual machine where task i will be available
 * and unavailable, respectively
 */
double * Ya, * Yu;

/* Functions */
int allocDecisionVariables(void);
int initializeProblem(void);
int setConstraints(void);
int setObjective(void);
void intSolve(void);
void dIntSolve(void);
void fixTimeValues(void);
void fixTimeValues1(void);
void printResults(void);
void printResultsBuffer(void);
void printResultsBuffer1(void);
void printResultsBufferOLD(void);
void freeMemory(void);

#endif
