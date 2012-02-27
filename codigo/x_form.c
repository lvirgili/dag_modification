#include "x_form.h"

int allocDecisionVariables(void) {
	int i,j;

	/* Allocation of X */
	x = (XPRBvar ***)malloc(n*sizeof(XPRBvar **));
	if (!x) {
		fprintf(stderr,"Error allocating memory [x] :'(.\n");
		return(2);
	}
	for (i=0;i<n;i++) {
		*(x+i)=(XPRBvar **)malloc((Tmax+3)*sizeof(XPRBvar *));
		if (! (*(x+i)) ) {
			fprintf(stderr,"Error allocating memory [x+i] :'(.\n");
			return(2);
		}
		for (j=1;j<=Tmax;j++) {
			*(*(x+i)+j)=(XPRBvar *)malloc(m*sizeof(XPRBvar));
			if (! (*(*(x+i)+j)) ) {
				fprintf(stderr,"Error allocating memory [x+i+j] :'(.\n");
				return(2);
			}
		}
	}

	/* Allocation of Y */
	y = (XPRBvar ***)malloc(o*sizeof(XPRBvar **));
	if (!y) {
		fprintf(stderr,"Error allocating memory [y] :'(.\n");
		return(2);
	}
	for (i=0;i<o;i++) {
		*(y+i)=(XPRBvar **)malloc(Tmax*sizeof(XPRBvar *));
		if (! (*(y+i)) ) {
			fprintf(stderr,"Error allocating memory [y+i] :'(.\n");
			return(2);
		}
		for (j=1;j<=Tmax;j++) {
			*(*(y+i)+j)=(XPRBvar *)malloc(m*sizeof(XPRBvar));
			if (! (*(*(y+i)+j)) ) {
				fprintf(stderr,"Error alocating memory [y+i+j] :'(.\n");
				return(2);
			}
		}
	}

	return(0);
}

int initializeProblem(void) {
	char NOME[20];
	int i,t,k,v;

	/* Creating the problem */
	XPRBinit();
	problema=XPRBnewprob("ts");

	/* Linking variable X with the problema and defining that it will be binary */
	for (i=0;i<n;i++) {
		for (t=1;t<=Tmax;t++) {
			for (k=0;k<m;k++) {
				sprintf(NOME,"x_%d_%d_%d",i,t,k);
				x[i][t][k]=XPRBnewvar(problema,XPRB_BV,NOME,0,1);
				//x[i][t][k]=XPRBnewvar(problema,XPRB_PL,NOME,0,1);
			}
		}
	}

	/* Linking variable Y with the problema and defining that it will be binary */
	for (v=0;v<o;v++) {
		for (t=1;t<=Tmax;t++) {
			for (k=0;k<m;k++) {
				sprintf(NOME,"y_%d_%d_%d",v,t,k);
				y[v][t][k]=XPRBnewvar(problema,XPRB_BV,NOME,0,1);
			}
		}
	}

	return(0);
}

int setConstraints(void) {

	int i,j,k,l,t,s,v; //,w;
	int sumMaxVal;
	char NOME[20];

	int const_count; /* Stores the count of constraits created in each group */


	/*******************************
	 * Constraints regarding tasks *
	 *******************************/

	/* --------------------  Constraints (C1)  --------------------
	 * Every task has to be executed and could only be executed once
	 */
	const_count=0;
	/* For all task in J */
	for (i=0;i<n;i++) {
		sprintf(NOME,"Const1_%d",i);
		ctr=XPRBnewctr(problema,NOME,XPRB_E);
		const_count++;
		/* For all time units in T */
		for (t=1;t<=Tmax;t++)
			/* For all host in H */
			for (k=0;k<m;k++)
				XPRBaddterm(ctr,x[i][t][k],1);
		XPRBaddterm(ctr,NULL,1);
	}
	printf("----> Total constraints (C1) = %d\n", const_count);


	/* --------------------  Constraints (C2)  --------------------
	 * A task execution can't be completed until all it's instructions
	 * have been processed
	 */
	const_count=0;
	sprintf(NOME,"Const2");
	ctr=XPRBnewctr(problema,NOME,XPRB_E);
	const_count++;
	/* For all task in J */
	for (i=0;i<n;i++) {
		/* For all host in H */
		for (k=0;k<m;k++) {
			sumMaxVal = ceil(I[i]*TI[k]);
			for (t=1;t<=sumMaxVal;t++) {
				XPRBaddterm(ctr,x[i][t][k],1);
			}
		}
	}
	XPRBaddterm(ctr,NULL,0);
	printf("----> Total constraints (C2) = %d\n", const_count);

	/*
	 * OLD version of (C2) that created more constraints
	 */
	/*for (i=0;i<n;i++) { //Tasks
		for (k=0;k<m;k++) { //Hosts
			for (t=1;t<=ceil(I[i]*TI[k]);t++) { //Time
				sprintf(NOME,"Const2_%d_%d_%d",i,t,k);
				ctr=XPRBnewctr(problema,NOME,XPRB_E);
				XPRBaddterm(ctr,x[i][t][k],1);
				XPRBaddterm(ctr,NULL,0);
			}
		}
	}*/


	/* --------------------  Constraints (C3)  --------------------
	 * A task execution may begin only after it's predecessors have finish,
	 * and all required data has been received.
	 */
	const_count=0;
	/* For all task in J */
	for (i=0;i<n;i++) {
		/* For all task in J */
		for (j=0;j<n;j++) {
			/* If task i precedes task j */
			if (D[i][j] == 1) {
				/* For all host in H */
				for (l=0;l<m;l++) {
					/* For all time units in T */
					for (t=1;t<=Tmax;t++) {
						sprintf(NOME,"Const3_%d_%d_%d_%d",i,j,l,t);
						ctr=XPRBnewctr(problema,NOME,XPRB_G);
						const_count++;
						/* For all host in H */
						for (k=0;k<m;k++) {
							/* If host k is linked to host l */
							if (N[k][l] == 1) {
								sumMaxVal = t-ceil(I[j]*TI[l]+B[i][j]*TB[k][l]);
								/* For all time units between 1 and sumMaxVal */
								for (s=1;s<=sumMaxVal;s++) {
									XPRBaddterm(ctr,x[i][s][k],1);
								}
							}
						}
						/* For all time units between 1 and t */
						for (s=1;s<=t;s++) {
							XPRBaddterm(ctr,x[j][s][l],-1);
						}
						XPRBaddterm(ctr,NULL,0);
					}
				}
			}
		}
	}
	printf("----> Total constraints (C3) = %d\n", const_count);


	/* --------------------  Constraints (C4)  --------------------
	 * Each host can only execute one task per CPU
	 */
	const_count=0;
	/* For all host in H */
	for (k=0;k<m;k++) {
		/* For all time units in T */
		for (t=1;t<=Tmax;t++) {
			sprintf(NOME,"Const4_%d_%d",k,t);
			ctr=XPRBnewctr(problema,NOME,XPRB_L);
			const_count++;
			/* For all task in J */
			for (j=0;j<n;j++) {
				if( t <= Tmax-ceil(I[j]*TI[k]) ) {
					sumMaxVal= t + ceil(I[j]*TI[k]-1);
					for (s=t;s<=sumMaxVal;s++) { //Time
						XPRBaddterm(ctr,x[j][s][k],1);
					}
				}
			}
			XPRBaddterm(ctr,NULL,C[k]);
		}
	}
	printf("----> Total constraints (C4) = %d\n", const_count);


	if(!dbatista) {
		/******************************************
		 * Constraints regarding Virtual Machines *
		 ******************************************/

		/* --------------------  Constraints (C6)  --------------------
		 * A Host can instaciate at most one VM per CPU at any given time
		 */
		const_count=0;
		/* For all host in H */
		for (k=0;k<m;k++) {
			/* For all time units in T */
			for (t=1;t<=Tmax;t++) {
				sprintf(NOME,"Const6_%d_%d",k,t);
				ctr=XPRBnewctr(problema,NOME,XPRB_L);
				const_count++;
				/* For all Virtual Machines in V */
				for (v=0;v<o;v++) {
					XPRBaddterm(ctr,y[v][t][k],1);
				}
				XPRBaddterm(ctr,NULL,C[k]);
			}
		}
		printf("----> Total constraints (C6) = %d\n", const_count);


		/* --------------------  Constraints (C7)  --------------------
		 * A VM isn't ready to be used until it has been copied to the host
		 * and has booted
		 */
		const_count=0;
		sprintf(NOME,"Const7");
		ctr=XPRBnewctr(problema,NOME,XPRB_E);
		const_count++;
		/* For all Virtual Machines in V */
		for (v=0;v<o;v++) {
			/* For all host in H */
			for (k=0;k<m;k++) {
				sumMaxVal = ceil(BV[v]*TR[k]+TV[v]);
				/* For all time units between 1 and sumMaxVal */
				for (t=1;t<=sumMaxVal;t++) {
					XPRBaddterm(ctr,y[v][t][k],1);
				}
			}
		}
		XPRBaddterm(ctr,NULL,0);
		printf("----> Total constraints (C7) = %d\n", const_count);

		/*
		 * OLD version of (C7) that created more constraints
		 */
		/*for (v=0;v<o;v++) { //Virtual machines
		for (k=0;k<m;k++) { //Hosts
			for (t=1;t<=ceil(BV[v]*TR[k]+TV[v]);t++) { //Time
				sprintf(NOME,"Const7_%d_%d_%d",v,t,k);
				ctr=XPRBnewctr(problema,NOME,XPRB_E);
				XPRBaddterm(ctr,y[v][t][k],1);
				XPRBaddterm(ctr,NULL,0);
			}
		}
	}*/


		/**********************************************************
		 * Constraints linking Virtual Machines to task execution *
		 **********************************************************/

		/* --------------------  Constraints (C9)  --------------------
		 * A task execution can't finish until the VMs that will run
		 * depending following tasks are ready to be used.
		 */

		const_count=0;
		/*	int w;
	// For all task in J
	for (i=0;i<n;i++) {
		// For all task in J
		for (j=0;j<n;j++) {
			// If task i precedes task j
			if (D[i][j] == 1) {
				// For all Virtual Machines in V
				for (w=0;w<o;w++) {
					// If task and VM software match
					if (S[j] == SV[w]) {
						// For all host in H
						for (k=0;k<m;k++) {
							// For all time units in T
							for (t=1;t<=Tmax;t++) {
								sprintf(NOME,"Const9_%d_%d_%d_%d_%d",i,j,w,k,t);
								ctr=XPRBnewctr(problema,NOME,XPRB_G);
								const_count++;
								// For all host in H
								for (l=0;l<m;l++) {
									 //If host k is linked to host l
									if (N[k][l] == 1) {
										XPRBaddterm(ctr,y[w][t][l],1);
									}
								}
								XPRBaddterm(ctr,x[i][t][k],-1);
								XPRBaddterm(ctr,NULL,0);
							}
						}
					}
				}
			}
		}
	}
		 */	printf("----> Total constraints (C9) = %d\n", const_count);

		 /* --------------------  Constraints (C10)  --------------------
		  * A task execution may begin only after the required VM is ready to be used
		  * ( (C10) is apparently not necessary, (C3) and (C13) do it's work )
		  */
		 const_count=0;
		 /*
	 for (i=0;i<n;i++) {
		 for (j=0;j<n;j++) {
			 if (D[i][j] == 1) {
				 for (w=0;w<o;w++) {
					 if (S[j] == SV[w]) {
						 for (l=0;l<m;l++) {
							 for (t=1;t<=Tmax;t++) {
								 sprintf(NOME,"Const10_%d_%d_%d_%d_%d",i,j,w,l,t);
								 ctr=XPRBnewctr(problema,NOME,XPRB_G);
								 const_count++;
								 for (k=0;k<m;k++) {
									 if (N[k][l] == 1) {
										 for (s=1;s<=t-ceil(I[j]*TI[l]+B[i][j]*TB[k][l]);s++) { //Time
											 XPRBaddterm(ctr,y[w][s][l],1);
										 }
									 }
								 }
								 XPRBaddterm(ctr,x[j][t][l],-1);
								 XPRBaddterm(ctr,NULL,0);
							 }
						 }
					 }
				 }
			 }
		 }
	 }
		  */
		 printf("----> Total constraints (C10) = %d\n", const_count);


		 /* --------------------  Constraints (C11)  --------------------
		  * The execution of the input task may begin only after the required
		  * VM is ready to be used
		  * ( (C11) is apparently not necessary, (C13) does it's work )
		  */
		 const_count=0;
		 /*
	 for (v=0;v<o;v++) {
		 if (S[0] == SV[v]) {
			 for (t=1;t<=Tmax;t++) {
				 sprintf(NOME,"Const11_%d_%d",v,t);
				 ctr=XPRBnewctr(problema,NOME,XPRB_G);
				 const_count++;
				 for (k=0;k<m;k++) {
					 //                    if (N[k][l] == 1) {
					 for (s=1;s<=t-ceil(I[0]*TI[k]);s++) {
						 XPRBaddterm(ctr,y[v][s][k],1);
					 }
					 //                    }
				 }
				 for (k=0;k<m;k++) {
					 XPRBaddterm(ctr,x[0][t][k],-1);
				 }
				 XPRBaddterm(ctr,NULL,0);
			 }
		 }
	 }
		  */
		 printf("----> Total constraints (C11) = %d\n", const_count);


		 /* --------------------  Constraints (C12)  --------------------
		  * A VM may only be instantiated on a host if, and only if, this
		  * host will execute a task requiring it
		  */
		 const_count=0;
		 /* For all host in H */
		 for (k=0;k<m;k++) {
			 /* For all Virtual Machines in V */
			 for (v=0;v<o;v++) {
				 sprintf(NOME,"Const12_%d_%d",k,v);
				 ctr=XPRBnewctr(problema,NOME,XPRB_G);
				 const_count++;
				 /* For all time units in T */
				 for (t=1;t<=Tmax;t++) {
					 /* For all task in J */
					 for (i=0;i<n;i++) {
						 /* If task and VM software match */
						 if (S[i] == SV[v]) {
							 XPRBaddterm(ctr,x[i][t][k],Tmax);
						 }
					 }
				 }
				 /* For all time units in T */
				 for (t=1;t<=Tmax;t++) {
					 XPRBaddterm(ctr,y[v][t][k],-1);
				 }
				 XPRBaddterm(ctr,NULL,0);
			 }
		 }
		 printf("----> Total constraints (C12) = %d\n", const_count);


		 /* --------------------  Constraints (C13)  --------------------
		  * A VM must stay active at least as long as the tasks requiring it have
		  * finish their execution
		  */
		 const_count=0;
		 /* For all task in J */
		 for (i=0;i<n;i++) {
			 /* For all Virtual Machines in V */
			 for (v=0;v<o;v++) {
				 if (S[i] == SV[v]) {
					 /* For all host in H */
					 for (k=0;k<m;k++) {
						 for (t=ceil(I[i]*TI[k])+1;t<=Tmax;t++) {
							 sprintf(NOME,"Const13_%d_%d_%d_%d",i,v,k,t);
							 ctr=XPRBnewctr(problema,NOME,XPRB_G);
							 const_count++;
							 for (s=t-ceil(I[i]*TI[k]);s<=t;s++) {
								 XPRBaddterm(ctr,y[v][s][k],1);
							 }
							 XPRBaddterm(ctr,x[i][t][k],-(ceil(I[i]*TI[k])+1));
							 XPRBaddterm(ctr,NULL,0);
						 }
					 }
				 }
			 }
		 }
		 printf("----> Total constraints (C13) = %d\n", const_count);
	}
	return(0);
}

int setObjective(void) {

	int j,k;

	/* Função objetivo */
	ctr=XPRBnewctr(problema,"Objective",XPRB_N);
	for (j=1;j<=Tmax;j++) {
		for (k=0;k<m;k++) {
			XPRBaddterm(ctr,x[n-1][j][k],j);
		}
	}
	XPRBsetobj(problema,ctr);
	XPRBsetsense(problema,XPRB_MINIM);

	return(0);
}

void intSolve(void) {
	int i, k, t;
	/* Allocs memory for vector X */
	X = (int *)malloc(n*sizeof(int));
	/* Allocs memory for vector f */
	f = (double *)malloc(n*sizeof(double));
	/* Allocs memory for vector Y */
	Y = (int *)malloc(n*sizeof(int));

	printf("\nChamando solução do problema!!\n");
	XPRBminim(problema, "g");
	//printResults();
	printf("\nProblema solucionado!!\n");
	if (XPRBgetmipstat(problema) != XPRB_MIP_INFEAS) {
		for (i=0; i<n; i++) {
			for (k=0;k<m;k++) {
				for (t=1;t<=Tmax;t++) {
					if(XPRBgetsol(x[i][t][k]) > 0) {
						X[i] = k;
						f[i] = t * timeUnit;
						Y[i] = S[i] - 1;
					}
				}

			}
		}
		/*  */
		printf("\nPronto para caulcular tempos!!\n");
		fixTimeValues();
		printResultsBuffer();

		/* Prints the solution with the lowest makespan */
		printf("%s", buffer);
	}
}

void dIntSolve(void) {
	int i, k, t;
	/* Allocs memory for vector X */
	X = (int *)malloc(n*sizeof(int));
	/* Allocs memory for vector f */
	f = (double *)malloc(n*sizeof(double));

	printf("\nChamando solução do problema!!\n");
	XPRBminim(problema, "g");
	//printResults();
	printf("\nProblema solucionado!!\n");
	if (XPRBgetmipstat(problema) != XPRB_MIP_INFEAS) {
		for (i=0; i<n; i++) {
			for (k=0;k<m;k++) {
				for (t=1;t<=Tmax;t++) {
					if(XPRBgetsol(x[i][t][k]) > 0) {
						X[i] = k;
						f[i] = t * timeUnit;
					}
				}

			}
		}
		/*  */
		printf("\nPronto para caulcular tempos!!\n");
		fixTimeValues1();
		printResultsBuffer();

		/* Prints the solution with the lowest makespan */
		printf("%s", buffer);
	}
}

void  fixTimeValues(void) {
	int i, j;
	/* Counter for the amount of scheduled tasks */
	int scheduled;
	/* Feasible starting time of a task i */
	double s;
	/* Variable to store temporary values */
	double temp;

	/* Allocs memory for vector d: Instantiation delay of VMs */
	d = (double *)malloc(n*sizeof(double));
	/* Allocs memory for vector f1 */
	f1 = (double *)malloc(n*sizeof(double));
	/* Allocs memory for vector Ya: VM availability starting time */
	Ya = (double *)malloc(n*sizeof(double));
	/* Allocs memory for vector Yu: VM availability ending time (unavailable)*/
	Yu = (double *)malloc(n*sizeof(double));

	/* Initialization of vector of calculated ending time*/
	for (i=0;i<n;i++) {
		f1[i] = -1;
		d[i] = BV[Y[i]] * TR[X[i]] * timeUnit;
	}

	f1[0] = d[0] + (TV[0] + I[0] * TI[X[0]]) * timeUnit;
	Ya[0] = d[0] + (TV[0] * timeUnit);
	Yu[0] = f1[0];

	i=1;
	scheduled=1;
	/* Repeat until every task has been scheduled */
	while (scheduled!=n) {
		if ( f1[i] == -1 ) {

			s = d[i] + TV[i] * timeUnit;
			for (j=0;j<i;j++) {
				/* If tasks i and j will run on the same host */
				if (X[i] == X[j]) {
					/* If task j has already been scheduled */
					if (f1[j] != -1) {
						/* If tasks i and j will run on the same VM */
						if (Y[i] == Y[j]) {
							s = f1[j];
						}
						else {
							temp = f1[j] + TV[i] * timeUnit;
							s = s > temp ? s : temp;
						}
					}
					else
						/* Since task i depends of task j, and task j hasn't
						 * been schedule, task i can't be scheduled for now
						 */
						break;
				}
				/* If task i depends of task j */
				else if (D[j][i]) {
					/* If task j has already been scheduled */
					if (f1[j] != -1) {
						temp = f1[j] + B[j][i] * TB[X[j]][X[i]] * timeUnit;
						s = s > temp ? s : temp;
					}
					else
						break;
				}
			}

			f1[i] = s + (I[i] * TI[X[i]] * timeUnit);
			scheduled++;
			/* Time at which the VM that will run task i will start to be available */
			Ya[i] = s;
			/* Time at which the VM that will run task i will stop to be available */
			Yu[i] = f1[i];
		}
		i = (i+1) % n;
	}

}

void  fixTimeValues1(void) {
	int i, j;
	/* Counter for the amount of scheduled tasks */
	int scheduled;
	/* Feasible starting time of a task i */
	double s;
	/* Variable to store temporary values */
	double temp;

	/* Allocs memory for vector f1 */
	f1 = (double *)malloc(n*sizeof(double));

	/* Initialization of vector of calculated ending time*/
	for (i=0;i<n;i++)
		f1[i] = -1;

	f1[0] = (I[0] * TI[X[0]]) * timeUnit;

	i=1;
	scheduled=1;
	/* Repeat until every task has been scheduled */
	while (scheduled!=n) {
		if ( f1[i] == -1 ) {
			s = 0;
			for (j=0;j<i;j++) {
				/* If tasks i and j will run on the same host */
				if (X[i] == X[j]) {
					/* If task j has already been scheduled */
					if (f1[j] != -1)
						s = f1[j];
					else
						/* Since task i depends of task j, and task j hasn't
						 * been schedule, task i can't be scheduled for now
						 */
						break;
				}
				/* If task i depends of task j */
				else if (D[j][i]) {
					/* If task j has already been scheduled */
					if (f1[j] != -1) {
						temp = f1[j] + B[j][i] * TB[X[j]][X[i]] * timeUnit;
						s = s > temp ? s : temp;
					}
					else
						break;
				}
			}

			f1[i] = s + (I[i] * TI[X[i]] * timeUnit);
			scheduled++;
		}
		i = (i+1) % n;
	}
}

void printResultsBuffer(void) {
	int i, j;

	sprintf(buffer, "\n--------------------------------------------------\n");
	sprintf(buffer, "%s\nResults: \n=======\n", buffer);
    sprintf(buffer, "%s\nTasks Schedule: \n--------------\n", buffer);

    for (i=0;i<n;i++) {
		sprintf(buffer, "%sTask %d: ", buffer, i);
		sprintf(buffer, "%stakes %.4lf s on host %d ", buffer, I[i]*TI[X[i]]*timeUnit, X[i]);

		if (!dbatista)
		    sprintf(buffer, "%susing VM %d ", buffer, Y[i]);

		sprintf(buffer, "%sstarts at t = %.4lf ", buffer, (f1[i]-(I[i]*TI[X[i]]*timeUnit) ));
		sprintf(buffer, "%sand finishes at t = %.4lf ", buffer, f1[i]);
		sprintf(buffer, "%s\n", buffer);
	}
    //sprintf(buffer, "%s---------------------------\n\n", buffer);

    // Prints the time at which data transmition between tasks edns
	sprintf(buffer, "%s\nTask Data Transmitions: \n----------------------\n", buffer);
	for (i=0;i<n;i++)
		for (j=i;j<n;j++)
			if (D[i][j]==1) {
				sprintf(buffer, "%sfTx[%d][%d]= %.4lf", buffer, i, j, f1[i] + B[i][j]*TB[X[i]][X[j]] * timeUnit);
				if (X[i] == X[j])
					sprintf(buffer, "%s |SEM REDE\n", buffer);
				else
					sprintf(buffer, "%s |COM REDE\n", buffer);
			}
	//sprintf(buffer, "%s--------------------------------------------------\n\n", buffer);

	if (!dbatista) {
		sprintf(buffer, "%s \nVMs Schedule: \n------------\n", buffer);
		for (i=0;i<n;i++) {
			sprintf(buffer, "%sOn host %d , MV %d is   available at t= %.4lf\n", buffer, X[i], Y[i], Ya[i]);
			sprintf(buffer, "%sOn host %d , MV %klççod is UNavailable at t= %.4lf\n", buffer, X[i], Y[i], Yu[i]);
		}
	}

	sprintf(buffer, "%s\nMakespan: %.4lf\n", buffer, f1[n-1]);
	//sprintf(buffer, "%s\n--------------------------------------------------\n", buffer);
}

void printResultsBuffer1(void) {
	int i, j;

	sprintf(buffer, "\n--------------------------------------------------\n");
	sprintf(buffer, "%s\nResults: \n=======\n", buffer);
    sprintf(buffer, "%s\nTasks Schedule: \n--------------\n", buffer);

    for (i=0;i<n;i++) {
		sprintf(buffer, "%sTask %d: ", buffer, i);
		sprintf(buffer, "%stakes %.4lf s on host %d ", buffer, I[i]*TI[X[i]]*timeUnit, X[i]);
//		sprintf(buffer, "%susing VM %d ", buffer, Y[i]);
		sprintf(buffer, "%sstarts at t = %.4lf ", buffer, (f1[i]-(I[i]*TI[X[i]]*timeUnit) ));
		sprintf(buffer, "%sand finishes at t = %.4lf ", buffer, f1[i]);
		sprintf(buffer, "%s\n", buffer);
	}
    //sprintf(buffer, "%s---------------------------\n\n", buffer);

    // Prints the time at which data transmition between tasks edns
	sprintf(buffer, "%s\nTask Data Transmitions: \n----------------------\n", buffer);
	for (i=0;i<n;i++)
		for (j=i;j<n;j++)
			if (D[i][j]==1) {
				sprintf(buffer, "%sfTx[%d][%d]= %.4lf", buffer, i, j, f1[i] + B[i][j]*TB[X[i]][X[j]] * timeUnit);
				if (X[i] == X[j])
					sprintf(buffer, "%s |SEM REDE\n", buffer);
				else
					sprintf(buffer, "%s |COM REDE\n", buffer);
			}
	//sprintf(buffer, "%s--------------------------------------------------\n\n", buffer);
/*
	sprintf(buffer, "%s \nVMs Schedule: \n------------\n", buffer);
	for (i=0;i<n;i++) {
	    sprintf(buffer, "%sOn host %d , MV %d is   available at t= %.4lf\n", buffer, X[i], Y[i], Ya[i]);
	    sprintf(buffer, "%sOn host %d , MV %d is UNavailable at t= %.4lf\n", buffer, X[i], Y[i], Yu[i]);
	}
*/
	sprintf(buffer, "%s\nMakespan: %.4lf\n", buffer, f1[n-1]);
	//sprintf(buffer, "%s\n--------------------------------------------------\n", buffer);
}

void printResults(void) {
	int i,j,t,k,v;

	/* Stores the Id of the host where the ith task was scheduled */
	int * task_host;
	/* Allocs memory for vector task_host */
	task_host = (int *)malloc(n*sizeof(int));

	/* Stores the time at which the ith task finishes */
	int * fTask;
	/* Allocs memory for vector fTask */
	fTask = (int *)malloc(n*sizeof(int));

	printf("\n---------------------------\n");
	printf("\nResults: \n=======\n\n");

	if (method == BCL) {
		//if (XPRBgetmipstat(problema) == XPRB_MIP_OPTIMAL) {
		if ( (XPRBgetlpstat(problema) == XPRB_LP_OPTIMAL) || (XPRBgetmipstat(problema) == XPRB_MIP_OPTIMAL) ) {
			// Prints the scheduling result for tasks on hosts
			printf("\nTasks Schedule: \n--------------\n");
			for (i=0;i<n;i++) {
				for (t=1;t<=Tmax;t++) {
					for (k=0;k<m;k++) {
						if(XPRBgetsol(x[i][t][k]) > 0) {
							printf("Task %d: ", i);
							printf("takes %d s on host %d ", (int)ceil(I[i]*TI[k])*timeUnit, k);//
							printf("using VM %d ", S[i]-1);
							printf("starts at t = %d ", (t-(int)ceil(I[i]*TI[k]))*timeUnit );//
							printf("and finishes at t = %d", t*timeUnit);//
							if (XPRBgetlpstat(problema) == XPRB_LP_OPTIMAL)
								printf(" P = %f", XPRBgetsol(x[i][t][k]));
							printf("\n");

							task_host[i] = k; //stores the executioner host for each task
							fTask[i] = t*timeUnit;//
							/*
					        printf("|s[0%d]=%d\n", i, t-(int)ceil(I[i]*TI[k]));
					        printf("|x[0%d:0%d:0%d]= 1\n", i, k, S[i]-1);
					        printf("|tempoProcessamento: %lf\n", ceil(I[i]*TI[k]));
					        printf("|tempoInstanciar MV%d no host%d: %lf\n", S[i]-1, k, ceil(TR[k]*BV[S[i]-1]) );
					        printf("f[0%d]= %d\n\n", i, t);
							 */
						}
					}
				}
			}
			printf("---------------------------\n\n");


			// Prints the time at which data transmition between tasks ends
			printf("\nTask Data Transmitions: \n------------\n");
			for (i=0;i<n;i++)
				for (j=i;j<n;j++)
					if (D[i][j]==1) {
						printf("fTx[%d][%d]= %d", i, j, fTask[i] + (int)ceil(B[i][j]*TB[task_host[i]][task_host[j]])*timeUnit);
						if (task_host[i] == task_host[j])
							printf("|SEM REDE\n");
						else
							printf("|COM REDE\n");
					}
			printf("---------------------------\n\n");


			// Prints the scheduling result for virtual machines on hosts
			printf("\nVMs Schedule: \n------------\n");
			float val; // stores the value of XPRBgetsol(y[v][t][k])
			for (k=0;k<m;k++) {
				for (v=0;v<o;v++) {
					for (t=2;t<=Tmax;t++) { //t starts in 2 because each timeslot is compared with the previous
						val = (float)XPRBgetsol(y[v][t][k]);
						if(val != (float)XPRBgetsol(y[v][t-1][k])) {
							if(val != 0)
								printf("On host %d , MV %d is   available at t= %d\n", k, v, t*timeUnit);//
							else
								printf("On host %d , MV %d is UNavailable at t= %d\n", k, v, t*timeUnit);//
						}
					}
				}
				//printf("--------------------\n");
			}
			printf("---------------------------\n\n");
			/*
            // Prints the scheduling result for virtual machines on hosts
	        int val; // stores the value of XPRBgetsol(y[v][t][k])
	        printf("\n\n");
	        for (k=0;k<m;k++) {
	            printf("Host %d:\n", k);
		        for (v=0;v<o;v++) {
    		        printf("\tMV %d:\n", v);
                	for (t=2;t<=Tmax;t++) { //t starts in 2 because each timeslot is compared with the previous
				        val = (int)XPRBgetsol(y[v][t][k]);
				        if(val != (int)XPRBgetsol(y[v][t-1][k])) {
					        if(val == 1)
						        printf("\t\t  available at t=%d\n", t);
					        else
						        printf("\t\tUNavailable at t=%d\n", t);
				        }
			        }
		        }
		        printf("--------------------\n");
	        }
			 */
			printf("F= %d\n", fTask[n-1]);
		}
		else
			if (XPRBgetmipstat(problema) == XPRB_MIP_SOLUTION)
				printf("\n*** Unable to find an optimal solution for the problem ***\n");
			else if (XPRBgetmipstat(problema) == XPRB_MIP_INFEAS)
				printf("\n*** It's impossible to resolve the problem ***\n");
	}
	//printf("Tmax: %d\n", Tmax*timeUnit);
}

void printResultsBufferOLD(void) {
	int i,j,t,k,v;

	/* Stores the Id of the host where the ith task was scheduled */
	int* task_host;
	/* Allocs memory for vector task_host */
	task_host = (int *)malloc(n*sizeof(int));

	/* Stores the time at which the ith task finishes */
	int* fTask;
	/* Allocs memory for vector fTask */
	fTask = (int *)malloc(n*sizeof(int)); // Aloca o espaço para o vetor fTask

	sprintf(buffer, "\n---------------------------\n");
	sprintf(buffer, "%s\nResults: \n=======\n\n", buffer);

	if (method == BCL) {
		//if (XPRBgetmipstat(problema) == XPRB_MIP_OPTIMAL) {
		if ( (XPRBgetlpstat(problema) == XPRB_LP_OPTIMAL) || (XPRBgetmipstat(problema) == XPRB_MIP_OPTIMAL) ) {
			// Prints the scheduling result for tasks on hosts
			sprintf(buffer, "%s \nTasks Schedule: \n--------------\n", buffer);
			for (i=0;i<n;i++) {
				for (t=1;t<=Tmax;t++) {
					for (k=0;k<m;k++) {
						if(XPRBgetsol(x[i][t][k]) > 0) {
							sprintf(buffer, "%sTask %d: ", buffer, i);
							sprintf(buffer, "%stakes %d s on host %d ", buffer, (int)ceil(I[i]*TI[k])*timeUnit, k);
							sprintf(buffer, "%susing VM %d ", buffer, S[i]-1);
							sprintf(buffer, "%sstarts at t = %d ", buffer, (t-(int)ceil(I[i]*TI[k]))*timeUnit );
							sprintf(buffer, "%sand finishes at t = %d ", buffer, t*timeUnit);
							if (XPRBgetlpstat(problema) == XPRB_LP_OPTIMAL)
								sprintf(buffer, "%sP = %f", buffer, XPRBgetsol(x[i][t][k]));
							sprintf(buffer, "%s\n", buffer);

							task_host[i] = k; //stores the executioner host for each task
							fTask[i] = t * timeUnit;
							/*
					        sprintf(buffer, "%s |s[0%d]=%d\n", buffer, i, t-(int)ceil(I[i]*TI[k]));
					        sprintf(buffer, "%s |x[0%d:0%d:0%d]= 1\n", buffer, i, k, S[i]-1);
					        sprintf(buffer, "%s |tempoProcessamento: %lf\n", buffer, ceil(I[i]*TI[k]));
					        sprintf(buffer, "%s |tempoInstanciar MV%d no host%d: %lf\n", buffer, S[i]-1, k, ceil(TR[k]*BV[S[i]-1]) );
					        sprintf(buffer, "%s f[0%d]= %d\n\n", buffer, i, t);
							 */
						}
					}
				}
			}
			sprintf(buffer, "%s ---------------------------\n\n", buffer);


			// Prints the time at which data transmition between tasks edns
			sprintf(buffer, "%s \nTask Data Transmitions: \n------------\n", buffer);
			for (i=0;i<n;i++)
				for (j=i;j<n;j++)
					if (D[i][j]==1) {
						sprintf(buffer, "%sfTx[%d][%d]= %d", buffer, i, j, fTask[i] + (int)ceil(B[i][j]*TB[task_host[i]][task_host[j]]) * timeUnit);
						if (task_host[i] == task_host[j])
							sprintf(buffer, "%s |SEM REDE\n", buffer);
						else
							sprintf(buffer, "%s |COM REDE\n", buffer);
					}
			sprintf(buffer, "%s ---------------------------\n\n", buffer);


			// Prints the scheduling result for virtual machines on hosts
			sprintf(buffer, "%s \nVMs Schedule: \n------------\n", buffer);
			float val; // stores the value of XPRBgetsol(y[v][t][k])
			for (k=0;k<m;k++) {
				for (v=0;v<o;v++) {
					for (t=2;t<=Tmax;t++) { //t starts in 2 because each timeslot is compared with the previous
						val = (float)XPRBgetsol(y[v][t][k]);
						if(val != (float)XPRBgetsol(y[v][t-1][k])) {
							if(val != 0)
								sprintf(buffer, "%sOn host %d , MV %d is   available at t= %d\n", buffer, k, v, t*timeUnit);
							else
								sprintf(buffer, "%sOn host %d , MV %d is UNavailable at t= %d\n", buffer, k, v, t*timeUnit);
						}
					}
				}
				//sprintf(buffer, "%s --------------------\n", buffer);
			}
			sprintf(buffer, "%s ---------------------------\n\n", buffer);
			/*
            // Prints the scheduling result for virtual machines on hosts
	        int val; // stores the value of XPRBgetsol(y[v][t][k])
	        sprintf(buffer, "%s \n\n", buffer);
	        for (k=0;k<m;k++) {
	            sprintf(buffer, "%s Host %d:\n", buffer, k);
		        for (v=0;v<o;v++) {
    		        sprintf(buffer, "%s \tMV %d:\n", buffer, v);
                	for (t=2;t<=Tmax;t++) { //t starts in 2 because each timeslot is compared with the previous
				        val = (int)XPRBgetsol(y[v][t][k]);
				        if(val != (int)XPRBgetsol(y[v][t-1][k])) {
					        if(val == 1)
						        sprintf(buffer, "%s \t\t  available at t=%d\n", buffer, t);
					        else
						        sprintf(buffer, "%s \t\tUNavailable at t=%d\n", buffer, t);
				        }
			        }
		        }
		        sprintf(buffer, "%s --------------------\n", buffer);
	        }
			 */
			sprintf(buffer, "%sF= %d\n", buffer, fTask[n-1]);
		}
		else
			if (XPRBgetmipstat(problema) == XPRB_MIP_SOLUTION)
				sprintf(buffer, "%s \n*** Unable to find an optimal solution for the problem ***\n", buffer);
			else if (XPRBgetmipstat(problema) == XPRB_MIP_INFEAS)
				sprintf(buffer, "%s \n*** It's impossible to resolve the problem ***\n", buffer);
	}
}


void freeMemory(void) {

	//   int i,j;

	XPRBdelprob(problema);
	freeVars();



	/*
	   for (i=0;i<n;i++) {
	   for (j=0;j<=Tmax;j++) {
	   free(*(*(x+i)+j));
	   }
	   free(*(x+i));
	   }
	   free(x);

	   for (i=0;i<o;i++) {
	   for (j=0;j<=Tmax;j++) {
	   free(*(*(y+i)+j));
	   }
	   free(*(y+i));
	   }
	   free(y);
	 */

}


