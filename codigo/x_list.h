/*
 *  Autor: Cesar G. Chaves A. <cesarchaves1@gmail.com>
 *  Data:  26/04/2010
 */

#ifndef X_LIST
#define X_LIST

#include<stdlib.h>
#include<stdio.h>

// Structure of list of elements
typedef struct list_el {
   int   f_time; // Time at which the execution of a task finishes
   int   host; // Host on which the execution finnished
   float prob; // Probability of the previous event happening
   struct list_el * next;
} tk_dimension;


tk_dimension * add2ListHead(tk_dimension * head, int t, int k, float p);
tk_dimension * add2ListAfter(tk_dimension * head, tk_dimension * curr, int t, int k, float p);
tk_dimension * add2ListOrderd(tk_dimension * head, int t, int k, float p, int ordervar);
tk_dimension * rmHead(tk_dimension * head);
tk_dimension * getRndItem(tk_dimension * head);
tk_dimension * freeList(tk_dimension * head);
void printList(tk_dimension * curr);

#endif
