#ifndef DAY6_H_
#define DAY6_H_

int day6();

typedef struct {
  int x, y;
} pos;

typedef enum {
  UNKNOWN = 0,
  OBSTACLE = -1,
  VIS_E = 0b0001,
  VIS_S = 0b0010,
  VIS_W = 0b0100,
  VIS_N = 0b1000,
} field;

// the ordering matters
typedef enum {
  EAST = 0,
  SOUTH,
  WEST,
  NORTH,
} direction;


#endif
