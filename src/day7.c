#include "day7.h"
#include "helpers.h"
#include <assert.h>
#include <stdbool.h>
#include <stdio.h>



#define T int
#include "vec.def"
#undef T

#define T vec_int
#include "vec.def"
#undef T

int read_input_line(FILE* input_f, vec_int* values, long* target) {
  if (fscanf(input_f, "%ld: ", target) < 1) 
    return 1;

  int next_val, rc;
  char c[2];
  while ((rc = fscanf(input_f, "%d%1[^\n]", &next_val, c)) == 2) {
    if (rc == EOF) break;
    // printf("%d ", next_val);
    vec_int_push(values, next_val);
  }
  if (rc == 1)
    vec_int_push(values, next_val);

  // free(line);
  return 0;
}

int production_possibilities(vec_int* values, long result, long first_n) {
  if (first_n == 1) {
    if (values->size == 0) {
      return 0;
    } else {
      return values->start[0] == result;
    }
  }
  int v = values->start[first_n - 1];
  
  int res_mul, res_add;

  if (result % v != 0) {
    // if the result is not divisible by
    // the last value, then it was surely not
    // produced by multiplication with this value
    res_mul = 0;
  } else {
    res_mul = production_possibilities(values, result / v, first_n - 1);
  }

  if (result - v < 0) {
    res_add = 0;
  } else {
    res_add = production_possibilities(values, result - v, first_n - 1);
  }
  
  return res_mul + res_add;
} 


int day7() {
  FILE* input_f = load_input(7);  
  assert(input_f);
  
  vec_int values = MK_VEC(int);
  long result = 0, target_sum, minimizer;
  while(!read_input_line(input_f, &values, &target_sum)) {
    int poss = production_possibilities(&values, target_sum, values.size);
    
    result  +=  poss != 0 ? target_sum : 0;
    // printf("Target sum: %ld", target_sum);
    
    // clear the vec
    values.size = 0;
  }

  printf("---> Q1: The sum of numbers that can be produced is: %ld", result);
  

  free(values.start);
  
  return 0;
}
