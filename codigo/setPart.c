/*
 * Description: a recursive algorithm for generating set partitions
 *              of a group with n elements.
 * 
 * Algorithm found on paper: M. C. Er, "A Fast Algorithm for 
 *                           Generating Set Partitions", Comput. J.,
 *                           vol. 31, no. 3, pp. 283-284, 1988
 *
 * Paper links: http://comjnl.oxfordjournals.org/cgi/reprint/31/3/283
 *              http://portal.acm.org/citation.cfm?id=48673
 *
 * Implemented: Cesar G. Chaves A. (cesarchaves1@hotmail.com)
 * Implementation Date: August 24th 2010 
 */
 

# include <stdio.h>
# include <stdlib.h>

int n; /* Amount of tasks requiring the same VM*/
int * c; /* Codeword */

void SP(int m, int p); /* Calculates the grups of tasks */
void printPartition(); /* Prints the groups of tasks */


int main (int argc, char **argv) {
    
    if (argc != 2) {
        printf("Usage: %s <amount_of_group_elements>\n", argv[0]);
        return 1;
    }
    
    n=atoi(argv[1]); 
    c = (int *)malloc(n*sizeof(int));
    SP(0,1);
    return 0;
}

void SP(int m, int p) {
    int i;
    if (p < n) {
        for (i=1; i<=m; i++) {
            c[p]=i;
            SP(m, p+1);
        }
        c[p]=m+1;
        SP(m+1,p+1);
    }
    else if (p==n)
        for (i=1;i<=m+1;i++) {
            c[p]=i;
            printPartition();
        }
}

void printPartition() {
    int i;
    for (i=1;i<=n;i++)
        printf(" %d ",c[i]);
    printf("\n");
}
