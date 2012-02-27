/*
 *  Autor: Cesar G. Chaves A. <cesarchaves1@gmail.com>
 *  Data:  28/04/2010
 */

#include "x_relax.h"


  /***********************/
 /* Randomized Rounding */
/***********************/
void randomizedRnd(long drawings) {
	int i, j, t, k, foundK, infeasible;
	long drawing;
	double curMakepan, minMakespan = Tmax * timeUnit;
	float maxProb, r, tmp;
	char drwRep[10000]=""; // Stores if a solution for each drawing is found

	/* Vector that stores hosts eligibility probability */
	float *host_prob;
	host_prob = (float *)malloc(m*sizeof(float));

	/* Allocs memory for vector X */
	X = (int *)malloc(n*sizeof(int));
	/* Allocs memory for vector f */
	f = (double *)malloc(n*sizeof(double));
	/* Allocs memory for vector Y */
	Y = (int *)malloc(n*sizeof(int));

	/* Executes the LP*/
	XPRBminim(problema,"");
	//printResults();
    /* Random number generator seed */
	unsigned int seed=(unsigned int)time(NULL);
	srand (seed);
	printf("\n--------------------------------------------------\n");
	printf("\nRandom Seed: %d\n", seed);

	// Randomly selects hosts and calculate more than once and picks the shortest schedule
	for (drawing=1; drawing<=drawings; drawing++) {

		if (XPRBgetlpstat(problema) != XPRB_LP_INFEAS) {

			for (i=0; i<n; i++) {

				/* Initializes vector*/
				for (k=0; k<m; k++)
					host_prob[k]=0;
				maxProb = 0;
				X[i] = 0;

                /* Creates a list with cadidate hosts to run task i considering that they
                 * have to be linked to the hosts running tasks to which task i dependes 
                 */ 
				for (k=0;k<m;k++) {
					for (t=1;t<=Tmax;t++) {
						if (XPRBgetsol(x[i][t][k]) > 0) {
							if (i==0)
							    host_prob[k] += XPRBgetsol(x[i][t][k]);
							else {
								infeasible = 0;
								for (j=0; j<n; j++)
									if ( D[j][i] && !N[X[j]][k] ) {
										infeasible = 1;
										break;
									}
								if (!infeasible)
									host_prob[k] += XPRBgetsol(x[i][t][k]);
							}
						}
					}
					maxProb += host_prob[k];
				}

 				infeasible = 0;
				if (maxProb > 0) {
					/* Selects a random host from solution to run task i */
					r = (float)rand() / (float)RAND_MAX;
					tmp = 0;
					for (k=0;k<m;k++) {
						tmp += (host_prob[k]) / maxProb;
						if (tmp > r) {
							foundK=1;
							break;
						}
					}
					X[i] = k;
					//f[i] = t * timeUnit;
					Y[i] = S[i] - 1;
				}
				else {
					infeasible = 1;
					break;
				}

				/* Prints hosts probabilities */
				/*printf("\nTask %d:\n", i);
				for (k=0;k<m;k++) {
					if (host_prob[k]>0)
						printf ("--> h[%d] = %f\n", k, host_prob[k]);

				}*/
			}




			/*
			for (i=0; i<n; i++) {
				//Selects a random host from solution to run task i
				r = (float)rand() / (float)RAND_MAX;
				tmp = 0;
				foundK = 0;
				for (k=0;k<m;k++) {
					for (t=1;t<=Tmax;t++) {
						if(XPRBgetsol(x[i][t][k]) > 0) {
							tmp += XPRBgetsol(x[i][t][k]);
							if (tmp > r) {
								foundK=1;
								break;
							}
						}
					}
					if (foundK)
						break;
				}

				X[i] = k;
				f[i] = t * timeUnit;
				Y[i] = S[i] - 1;
			}
*/

			if (!infeasible) {
				/*  */
				if (!dbatista)
				    fixTimeValues();
				else
				    fixTimeValues1();

				infeasible = 0;
				for (i=0; i<n; i++)
					for (j=0; j<i; j++)
						if (D[j][i]==1)
							if (N[X[j]][X[i]]==0)
								infeasible=1;

				if (!infeasible) {
					/* Keep the shortest schedule */
					curMakepan = f1[n-1];
					if (curMakepan < minMakespan) {
						minMakespan = curMakepan;
						//printResults();
						printResultsBuffer();
					}
					//sprintf(drwRep,"%sObtained value on drawing %02d: %.4lf\n", drwRep, drawing, curMakepan);
				}
			}
			//if (infeasible)
				//sprintf(drwRep,"%sObtained value on drawing %02d: INFEASIBLE\n", drwRep, drawing);
		}
		//printf("drawing %02li\n", drawing);
		/*printf ("Chosen hosts on drawing %d: ", drawing);
		for (i=0; i<n; i++)
			printf (" %d", X[i]);
		printf("\n");*/
	}

	/* Prints additional relaxation algorithm info*/
	printf("\nRelaxation Algorithm: Randomized Rounding (RR)\n\n");
	printf("%s", drwRep);

	/* Prints the solution with the lowest makespan */
	printf("%s", buffer);
}


  /*********************************/
 /* Iterative Randomized Rounding */
/*********************************/
void randomizedRndIt(int drawings) {
	int i, t, k, drawing, foundK, curMakepan, minMakespan=Tmax; //, s, k1
	float r, tmp;
	char NOME[20];
	char drwRep[10000]=""; // Stores if a solution for each drawing is found
	tk_dimension * opc_lst = NULL; // Possible solution options
	tk_dimension * opc = NULL; // A possible solution option

	/* Random number generator seed */
	unsigned int seed=(unsigned int)time(NULL);
	srand (seed);
	printf("--> Seed: %d\n", seed);

	time(&end_t);
	progExec_t = difftime(end_t,start_t);
	printf("--> Running first execution of the LP! (t=%lf)\n", progExec_t);
	XPRBminim(problema,"");
	time(&end_t);
	progExec_t = difftime(end_t,start_t);
	printf("--> Finished running the LP! (t=%lf)\n", progExec_t);

	// Stores all the solution probabilities of task 0 on opc_lst
	for (k=0; k<m; k++) { // For every host
		for (t=ceil(I[0]*TI[k]); t<=Tmax; t++) { // For every time slot
			if ( XPRBgetsol(x[0][t][k]) > 0.0 )
				opc_lst = add2ListOrderd(opc_lst, t, k, XPRBgetsol(x[0][t][k]), 2);
		}
	}

	// Solves the problem more than once and picks the shortest schedule
	for (drawing=1; drawing<=drawings; drawing++) {

		// Selects a random option for the execution of task 0
		opc = getRndItem(opc_lst);
		//sprintf(drwRep,"%s DebugMsg: Task 0: Host: %d  Finishing Time: %d\n", drwRep, opc->host, opc->f_time );
		/***********************************************************************************************/
		/*Inserts a constraint fixing the execution of task 0 to a random host k*/
		sprintf(NOME,"Const-rx_0");
		ctr=XPRBnewctr(problema,NOME,XPRB_E);

		XPRBaddterm(ctr,x[0][opc->f_time][opc->host],1);
		XPRBaddterm(ctr,NULL,1);

		/*
		for (t=1;t<=Tmax;t++) { //Time
			for (k1=0;k1<m;k1++) { //Hosts
				if (k1 != opc->host )
					XPRBaddterm(ctr,x[0][t][k1],1);
			}
		}
		XPRBaddterm(ctr,NULL,0);
		 */

		/*Inserts a constraint fixing the VM that will execute 0 to the host k where the task was scheduled*/
		/*
		sprintf(NOME,"Const-rx_0_MV");
		ctr=XPRBnewctr(problema,NOME,XPRB_E);
		for (s=opc->f_time-ceil(I[0]*TI[k]);s<=opc->f_time;s++) { //Time
			XPRBaddterm(ctr,y[7][s][opc->host],1);
		}
		XPRBaddterm(ctr,NULL,ceil(I[0]*TI[k])+1);
		 */

		/***********************************************************************************************/

		XPRBminim(problema,"");

		for (i=1; i<n; i++) {

			if (XPRBgetlpstat(problema) != XPRB_LP_INFEAS) {
				/*Selects a random host from solution to run task i*/
				r = (float)rand() / (float)RAND_MAX;
				tmp = 0;
				foundK = 0;
				for (k=0;k<m;k++) {
					for (t=1;t<=Tmax;t++) {
						if(XPRBgetsol(x[i][t][k]) > 0) {
							tmp += XPRBgetsol(x[i][t][k]);
							if (tmp > r) {
								foundK=1;
								break;
							}
						}
					}
					if (foundK)
						break;
				}

				/*Inserts a constraint fixing the execution of task i to host k*/
				sprintf(NOME,"Const-rx_%d",i);
				ctr=XPRBnewctr(problema,NOME,XPRB_E);
				/********************************************/
				/*				for (t=1;t<=Tmax;t++) { //Time
					for (k1=0;k1<m;k1++) { //Hosts
						if (k != k1)
							XPRBaddterm(ctr,x[i][t][k1],1);
					}
				}
				XPRBaddterm(ctr,NULL,0);
				 */
				/********************************************/
				XPRBaddterm(ctr,x[i][t][k],1);
				XPRBaddterm(ctr,NULL,1);
				/********************************************/


				/*Inserts a constraint fixing the VM that will execute 0 to the host k where the task was scheduled*/
				/*
				sprintf(NOME,"Const-rx_%d_MV",i);
				ctr=XPRBnewctr(problema,NOME,XPRB_E);
				for (s=t-ceil(I[i]*TI[k]);s<=t;s++) { //Time
					XPRBaddterm(ctr,y[S[i]-1][s][k],1);
				}
				XPRBaddterm(ctr,NULL,ceil(I[i]*TI[k])+1);
				 */


				/*Solves the LP problem with the new constraint*/
				XPRBminim(problema,"");
			}
			else
				break;
		}

		/* Keep the shortest schedule */
		if (XPRBgetlpstat(problema) != XPRB_LP_INFEAS) {
			curMakepan = (int)XPRBgetobjval(problema);
			if (curMakepan < minMakespan) {
				minMakespan = curMakepan;
				printResultsBufferOLD();
			}
			sprintf(drwRep,"%s DebugMsg: drawing %02d: OK %d\n", drwRep, drawing, curMakepan* timeUnit);
		}
		else {
			sprintf(drwRep,"%s DebugMsg: drawing %02d: INFEASIBLE\n", drwRep, drawing);
		}

		/* Removes last constraints */
		if (drawing < drawings) {
			for (;i>=0;i--) {
				sprintf(NOME,"Const-rx_%d",i);
				ctr = XPRBgetbyname(problema,NOME,XPRB_CTR);
				if (ctr != NULL)
					XPRBdelctr(ctr);
				sprintf(NOME,"Const-rx_%d_MV",i);
				ctr = XPRBgetbyname(problema,NOME,XPRB_CTR);
				if (ctr != NULL)
					XPRBdelctr(ctr);

			}
		}
	}
	opc_lst = freeList(opc_lst);

	/* Prints the solution with the lowest makespan */
	printf("%s", buffer);
	printf("%s", drwRep);
}


  /******************************************/
 /* Iterative randomized rounding enhanced */
/******************************************/
void randomizedRndIt_En() {
	int i, t, k, k1, foundK;
	float r, tmp;
	char NOME[20];

	/* Random number generator seed */
	unsigned int seed=(unsigned int)time(NULL);
	srand (seed);
	printf("--> Seed: %d\n", seed);

	/* Executes the IP */
	XPRBminim(problema,"");

	for (i=0; i<n; i++) {
		do {
			if (XPRBgetlpstat(problema) == XPRB_LP_INFEAS)
				XPRBminim(problema,"");

			/*Selects a random host from solution to run task i*/
			r = (float)rand() / (float)RAND_MAX;
			tmp = 0;
			foundK = 0;
			for (k=0;k<m && !foundK;k++) {
				for (t=1;t<=Tmax && !foundK;t++) {
					if(XPRBgetsol(x[i][t][k]) > 0) {
						tmp += XPRBgetsol(x[i][t][k]);
						if (tmp > r)
							foundK=1;
					}
				}
			}
			k--;

			/*Inserts a constraint fixing the execution of task i to host k*/
			sprintf(NOME,"Const-rx_%d",i);
			ctr=XPRBnewctr(problema,NOME,XPRB_E);
			for (t=1;t<=Tmax;t++) { //Time
				for (k1=0;k1<m;k1++) { //Hosts
					if (k != k1)
						XPRBaddterm(ctr,x[i][t][k1],1);
				}
			}
			XPRBaddterm(ctr,NULL,0);

			/*Solves the LP problem with the new constraint*/
			XPRBminim(problema,"");

			/*If the problem became infeaseble, rejects host k*/
			if (XPRBgetlpstat(problema) == XPRB_LP_INFEAS) {

				/* Removes last constraint */
				XPRBdelctr(ctr);

				/* Inserts a constraint to avoid the execution of task i to host k */
				sprintf(NOME,"Const-rx_%d_k%d_0",i,k);
				ctr=XPRBnewctr(problema,NOME,XPRB_E);
				for (t=1;t<=Tmax;t++) //Time
					XPRBaddterm(ctr,x[i][t][k],1);
				XPRBaddterm(ctr,NULL,0);
			}
		} while ( (XPRBgetlpstat(problema) == XPRB_LP_INFEAS) && foundK );
	}
	printResults();

}


  /********************************/
 /* Maximum Probability Rounding */
/********************************/
/*void maxProbRnd() {
	//Executes relaxation and rounds by the greatest probability

	XPRBminim(problema,"");
	int i, t, k, T, K;
	char NOME[20];

	for (i=0; i<n; i++) { // For every task
		T=1, K=0;

		if ( XPRBgetlpstat(problema) != XPRB_LP_INFEAS ) {

			// Finds the greatest probability x[i][T][K]
			for (k=0; k<m; k++) { // For every host
				for (t=ceil(I[i]*TI[k]); t<Tmax; t++) { // For every time slot
					if ( (float)XPRBgetsol(x[i][t][k]) > (float)XPRBgetsol(x[i][T][K]) ) {
						//printf("\nx[%d][%d][%d] > x[%d][%d][%d] ==> %f > %f\n", i, t*timeUnit, k, i, T*timeUnit, K, (float)XPRBgetsol(x[i][t][k]), (float)XPRBgetsol(x[i][T][K]));
						T = t;
						K = k;
					}
				}
			}

			//Inserts a constraint fixing x[i][T][K] = 1
			sprintf(NOME,"Const-rx_%d",i);
			ctr=XPRBnewctr(problema,NOME,XPRB_E);
			XPRBaddterm(ctr,x[i][T][K],1);
			XPRBaddterm(ctr,NULL,1);

			// Executes the Integer Program
			XPRBminim(problema,"");
		}
		else
			break;
	}
	 Printing obtained results
	printf("\n***************************************************************************************************************\n");
	printResults();
	printf("\n***************************************************************************************************************\n");

}*/
void maxProbRnd() {

	int i, j, t, k, T, K, infeasible;

	/* Allocs memory for vector X */
	X = (int *)malloc(n*sizeof(int));
	/* Allocs memory for vector f */
	f = (double *)malloc(n*sizeof(double));
	/* Allocs memory for vector Y */
	Y = (int *)malloc(n*sizeof(int));

	/* Executes relaxation and rounds by the greatest probability */
	XPRBminim(problema,"");

	if ( XPRBgetlpstat(problema) != XPRB_LP_INFEAS ) {
		for (i=0; i<n; i++) { // For every task
			T=1, K=0;

			// Finds the greatest probability x[i][T][K]
			for (k=0; k<m; k++) { // For every host
				for (t=ceil(I[i]*TI[k]); t<Tmax; t++) { // For every time slot
					if ( XPRBgetsol(x[i][t][k]) > XPRBgetsol(x[i][T][K]) ) {
						T = t;
						K = k;
					}
				}
			}

			X[i] = K;
			f[i] = T * timeUnit;
			Y[i] = S[i] - 1;
		}

		fixTimeValues();

		infeasible = 0;
		for (i=0; i<n; i++)
			for (j=0; j<i; j++)
				if (D[j][i]==1)
					if (N[X[j]][X[i]]==0)
						infeasible=1;

		if (!infeasible) {
			printResultsBuffer();

			/* Prints the solution with the lowest makespan */
			printf("%s", buffer);
		}
		else
			printf("INFEASIBLE");
	}
}


  /*****************************************/
 /* Maximum Probability Rounding Enhanced */
/*****************************************/
void maxProbRnd_En() {
	//Executes relaxation and rounds by the greatest probability - Enhanced

	int i, j, t, k, T, K, infeasible;
	tk_dimension * opc_lst = NULL; // Possible solution options

	/* Allocs memory for vector X */
	X = (int *)malloc(n*sizeof(int));
	/* Allocs memory for vector f */
	f = (double *)malloc(n*sizeof(double));
	/* Allocs memory for vector Y */
	Y = (int *)malloc(n*sizeof(int));

	/* Executes the IP */
	XPRBminim(problema,"");

	for (i=0; i<n; i++) { // For every task

		// Finds the greatest probability x[i][T][K]
		for (k=0; k<m; k++) { // For every host
			for (t=ceil(I[i]*TI[k]); t<=Tmax; t++) { // For every time slot
				if ( XPRBgetsol(x[i][t][k]) > 0 ) {
					opc_lst = add2ListOrderd(opc_lst, t, k, (float)XPRBgetsol(x[i][t][k]), 3);
				}
			}
		}

		do {

			T = opc_lst->f_time;
			K = opc_lst->host;

			X[i] = K;
			f[i] = T * timeUnit;
			Y[i] = S[i] - 1;

			infeasible = 0;
			for (j=0; j<i; j++)
				if (D[j][i]==1)
					if (N[X[j]][X[i]]==0)
						infeasible=1;

			/* If the problem became infeaseble */
			if (infeasible) {
				opc_lst = rmHead(opc_lst);
			}
		} while( opc_lst!=NULL && infeasible);
		opc_lst = freeList(opc_lst);
		if (infeasible)
			break;
	}
	if (!infeasible) {
		fixTimeValues();
		printResultsBuffer();
		/* Prints the solution with the lowest makespan */
		printf("%s", buffer);
	}
	else
		printf("INFEASIBLE");
}

/*void maxProbRnd_En() {
	//Executes relaxation and rounds by the greatest probability - Enhanced

	int i, t, k, T, K;
	char NOME[20];
	tk_dimension * opc_lst = NULL; // Possible solution options

	 Executes the IP
	XPRBminim(problema,"");

	for (i=0; i<n; i++) { // For every task

		// Finds the greatest probability x[i][T][K]
		for (k=0; k<m; k++) { // For every host
			for (t=ceil(I[i]*TI[k]); t<=Tmax; t++) { // For every time slot
				if ( XPRBgetsol(x[i][t][k]) > 0.0 )
					opc_lst = add2ListOrderd(opc_lst, t, k, (float)XPRBgetsol(x[i][t][k]), 3);
			}
		}

		//printList(opc_lst);
		//printf("\n\n");

		do {

			T = opc_lst->f_time;
			K = opc_lst->host;

			//printf("\nFixing x[%d][%d][%d] = 1\n", i, T*timeUnit, K);

			//Inserts a constraint fixing x[i][T][K] = 1
			sprintf(NOME,"Const-rx_%d",i);
			ctr=XPRBnewctr(problema,NOME,XPRB_E);
			XPRBaddterm(ctr,x[i][T][K],1);
			XPRBaddterm(ctr,NULL,1);

			 Executes the Integer Program with the new constraint
			XPRBminim(problema,"");

			 If the problem became infeaseble
			if (XPRBgetlpstat(problema) == XPRB_LP_INFEAS) {

				//printf("\nUnfixing x[%d][%d][%d] = 1\n", i, T*timeUnit, K);

				 Removes last constraint
				XPRBdelctr(ctr);

				 Inserts an oposite constraint
				sprintf(NOME,"Const-rx_%d_k%d_0",i,k);
				ctr=XPRBnewctr(problema,NOME,XPRB_E);
				for (t=1;t<=Tmax;t++) //Time
					XPRBaddterm(ctr,x[i][t][k],1);
				XPRBaddterm(ctr,NULL,0);
				opc_lst = rmHead(opc_lst);
			}
		} while( opc_lst!=NULL && XPRBgetlpstat(problema) == XPRB_LP_INFEAS);
		opc_lst = freeList(opc_lst);
		//opc_lst = NULL;
	}

	printf("\n***************************************************************************************************************\n");
	// Printing obtained results
	printResults();
	printf("\n***************************************************************************************************************\n");
}*/

  /***********************************/
 /* Shortest Time Rounding Enhanced */
/***********************************/
void shortTimeRnd_En() {
	//Executes relaxation and rounds by the greatest probability - Enhanced

	int i, t, k, T, K;
	char NOME[20];
	tk_dimension * opc_lst = NULL; // Possible solution options

	/* Executes the IP */
	XPRBminim(problema,"");

	for (i=0; i<n; i++) { // For every task

		// Finds the greatest probability x[i][T][K]
		for (k=0; k<m; k++) { // For every host
			for (t=ceil(I[i]*TI[k]); t<=Tmax; t++) { // For every time slot
				if ( (float)XPRBgetsol(x[i][t][k]) > 0 )
					opc_lst = add2ListOrderd(opc_lst, t, k, (float)XPRBgetsol(x[i][t][k]), 1);
			}
		}

		printList(opc_lst);
		printf("\n\n");

		do {

			T = opc_lst->f_time;
			K = opc_lst->host;

			printf("\n--> Fixing x[%d][%d][%d] = 1\n", i, T*timeUnit, K);

			//Inserts a constraint fixing x[i][T][K] = 1
			sprintf(NOME,"Const-rx_%d",i);
			ctr=XPRBnewctr(problema,NOME,XPRB_E);
			XPRBaddterm(ctr,x[i][T][K],1);
			XPRBaddterm(ctr,NULL,1);

			/* Executes the Integer Program with the new constraint */
			XPRBminim(problema,"");

			/* If the problem became infeaseble */
			if (XPRBgetlpstat(problema) == XPRB_LP_INFEAS) {

				printf("\n--> Unfixing x[%d][%d][%d] = 1\n", i, T*timeUnit, K);

				/* Removes last constraint */
				XPRBdelctr(ctr);

				/* Inserts an oposite constraint */
				sprintf(NOME,"Const-rx_%d_k%d_0",i,k);
				ctr=XPRBnewctr(problema,NOME,XPRB_E);
				for (t=1;t<=Tmax;t++) //Time
					XPRBaddterm(ctr,x[i][t][k],1);
				XPRBaddterm(ctr,NULL,0);
				opc_lst = rmHead(opc_lst);

			}
		} while( opc_lst!=NULL && XPRBgetlpstat(problema) == XPRB_LP_INFEAS);
		//opc_lst = freeList(opc_lst);
		opc_lst = NULL;
	}

	printf("\n***************************************************************************************************************\n");
	// Printing obtained results
	printResults();
	printf("\n***************************************************************************************************************\n");
}


  /*******************/
 /* Time Tightening */
/*******************/
void timeTightening() {

}


/*
    int ** rxSol; // Array storing relaxed solutions per task
	rxSol = (int **)malloc(n*sizeof(int *));
	for (i=0;i<n;i++)
 *(rxSol+i)=(int *)malloc(1000*sizeof(int));

    int * rxSols; // Vector storing amount of relaxed solutions per task
	rxSols = (int *)malloc(n*sizeof(int));// Alocating memory for vector probs

    // Counts the amount of relaxed solutions per task
    for (i=0;i<n;i++) {
        rxSols[i] = 0;
        for (t=1;t<=Tmax;t++)
	        for (k=0;k<m;k++)
		        if(XPRBgetsol(x[i]                if (tmp>r)
	                break;
[t][k]) > 0) {
		            foundK = 0;
		            for(j=0; j<rxSols[i] && !foundK; j++)
		                if (k == rxSol[i][j])
		                    foundK = 1;
		            if (!foundK)        
		                rxSol[i][rxSols[i]++] = k;
		        }
    }
 */



// r ../cenarios/simples/hosts/c_050.dat 0 "" 19 > result

