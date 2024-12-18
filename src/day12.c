#include "day12.h"
#include "helpers.h"
#include "common.h"
#include <assert.h>
#include <stdbool.h>
#include <stdlib.h>

#define T int
#include "templates/vec.def"
#undef T

#define T pos
#include "templates/vec.def"
#undef T


__always_inline bool in_bounds(int j, int i, int w, int h) {
  return i >= 0 && j >= 0 && j < w && i < h;
}

inline void mark_sides(vec_int* grid, sides* side_arr, int w, int h, int i, int j) {
  sides* cur_sides = &side_arr[i * w + j];
  cur_sides->N = !in_bounds(j, i - 1, w, h) || 
                 grid->start[i * w + j] != grid->start[(i - 1) * w + j];
  cur_sides->S = !in_bounds(j, i + 1, w, h) || 
                 grid->start[i * w + j] != grid->start[(i + 1) * w + j];
  cur_sides->W = !in_bounds(j - 1, i, w, h) || 
                 grid->start[i * w + j] != grid->start[i * w + j - 1];
  cur_sides->E = !in_bounds(j, i + 1, w, h) || 
                 grid->start[i * w + j] != grid->start[i * w + j + 1];
}
bool mark_interior(vec_int* grid, bool* vis, int w, int i, int j, plot_data *data, char plot_id) {
  int h = grid->size / w;
  if (
    !in_bounds(j, i, w, h) || 
    grid->start[i * w + j] != plot_id
  ) return false;
  if (vis[i * w + j])
    return true;
  vis[i * w + j] = true;
  mark_interior(grid, vis, w, i, j + 1, data, plot_id);
  mark_interior(grid, vis, w, i + 1, j, data, plot_id);
  mark_interior(grid, vis, w, i - 1, j, data, plot_id);
  mark_interior(grid, vis, w, i, j - 1, data, plot_id);

  data->area += 1;
  

  return true;
}

bool mark_plot(vec_int* grid, bool* vis, int w, int i, int j, plot_data* data) {
  int h = grid->size / w;

  // the direction the algorithm moves in (i, j)
  // it tries to traverse the edge of the plot first, before entering the inside
  pos dir = { 0, 1 };
  pos p = {i, j};

  if (!in_bounds(j, i, w, h) || vis[i * w + j])
    return false;

  
  vis[i * w + j] = true;
  // data->area += 1;
  char c = grid->start[i * w + j];

  // we track the number of successive turns we perform to avoid 
  // getting stuck on 1x1 plots
  int succ_turns = 0;

  vec_pos interior = MK_VEC(pos);
  printf("Starting walk with %d, %d\n", i, j);

  do {
    p.i += dir.i;
    p.j += dir.j;
    // we reached an edge
    if (!in_bounds(p.j, p.i, w, h) || grid->start[p.i * w + p.j] != c) {
      data->sides += 1;

      if (succ_turns == 4) break;

      // the additional length from the turn 
      // eg the following:
      // -->
      // A A v
      // A A 
      // turn clockwise and try again
      // printf("BEFORE: %d, %d\n", dir.i, dir.j);
      p.i -= dir.i;
      p.j -= dir.j;
      int tmp = dir.i;
      dir.i = dir.j;
      dir.j = -tmp;
      succ_turns += 1;
      // printf("AFTER: %d, %d\n", dir.i, dir.j);
    } else {
      // printf("setting (%d, %d)\n", p.i, p.j);
      vis[p.i * w + p.j] = true;
      data->area += 1;
      data->perimeter += 1;
      succ_turns = 0;
      // save position around this position to later process
      // the interiour of the plot using DFS
      vec_pos_push(&interior, p);
    }
    // printf("(%d, %d): (%d, %d) / (%d, %d)\n", i, j, p.i, p.j, dir.i, dir.j);
  } while (p.i != i || p.j != j || dir.i != 0 || dir.j != 1);


  // call dfs on the potentially interiour parts of the plot
  pos plot_inner;
  while ((!vec_pos_pop(&interior, &plot_inner))) {
   mark_interior(grid, vis, w, plot_inner.i, plot_inner.j, data, c);
  }
  
  
  free(interior.start);
  
  return true;
}


int day12() {

  FILE* input_f = load_input(12);  
  assert(input_f);
  vec_int grid = MK_VEC(int);
  char c;
  int width = 0, i = 0;

  while ((c = getc(input_f)) != EOF) {
    if (i > 0 && c == '\n') {
      width = i;
      i = 0;
    } else {
      vec_int_push(&grid, c);
      i++;
    }
  }

  int height = grid.size / width;
  bool* vis = calloc(grid.size, sizeof(bool));
  sides* side_arr = calloc(grid.size, sizeof(sides));
  
  long price = 0;
  for (int i = 0; i < height; i++) {
    for (int j = 0; j < width; j++) {
      plot_data data = {0};
      bool new = mark_plot(&grid, vis, width, i, j, &data);
      if (new && data.area > 0) {
        printf("(%d, %d) -> PLOT OF %c WITH AREA: %d, PERIMETER: %d, SIDES: %d\n", i, j, grid.start[i * width + j], data.area, data.perimeter, data.sides);
        price += data.area * data.perimeter;
      }
      // return 1;
    }
  }

  panswer(1, "The price is ", price);
  
  free (vis);
  free (side_arr);
  free(grid.start);
  return 0;
}
