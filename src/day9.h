#ifndef DAY9_H_
#define DAY9_H_


#include <stdio.h>
#include <stdlib.h>
int day9();

typedef struct node node;
struct node {
  node* next; 
  size_t size; // the size of the free block on disk
  size_t pos;  // the position on the disk
};

void free_node(node* n); 


#endif
