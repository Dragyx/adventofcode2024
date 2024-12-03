#include "helpers.h"
#include <stdio.h>
#include <stdbool.h>


#define T int
#include "vec.def"
// to avoid unused funtion warnings
void* _ = vec_int_pop;
#undef T
#define T vec_int
#include "vec.def"
#undef T

// checks if the report is safe. Has the option to ignore
// an entry (for task 2)
bool is_safe(vec_int* report, int ignore) {
  if (report->size == 0) return false;
  int last = *report->start;
  bool increasing = true;
  bool decreasing = true;

  int i = 1;
  if (ignore == 0) {
    if (report->size == 1) return true;
    last = report->start[1];
    i++;
  }
  for (; i < report->size; i++) {
    if (i == ignore) continue;
    int cur = report->start[i];
    int d = abs(last - cur);
    if (d < 1 || d > 3) {
      return false;
    }
    if (cur < last) {
      increasing = false;
    } else if (cur > last) {
      decreasing = false;
    }
    last = cur;
  }
  return increasing || decreasing;
}

int day2() {
  FILE* file = load_input(2);
  if (file == NULL) return 1;

  int cur_val;
  vec_vec_int reports = MK_VEC(vec_int);

  
  vec_int new_report = MK_VEC(int);
  vec_vec_int_push(&reports, new_report);

  while ((fscanf(file, "%d", &cur_val) != EOF)) {
    char sep = getc(file);
    if (sep == '\n' || sep == ' ')
      vec_int_push(&reports.start[reports.size - 1], cur_val);
    if (sep == '\n') {
      vec_int new_report = MK_VEC(int);
      vec_vec_int_push(&reports, new_report);
    }
  }


  int count_task1 = 0;
  int count_task2 = 0;
  for (int i = 0; i < reports.size; i++) {
    vec_int* report = &reports.start[i];
    count_task1 += is_safe(report, -1);
    for (int j = 0; j < report->size; j++) {
      if (is_safe(report, j)) {
        count_task2 += 1;
        break;
      }
    }
  }

  printf("--> Q1: Number of safe reports: %d\n", count_task1);
  printf("--> Q2: Number of safe reports: %d\n", count_task2);

  // cleanup
  vec_int report;
  while (!vec_vec_int_pop(&reports, &report)) {
    free(report.start);
  }
  free(reports.start);
  
  return 0;
}
