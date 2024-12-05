#include <assert.h>
#include <stdbool.h>
#include <stdio.h>
#include  "helpers.h"

#define T int
#include "vec.def"
#undef T

#define T vec_int
#include "vec.def"
#undef T


int day5() {
  FILE* input_f = load_input(5);  
  assert(input_f);
  vec_int rules = MK_VEC(int);
  vec_vec_int updates = MK_VEC(vec_int);

  
  int before, after, rc;
   //skip the newline

  int max_page = 0;
  while ((rc = fscanf(input_f, "%u|%u", &before, &after))) {
    if (rc == EOF) break;    
    // printf("%d|%d", before, after);
    max_page = max(max_page, before);
    max_page = max(max_page, after);
    vec_int_push(&rules, before);
    vec_int_push(&rules, after);
  }

  int page;
  vec_int _init_update = MK_VEC(int);
  vec_vec_int_push(&updates, _init_update);
  vec_int* current_update = updates.start;


  vec_int_push(current_update, before);
  getc(input_f);

  char sep[2];
  for (;;) {
    rc = fscanf(input_f, "%u%1[^\n]", &page, sep);
     if (rc == EOF) break;
    max_page = max(max_page, page);
    vec_int_push(current_update, page);
    if (rc == 1) {
      vec_int new_page = MK_VEC(int);
      vec_vec_int_push(&updates, new_page);
      current_update = &updates.start[updates.size - 1];
    }
    
  }


  // adjacancy list for the "comes after" relation
  vec_vec_int adj = MK_VEC_WITH_CAP(int, max_page);
  for (int i = 0; i < max_page; i++) {
    vec_int _new_adj_list = MK_VEC(int);
    vec_vec_int_push(&adj, _new_adj_list);
  }

  for (int i = 0; i < rules.size; i += 2) {
    size_t u = rules.start[i];
    size_t v = rules.start[i+1];
    vec_int_push(&adj.start[v], u);
  }



  // find middle elements and sum them up

  printf("///%lu\n", updates.size);
  vec_int cur_update;
  unsigned long sum = 0;
  int h = 0;
  bool* contained = calloc(max_page, sizeof(bool));
  while (!vec_vec_int_pop(&updates, &cur_update)) {
    if (cur_update.size == 0) continue;
    // check if the relation holds
    bool rules_hold = true;
    for (unsigned int i = 0; i < cur_update.size; ++i) {
      int u = cur_update.start[i];
      vec_int* come_before = &adj.start[u];
      for (unsigned int j = 0; j < come_before->size; j++) {
        printf("CHECKING IF %d is contained\n", come_before->start[j]);
        rules_hold = contained[come_before->start[j]];
        if (!rules_hold) break;
      }
      if (!rules_hold) break;
      contained[u] = true;

    }
    for (unsigned int i = 0; i < cur_update.size; ++i) {
      contained[cur_update.start[i]] = false;
    }

    if (rules_hold) {
      sum += cur_update.start[cur_update.size / 2];
      printf("(%d) Adding: %d\n", h, cur_update.start[cur_update.size / 2]);
    }

    // free(cur_update.start);
    h++;
  }
  free(contained);

  printf("--> Q1: The sum of all middle pages is %lu\n", sum);
  

  vec_int to_free;
  // while (!vec_vec_int_pop(&updates, &to_free)) {
  //   printf("%p\n", to_free.start);
  //   // free(to_free.start);
  // }
  free(updates.start);
  free(adj.start);
  
  return 0;
}
