#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>

#include "day1.h"
#include "helpers.h"

#define COULD_NOT_OPEN_FILE 1


#define T int
#include "vec.def"
static void* _ = vec_int_pop;
#undef T

// comparison function for sorting
int comp(const void * el1, const void * el2) {
  int l = *((int*) el1);
  int r = *((int*) el2);
  return (l > r) - (l < r);
}


int day1() {
  FILE* input = load_input(1);
  if (input == NULL) return COULD_NOT_OPEN_FILE;
  vec_int list1 = MK_VEC(int), list2 = MK_VEC(int);
  unsigned int v1, v2;
  while ((fscanf(input, "%d %d\n", &v1, &v2)) != EOF) {
    vec_int_push(&list1, v1);
    vec_int_push(&list2, v2);
  }

  // sort the lists and compare
  qsort(list1.start, list1.size, sizeof(int), comp);
  qsort(list2.start, list2.size, sizeof(int), comp);


  unsigned int diff = 0;
  for (unsigned int i = 0; i < list1.size; ++i) {
    diff += abs(list1.start[i] - list2.start[i]);
  }

  printf("--> Q1: The difference is \t%d\n", diff);

  // we use a very basic O(n^2) way to count the occurences for all values in list1
  // of course, binary search could be used, but I won't bother

  int64_t score = 0;
  for (unsigned int i = 0; i < list1.size; ++i) {
    unsigned int to_search = list1.start[i];
    unsigned int count = 0;
    for(unsigned int j = 0; j < list2.size; j++)
      count += list2.start[j] == to_search;
    score += to_search * count;
  }
  
  printf("--> Q2: The total score is \t%ld\n", score);


  return 0;
}
