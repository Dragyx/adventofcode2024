#include <assert.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include  "helpers.h"

#define T int
#include "vec.def"
#undef T

#define T vec_int
#include "vec.def"
#undef T

void modified_selection_sort(vec_int* list, vec_int* adj, int n) {
  // this is basically selection sort (smallest to greates), but it finds 
  // a minimal element in regards to the (antisymmetric)
  // "before" relation (i.e. i < j <==> i must appear before j)l
  // prescribed by the rules instead of the minimum numerical 
  // value in each outer iteration
  size_t m = list->size;
  for (int i = 0; i < m; i++) {
    // find an element that is allowed to be the first of all other
    // elements in the non-sorted part [i, m) and swap it to position i
    for (int j = i; j < m; j++) {
      bool minimal = true;
      for (int k = i; k < m; k++) {
        int u = list->start[j];
        int v = list->start[k];
        if (adj->start[v * n + u]) {
          minimal = false;
          break;
        }
      }
      if (minimal) {
        int tmp = list->start[i];
        list->start[i] = list->start[j];
        list->start[j] = tmp;
        break;
      }
    }
  }
}


int day5() {
  FILE* input_f = load_input(5);  
  assert(input_f);
  vec_int rules = MK_VEC(int);
  vec_vec_int updates = MK_VEC(vec_int);

  
  int before, after, rc;
   //skip the newline


  // load the rules
  int max_page = 0;
  while ((rc = fscanf(input_f, "%u|%u", &before, &after))) {
    if (rc == EOF) break;    
    max_page = max(max_page, before);
    max_page = max(max_page, after);
    vec_int_push(&rules, before);
    vec_int_push(&rules, after);
  }

  // load the updates
  int page;
  vec_int _init_update = MK_VEC(int);
  vec_vec_int_push(&updates, _init_update);
  vec_int* current_update = updates.start;

  vec_int_push(current_update, before);
  getc(input_f);

  char sep[2];
  for (;;) {
    // scan and check for newlines with %1[^\n]
    rc = fscanf(input_f, "%u%1[^\n]", &page, sep);
    if (rc == EOF) break;
    max_page = max(max_page, page);
    vec_int_push(current_update, page);

    if (rc == 1) {
      // there is a newline instead of a ","
      vec_int new_page = MK_VEC(int);
      vec_vec_int_push(&updates, new_page);
      current_update = &updates.start[updates.size - 1];
    }    
  }

  // create an n by n adjacency matrix where 
  // (i must be before j) <===> adj[i * n + j] == 1
  int n = max_page + 1;

  vec_int adj = MK_VEC_ZERO(int, n * n);
  for (int i = 0; i < rules.size; i += 2) {
    int u = rules.start[i];
    int v = rules.start[i+1];
    adj.start[u * n + v] = 1;
  }

  

  // find middle elements and sum them up
  vec_int cur_update;
  unsigned long sum = 0, sumt2 = 0;

  while ((rc = vec_vec_int_pop(&updates, &cur_update)) == 0) {
    assert(rc != 2);
    if (cur_update.size == 0) {
      free(cur_update.start);
      continue;
    }
    // check if the relation holds
    bool rules_hold = true;
  
    for (unsigned int i = 0; i < cur_update.size; ++i) {
      for (unsigned int j = i + 1; j < cur_update.size; ++j) {
        int u = cur_update.start[i];
        int v = cur_update.start[j];
        rules_hold = !adj.start[v * n + u];
        if (!rules_hold) break;
      }
      if (!rules_hold) break;
    }

    if (rules_hold)
      sum += cur_update.start[cur_update.size / 2];

    if (!rules_hold) {
      // sort for task 2
      modified_selection_sort(&cur_update, &adj, n);
      sumt2 += cur_update.start[cur_update.size / 2];
    };
    free(cur_update.start);
  }


  printf("--> Q1: The sum of all middle pages is: %lu\n", sum);
  printf("--> Q2: The sum of all invalid, sorted middle pages is: %lu\n", sumt2);
  

  free(rules.start);
  free(updates.start);
  free(adj.start);
  
  return 0;
}
