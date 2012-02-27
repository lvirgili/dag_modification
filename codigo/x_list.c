/*
 *  Autor: Cesar G. Chaves A. <cesarchaves1@gmail.com>
 *  Data:  26/04/2010
 */

#include "x_list.h"
//#include <stdlib.h>

/*      #include <stdlib.h>
 *
int main() {
    tk_dimension * dmHead = NULL;
    int orderby=1;
        
    int i;
    for(i=1;i<=10;i++) {
        dmHead = add2ListOrderd(dmHead, i, (i-1)%5+1, i, orderby);
    }

    dmHead = add2ListOrderd(dmHead, 1, 111.1111, 0.5, orderby);    
    dmHead = add2ListOrderd(dmHead, 1, 111.1111, 6, orderby);
  
    dmHead = add2ListOrderd(dmHead, 11, 111.1111, 10, orderby);
    dmHead = add2ListOrderd(dmHead, 9, 111.1111, 10, orderby);    

    printList(dmHead);
    printf("\n\n");

    dmHead = freeList(dmHead);

    printList(dmHead);

    dmHead = add2ListOrderd(dmHead, 1, 111.1111, 0.5, orderby);    
    dmHead = add2ListOrderd(dmHead, 1, 111.1111, 6, orderby);
  
    dmHead = add2ListOrderd(dmHead, 11, 111.1111, 10, orderby);
    dmHead = add2ListOrderd(dmHead, 9, 111.1111, 10, orderby);

    printList(dmHead);

}
*/

// Adds a new element to the beginning of the list
tk_dimension * add2ListHead(tk_dimension * head, int t, int k, float p) {
    
    tk_dimension * new;
    
    new = (tk_dimension *)malloc(sizeof(tk_dimension));
    new->f_time = t;
    new->host = k;
    new->prob = p;
    
    new->next  = head;
    head = new;
    
    return head;
}

// Adds a new element after an specific item of the list
tk_dimension * add2ListAfter(tk_dimension * head, tk_dimension * curr, int t, int k, float p) {
    
    tk_dimension * new;
    
    new = (tk_dimension *)malloc(sizeof(tk_dimension));
    new->f_time = t;
    new->host = k;
    new->prob = p;
    
    new->next  = curr->next;
    curr->next = new;
    
    return head;
}

// Adds a new element maintainning the order of the list
tk_dimension * add2ListOrderd(tk_dimension * head, int t, int k, float p, int ordervar) {

    tk_dimension * curr = head;

    if (!head) { // If the list is empty
        head = (tk_dimension *)malloc(sizeof(tk_dimension));
        head->f_time = t;
        head->host = k;
        head->prob = p;
        head->next=NULL;
    }
    else if (ordervar == 1) { // Ordered by time (crescent)
        if ( (t < head->f_time) || (t == head->f_time && p > head->prob) ) // If the item should be before the head
            head = add2ListHead(head, t, k, p);
        else {
            while( curr->next && t > curr->next->f_time)                
                curr = curr->next;
            while ( curr->next && (t == curr->next->f_time) && (p < curr->next->prob) )
                curr = curr->next;            
            head = add2ListAfter(head, curr, t, k, p);
        }
    }
    else if (ordervar == 2) { // Ordered by host (crescent)
    	if ( (k < head->host) || (k == head->host && t < head->f_time) ) // If the item should be before the head
    		head = add2ListHead(head, t, k, p);
    	else {
    		while( curr->next && k > curr->next->host)
    			curr = curr->next;
    		while ( curr->next && (k == curr->next->host) && (t > curr->next->f_time) )
    			curr = curr->next;
    		head = add2ListAfter(head, curr, t, k, p);
    	}
    }
    else if (ordervar == 3) { // Ordered by probability (decrescent)
        if ( (p > head->prob) || (p == head->prob && t < head->f_time) ) // If the item should be before the head
            head = add2ListHead(head, t, k, p);
        else {
            while( curr->next && p < curr->next->prob)
                curr = curr->next;
            while ( curr->next && (p == curr->next->prob) && (t > curr->next->f_time) )
                curr = curr->next;            
            head = add2ListAfter(head, curr, t, k, p);
        }
    }

    return head;
}

// Removes first item from the list 
tk_dimension * rmHead(tk_dimension * head) {
    if (head) {
        tk_dimension * temp = head;
        head = head->next;
        free( temp );
    }
    return head;
}

// Returns a random item considering it's probability
tk_dimension * getRndItem(tk_dimension * head) {

	tk_dimension * curr = head;
	float r, tmp;

	if (curr != NULL) {
		r = (float)rand()/(float)RAND_MAX;
		//printf("  DebugMsg: Random value: %f\n", r);

		tmp = curr->prob;

		while ( r > tmp && curr->next != NULL) {

			curr = curr->next;
			tmp += curr->prob;
		}
	}
	return curr;
}


tk_dimension * freeList(tk_dimension * head) {
    tk_dimension * temp;
    
    while (head) {
        temp = head->next;
        free(head);
        head = temp;
    }
    return head;
}

/*
tk_dimension * freeList(tk_dimension * head) {
    if (head->next == NULL)
        return head;
    else {
        head = freeList(head->next);
        free (tmp);
        return head;
    }
}
*/

// Prints the content of the list
void printList(tk_dimension * curr) {
    while(curr) {
    	printf("DebugMsg: %8d -- %8d -- %8.4f \n", curr->f_time, curr->host, curr->prob);
        curr = curr->next ;
    }
}

// r ../cenarios/simples/hosts/c_050.dat 0 "" 19 > result

