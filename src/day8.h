#ifndef DAY8_H_
#define DAY8_H_

int day8();

typedef struct {
  int i, j;
} pos_t;

typedef enum {
  UNDEF = -1,
  EMPTY = 0,
  ANTINODE
} state;

#endif
