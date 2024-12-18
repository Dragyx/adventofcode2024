#include "common.h"
#include <stdlib.h>



void enqueue(queue* Q, void* v) {
  q_node* new = malloc(sizeof(q_node));
  *new = (q_node){.next=NULL, .value=v};
  if (Q->size == 0)  {
    Q->start = new; 
    Q->end = new;
  } else {
    Q->end->next = new;
    Q->end = new;
  }

  Q->size++;
}
void* dequeue(queue* Q) {
  if (Q->size == 0) 
    return NULL;

  void* v = Q->start->value;
  q_node* next = Q->start->next;
  free(Q->start);
  Q->start = next;


  // just for good measure 
  // (so the state of end is consistent with the rest)
  if (Q->size == 1)
    Q->end = NULL;

  Q->size--;

  return v;
}
void free_nodes(queue* Q) {
  q_node* s = Q->start; 
  while (s != NULL) {
    q_node* to_free = s;
    s = s->next;
    free(to_free);
  }
}
