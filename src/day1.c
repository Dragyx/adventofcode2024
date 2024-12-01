#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>

#include "day1.h"
#include "helpers.h"

#define COULD_NOT_OPEN_FILE 1

// comparison function for sorting
int comp(const void * el1, const void * el2) {
  unsigned int l = *((unsigned int*) el1);
  unsigned int r = *((unsigned int*) el2);
  return (l > r) - (l < r);
}


int day1() {
  FILE* input = load_input(1);
  if (input == NULL) return COULD_NOT_OPEN_FILE;
  unsigned int* list1, *list2;
  // we dynamically allocate list1 and list2 by creating
  // a simple arraylist
  list1 = malloc(sizeof(unsigned int));
  list2 = malloc(sizeof(unsigned int));
  unsigned int size, read;
  size = read = 1;
  unsigned int v1, v2;
  while ((fscanf(input, "%d %d\n", &v1, &v2)) != EOF) {
    if (read >= size) {
      size <<= 1;
      list1 = realloc(list1, size * sizeof(unsigned int));
      list2 = realloc(list2, size * sizeof(unsigned int));
    }
    list1[read] = v1;
    list2[read] = v2;
    read++;
  }
  // resize the two arrays to not waste space
  list1 = realloc(list1, read * sizeof(unsigned int));
  list2 = realloc(list2, read * sizeof(unsigned int));

  // sort the lists and compare
  qsort(list1, read, sizeof(unsigned int), comp);
  qsort(list2, read, sizeof(unsigned int), comp);

  unsigned int diff = 0;
  for (unsigned int i = 0; i < read; ++i) {
    diff += abs((int)list1[i] - (int)list2[i]);
  }

  printf("--> Q1: The difference is \t%d\n", diff);

  // we use a very basic O(n^2) way to count the occurences for all values in list1
  // of course, binary search could be used, but I won't bother

  int64_t score = 0;
  for (unsigned int i = 0; i < read; ++i) {
    unsigned int to_search = list1[i];
    unsigned int count = 0;
    for(unsigned int j = 0; j < read; j++)
      count += list2[j] == to_search;
    score += to_search * count;
  }
  
  printf("--> Q2: The total score is \t%ld\n", score);


  return 0;
}
