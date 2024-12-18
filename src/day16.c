#include "common.h"
#include "helpers.h"
#include <assert.h>
#include <stdio.h>

#define T int
#include "templates/vec.def"
#undef T

// #define N pos
// #include "templates/minheap.def"
// #undef N

int day16() {
  FILE* input_f = load_input(12);  
  assert(input_f);
  vec_int grid = MK_VEC(int);
  char c;
  int width = 0, i = 0;

  pos deer, goal;


  while ((c = getc(input_f)) != EOF) {
    if (c == 'S')
      deer = (pos){i / width, i % width};
    else if (c == 'E')
      goal = (pos){i / width, i % width};

    if (i > 0 && c == '\n') {
      width = i;
      i = 0;
    } else {
      vec_int_push(&grid, c == '#');
      i++;
    }
  }

  int height = grid.size / width;


  

  free(grid.start);

  
  return 0;
}
