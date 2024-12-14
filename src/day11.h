#ifndef DAY11_H_
#define DAY11_H_
#include <stdbool.h>

#define ULTRALONG_SIZE 4

int day11(); 

typedef struct {
  unsigned long l[ULTRALONG_SIZE];
} ultralong;

bool is_zero(ultralong* x);

bool is_ge(ultralong* a, ultralong* b);

unsigned int num_digits(ultralong* a);

// shifts x by delta (<= sizeof(unsigned int) bits to the left
// true on no overflow
bool shift_left_logical(ultralong* x, unsigned int delta);

ultralong split(ultralong* x, unsigned int digits);
// adds x to y, true on no overflow
bool add(ultralong* x, ultralong* y); 




#endif
