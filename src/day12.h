#ifndef DAY12_H_
#define DAY12_H_

#include <stdbool.h>

int day12();

typedef struct {
  unsigned int perimeter, area, sides;
} plot_data;

typedef struct {
  bool N, E, S, W;
} sides;

#endif
