#include "day1.h"
#include "day11.h"
#include "day2.h"
#include "day3.h"
#include "day4.h"
#include "day5.h"
#include "day6.h"
#include "day7.h"
#include "day8.h"
#include "day9.h"
#include "day10.h"
#include "colors.h"
#include <assert.h>



long vec_resize_count = 0;
size_t total_cycle_count = 0;

const unsigned PERF_FLAG = (1 << 30) + 1; 


// Function to read the Time Stamp Counter
static inline size_t rdtsc()
{
  unsigned int lo, hi;
  // __asm__ __volatile__(
  //     "rdpmc"
  //     : "=a"(lo), "=d"(hi) // Outputs: low and high bits of the TSC
  //     : "c"(PERF_FLAG)
  // );
  __asm__ __volatile__(
      "rdtsc"
      : "=a"(lo), "=d"(hi) // Outputs: low and high bits of the TSC
      // : "c"(PERF_FLAG)
  );
  return ((size_t)hi << 32) | lo;
}


void execute_task(int (*f)(void)) {
  int rc;
  size_t t_before = rdtsc();
  rc = f(); 
  size_t t_after = rdtsc();
  size_t time = t_after - t_before; 
  printf(C_YELLOW "TOOK: %10lu = %.2e cycles\n\n" C_RESET, time, (double)time);
  total_cycle_count += time;

  assert(!rc);
}


int main() {
  execute_task(day1);
  execute_task(day2);
  execute_task(day3);
  execute_task(day4);
  execute_task(day5);
  execute_task(day6);
  execute_task(day7);
  execute_task(day8);
  execute_task(day9);
  execute_task(day10);
  execute_task(day11);
  
  printf(C_MAGENTA "RESIZES: %ld\n", vec_resize_count);
  printf("CYCLES:  %lu = %.2e \n" C_RESET, total_cycle_count, (double)total_cycle_count);
}

