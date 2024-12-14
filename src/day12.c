#include "day12.h"
#include "helpers.h"
#include "sys/cdefs.h"
#include <assert.h>
#include <stdbool.h>

#define T int
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

bool mark_plot(vec_int* grid, bool* vis, sides* side_arr, int w, int i, int j, plot_data* data, char plot_id) {
  int h = grid->size / w;
  if (
    !in_bounds(j, i, w, h) || 
    grid->start[i * w + j] != plot_id
  ) return false;
  if (vis[i * w + j])
    return true;
  vis[i * w + j] = true;
  mark_sides(grid, side_arr, w, h, i, j);
  bool p_E = mark_plot(grid, vis, side_arr, w, i, j + 1, data, plot_id);
  bool p_S = mark_plot(grid, vis, side_arr, w, i + 1, j, data, plot_id);
  bool p_W = mark_plot(grid, vis, side_arr, w, i - 1, j, data, plot_id);
  bool p_N = mark_plot(grid, vis, side_arr, w, i, j - 1, data, plot_id);

  data->perimeter += !p_E + !p_S + !p_W + !p_N;
  data->area += 1;
  

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
      bool new = mark_plot(&grid, vis, side_arr, width, i, j, &data, grid.start[i * width + j]);
      if (new && data.area > 0) {
        // printf("(%d, %d) -> PLOT OF %c WITH AREA: %d, PERIMETER: %d\n", i, j, grid.start[i * width + j], data.area, data.perimeter);
        price += data.area * data.perimeter;
      }
    }
  }

  panswer(1, "The price is ", price);
  
  free (vis);
  free (side_arr);
  free(grid.start);
  return 0;
}
