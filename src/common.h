#ifndef COMMON_H_
#define COMMON_H_

#include <stddef.h>
typedef struct {
  int i, j;
} pos;



struct q_node {
  struct q_node* next;
  void* value;
};
typedef struct q_node q_node;

typedef struct {
  q_node* start;
  q_node* end;
  size_t size;
} queue;

void enqueue(queue* Q, void* v);
void* dequeue(queue* Q);
void free_nodes(queue* Q);

#endif
