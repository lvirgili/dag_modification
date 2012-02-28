#ifndef X_IO
#define X_IO

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <time.h>

int timeUnit;

/* Where to print the results */
char fileName[50];

/* Execution duration */
time_t start_t, end_t;
double progExec_t;

/* ------------------- Constantes do problema ------------------ */

int n; //number of tasks
int m; //number of hosts
int o; //number of virtual machines
int Tmax; //Rounded Maximum scheduling time
float TmaxReal; //Maximum scheduling time

//Parameters related to Tasks
int    * S; //Software demand of tasks (VM's ID)
double * I; //Processing demand of tasks (Number of instructions)
double **B; //Number of byte transmitted between tasks
int    **D; //Set of arcs between tasks

//Parameters related to Hosts
int    * C;  //Number of processing cores per host
double * TI; //Time each host takes to execute one instruction
double **TB; //Time for transmitting 1 byte between hosts
int    **N;  //Set of links between hosts

//Parameters related to Virtual Machines
int    * SV; //Software ofering of virtual Machines
double * BV; //Size of virtual Machines
double * TR; //Time for transmitting 1 byte between the VMs repository and a hosts
double * TV; //Time taken for booting the virtual Machines


//Methods
int readInputFile(char* nome);
int readInputFiles(char* appFile, char* gridFile, char* vmrFile);
void calcTmax(void);
void calcTmaxOld(void);
void printVars(void);
void printApp(void);
void printApp2File(char * fileName);
int myrand (int min, int max);
void freeVars(void);

#endif
