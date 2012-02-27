#include "heft.h"
#include "x_io.h"

char progName[10] = "";
void usage();

int main(int argc, char **argv) {
    int i, k;

    /********************************************************************************/
	strcpy(progName, argv[0]);

	/* Verifying the arguments input by the user */
	if (argc < 2 || argc > 4) {
		usage();
		return(1);
	}

	/* Reading the input file or files */
	if (argc == 2) {
		if(readInputFile(argv[1])) {
			return(2);
		}
		else {
			time(&end_t);
			progExec_t = difftime(end_t,start_t);
			printf("\n--> Finished reading input data! (t=%lf)\n", progExec_t);
			//calcTmax();
			//printf("\nTmax: %.4lf\n\n", TmaxReal);
		}
	}
	else if (argc == 4) {
		if(readInputFiles(argv[1], argv[2], argv[3]) ) {
			return(2);
		}
		else {
			time(&end_t);
			progExec_t = difftime(end_t,start_t);
			printf("\n--> Finished reading input data files! (t=%lf)\n", progExec_t);
			//calcTmax();
			//printf("\nTmax: %f\n\n", TmaxReal);
		}
	}

    /********************************************************************************/

    allocVars();
    calcMeans();

    /* Initializes vectors rankU and rankd */
    for (i=0; i<n; i++) {
       	rankU[i] = 0;
       	rankD[i] = 0;
    }

    /* Calculate upward rank of tasks */
    if (calcRankU(n-1) == 0)
        return(3); /* Unable to calculate ranks */

    /* Initializes vector rankUindex */
    for (i=0; i<n; i++)
    	rankUindex[i] = i;
    /* Creates an ordered index of vector rankU */
    mergeSort(rankUindex, rankU, 0, n-1);

    /* Initializes the host availability vector */
    for (k=0; k<m; k++)
    	avail[k] = 0;

    double minEFT, temp;
    int minEFTk; /* Host providing the shortest EFT */
    for (i=0; i<n; i++) {
    	for (k=0; k<m; k++) {
    		temp = EFT(rankUindex[i], k);
    		if (minEFT > temp) {
    			minEFT = temp;
    			minEFTk = k;
    		}
    	}
    	AFT[rankUindex[i]] = minEFTk;
    	hostxTask[rankUindex[i]] = minEFTk;
    	avail[minEFTk] = minEFTk;
    }
	return 0;
}

int allocVars(void) {

	/* Allocating memory space for vector 'rankU' */
	rankU = (double *)malloc(n*sizeof(double));
	if (!rankU) {
		fprintf(stderr,"Error allocating memory [rankU] :'(.\n");
		return(2);
	}

	/* Allocating memory space for vector 'rankUindex' */
	rankUindex = (int *)malloc(n*sizeof(int));
	if (!rankUindex) {
		fprintf(stderr,"Error allocating memory [rankUindex] :'(.\n");
		return(2);
	}


	/* Allocating memory space for vector 'rankD' */
	rankD = (double *)malloc(n*sizeof(double));
	if (!rankD) {
		fprintf(stderr,"Error allocating memory [rankD] :'(.\n");
		return(2);
	}

	/* Allocating memory space for vector 'AST' */
	AST = (double *)malloc(n*sizeof(double));
	if (!AST) {
		fprintf(stderr,"Error allocating memory [AST] :'(.\n");
		return(2);
	}

	/* Allocating memory space for vector 'AFT' */
	AFT = (double *)malloc(n*sizeof(double));
	if (!AFT) {
		fprintf(stderr,"Error allocating memory [AFT] :'(.\n");
		return(2);
	}

	/* Allocating memory space for vector 'avail' */
	avail = (double *)malloc(m*sizeof(double));
	if (!avail) {
		fprintf(stderr,"Error allocating memory [avail] :'(.\n");
		return(2);
	}
	return 0;
}

void calcMeans(void) {
    int k, l;
    int aLinks; /* Amount of links */

    meanTI = 0;
    meanTB = 0;
    aLinks = 0;

    /* Calculates the mean TI */
    for (k=0; k<m; k++)
    	meanTI += TI[k];
    meanTI /= m;

    /* Calculates the mean TB */
    for (k=0; k<m-1; k++)
    	for (l=k+1; l<m; l++)
    		if (TB[k][l] > 0) {
    			meanTB += TB[k][l];
    			aLinks++;
    		}
    meanTB /= aLinks;
}

double calcRankU(int task) {
	int j;
	double tmp, max;
    if (task == n-1) {
    	rankU[task] = I[task] * meanTI;
    	return(rankU[task]);
    }
    else {
    	max = 0;
    	/* For successors of task */
    	for (j=task; j<n; j++) {
    		if (D[task][j]) {
				if (rankU[j] == 0)
					rankU[j] = calcRankU(j);
				tmp = rankU[j] + B[task][j]*meanTB;
				if (max < tmp)
					max = tmp;
    		}
    	}
    	rankU[task] = I[j] * meanTI + max;
    	return(rankU[task]);
    }
}

double calcRankD(int task) {
	int j;
	double tmp, max;
    if (task == 0) {
    	rankD[0] = 0;
    	return(0);
    }
    else {
    	max = 0;
    	/* For predecessors of task */
    	for (j=task; j>=0; j--) {
    		if (D[j][task]) {
				if (rankD[j] == 0)
					rankD[j] = calcRankD(j);
				tmp = rankD[j] + I[j]*meanTI + B[j][task]*meanTB;
				if (max < tmp)
					max = tmp;
    		}
    	}
    	rankD[task] = max;
    	return(rankD[task]);
    }
}

double EFT(int task, int host) {
	return ( I[task]*TI[host] + EST(task, host) );
}

double EST(int task, int host) {
	int j;
	double tmp, max;
	if (task==0)
		return 0;
	else {
		max = 0;
		/* For predecessors of task */
		for (j=task; j>=0; j--) {
			if (D[j][task]) {
				tmp = AFT[j] + B[j][task]*TB[hostxTask[j]][host];
				if (max < tmp)
					max = tmp;
			}
		}
		if (avail[host] > max)
			return (avail[host]);
		else
			return (max);
	}
}

// creates an index of a vector using the MergeSort algorithm
void mergeSort(int * index, double * vector, int beg, int end) {
	if(beg >= end) return;
	int med = (beg + end) / 2;
	mergeSort(index, vector, beg, med);
	mergeSort(index, vector, med + 1, end);
	merge(index, vector, beg, med, end);
}

void merge(int * index, double * vector, int beg, int med, int end) {
	int k = 0;
	int * aux;

	/* Allocating memory space for vector 'aux' */
	aux = (int *)malloc(n*sizeof(int));
	if (!aux) {
		fprintf(stderr,"Error allocating memory [aux] :'(.\n");
	}

	int i = beg;
	int f = med + 1;

	while((i <= med) && (f <= end)) {
		if(vector[index[i]] < vector[index[f]])
			aux[k++] = index[i++];
		else
			aux[k++] = index[f++];
	}

	while(i <= med)
		aux[k++]=index[i++];
	while(f <= end)
		aux[k++]=index[f++];
	for(k = beg, i = 0; k <= end; k++, i++)
		index[k] = aux[i];
	free(aux);
}

void usage() {
	fprintf(stderr,"\nUse : %s SCENERY\n",progName);
	fprintf(stderr,"      %s APP_FILE GRID_FILE VMR_FILE\n",progName);

//	fprintf(stderr,"\nExamples: %s 0 int 20 scenery01\n",progName);
//	fprintf(stderr,"          %s 0 rr 20 scenery01\n",progName);
//	fprintf(stderr,"          %s 0 int 20 app01 grid01 vmr01\n",progName);
}
