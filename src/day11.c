#include "day11.h"
#include "helpers.h"
#include <assert.h>
#include <stdbool.h>
#include <stdio.h>

bool is_zero(ultralong* x) {
  for (size_t i = 0; i < ULTRALONG_SIZE; i++)
    if (x->l[i] != 0) return false;

  return true;
}

bool is_ge(ultralong* a, ultralong* b) {
  for (size_t i = 0; i < ULTRALONG_SIZE; i++) {
    if (a->l[i] < b->l[i]) {
      return false;
    } else if (a->l[i] > b->l[i]) {
      return true;
    }
  }
  return true;
};

unsigned int num_digits(ultralong* a) {
  unsigned int digits = 1, ncopy;
  unsigned long mask = 1;
  // we count 
  for (size_t i = 0; i < ULTRALONG_SIZE; i++) {
    ncopy = a->l[i];
    while (ncopy > 0) {
      digits++;
      ncopy /= 10;
    } 
  }
  return digits;
}

bool add(ultralong* x, ultralong* y) {
  unsigned int carry = 0;
  for (size_t i = ULTRALONG_SIZE - 1; i >= 0; i--) {
    unsigned long before = x->l[i];
    x->l[i] += before + carry;
    carry = x->l[i] < before;
  }
  return !carry;
} 

bool shift_left_logical(ultralong* x, unsigned int delta) {
  unsigned long old_value = 0;
  for (size_t i = ULTRALONG_SIZE - 1; i >= 0; i--)  {
    old_value = x->l[i];
    x->l[i] <<= delta;
    x->l[i] |= old_value >> (sizeof(unsigned long) * 8 - delta);
  }
  return old_value == 0;
}

ultralong from_ulong(unsigned long x) {
  ultralong a = {0};
  a.l[ULTRALONG_SIZE - 1] = x;
  return a;
}


ultralong count_stones(ultralong n, int depth, int maxdepth) {
  if (depth >= maxdepth) {
    // printf("%ld ", n);
    return 1; 
  }
  depth++;
  if (n == 0) return count_stones(1, depth, maxdepth);
  int digits = 0, ncopy = n;
  ultralong mask = 1;
  do  {
    digits++;
    ncopy /= 10;
  } while (ncopy > 0);
  // printf("%ld: %d: %ld\n", n, digits, mask);
  if (digits % 2 == 0) {
    for (unsigned int i = 0; i < digits/2; i++)
      mask *= 10;
    return count_stones(n / mask, depth, maxdepth) + 
           count_stones(n % mask, depth, maxdepth);
    
  }
  if (2024LU * n <= n) { //overflow
    printf("BEFORE: %llx AFTER: %llx\n", n, ((ultralong)2024LU) * n);
  }
  return count_stones(2024U * n, depth, maxdepth);
}

int day11() {  
  FILE* input_f = load_input(11);  
  assert(input_f);
  int rc;
  unsigned long d, sum = 0;
  while ((rc = fscanf(input_f, "%lu", &d)) != EOF) {
    if (rc < 1) break;
    // printf("%ld: --> \n", d);

    sum += count_stones(d, 0, 25);
    // printf("\n");
  }
  printf("\n");
  panswer(1, "The total sum after blinking 25 times is: ", sum);
  printf("size: %lx \n", sizeof(__uint128_t));
  return 0;
}
