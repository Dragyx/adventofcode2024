#include <assert.h>
#include <string.h>
#include <stdbool.h>
#include "helpers.h"
#include "sys/cdefs.h"
#include "day6.h"

#define T int
#include "templates/vec.def"
#undef T

typedef struct {
  pos p;
  int dir;
} visited;

#define T visited
#include "templates/vec.def"
#undef T


__always_inline bool dir_marked(pos p, direction dir, const vec_int * const grid, int row_size) {
  int *v = &grid->start[p.y * row_size + p.x];
  return *v > 0 && (*v >> dir) & 0x1;
}

__always_inline bool is_valid_pos(pos* p, int w, int h) {
  return (p->x < w && p->x >= 0) &&
         (p->y < h && p->y >= 0);
}

pos dir_to_delta(direction d) {
  pos p = {0, 0};
  switch (d) {
    case NORTH: p.y = -1; 
      break;
    case SOUTH: p.y = 1;
      break;
    case EAST: p.x = 1;
      break;
    case WEST: p.x = -1;
      break;
    default: break;
  }
  return p;
}
// rotates a position by 90 degrees clockwise
__always_inline void rot_90(direction* d) {
  // d + 1 mod 4 (cycles through directions)
  *d = (*d + 1) & 0x03;
}

void print_grid(vec_int grid, int row_size, pos* obs, vec_visited* vis) {

    for (int i = 0; i < grid.size / row_size; i++) {
      for (int j = 0; j < row_size; j++) {
        bool pos_vis = false;
        if (vis != NULL)
          for (int k = 0; k < vis->size; k++) {
            pos p = vis->start[k].p;
            // printf("(%d, %d)::", p.x, p.y);
            if (p.x == j && p.y == i) {
              pos_vis = true;
              break;
            }
          }

        bool is_obs = obs != NULL && obs->x == j && obs->y == i;

        
        char c = is_obs ? 'O' :
                 pos_vis ? '~' :
                 grid.start[i * row_size + j] == -1 ? '#' :
                 dir_marked((pos){j, i}, 0, &grid, row_size) ? '>' :
                 dir_marked((pos){j, i}, 1, &grid, row_size) ? 'v' :
                 dir_marked((pos){j, i}, 2, &grid, row_size) ? '<' :
                 dir_marked((pos){j, i}, 3, &grid, row_size) ? '^' :
                 grid.start[i * row_size + j] == UNKNOWN ? ' ': '?';
        printf("%c", c);
      }
      printf("\n");
    }
}

bool creates_cycle(pos guard, direction dir, vec_int*  grid, int row_size) {
  // obstacle on the grid
  pos obs = { guard.x + dir_to_delta(dir).x, 
              guard.y + dir_to_delta(dir).y };
  int height = grid->size / row_size;

  // either the grid was visited before or there is an obstacle
  // ; either way, this can not be an additional obstacle
  if (grid->start[obs.y * row_size + obs.x] || !is_valid_pos(&guard, row_size, height))
    return false;
  
  bool cycle = false;

  // save changed values for cleanup
  vec_visited changed = MK_VEC(visited);

  while(is_valid_pos(&guard, row_size, height)) {
    int* cur_field = &grid->start[guard.y * row_size + guard.x];
    // mark // check for previous marking
    if (*cur_field > 0 && ((*cur_field) & (1 << dir))) {
      cycle = true;
      break;
    } else {
      *cur_field ^= 1 << dir;
      vec_visited_push(&changed, (visited){guard, dir});
    };

    // advance
    pos delta = dir_to_delta(dir);
    pos new_pos = { guard.x + delta.x, guard.y + delta.y};
    if (!is_valid_pos(&new_pos, row_size, height)) break;
    int* next_field = &grid->start[new_pos.y * row_size + new_pos.x];
    
    if (*next_field == OBSTACLE || (new_pos.x == obs.x && new_pos.y == obs.y)) {
      rot_90(&dir);
    } else {
      guard = new_pos;
    }

  }

  // clean up and retorn

  int rc;
  visited changed_pos;
  while (!(rc = vec_visited_pop(&changed, &changed_pos))) {
    pos p = changed_pos.p; 
    // toggle direction back to old value
    grid->start[p.y * row_size + p.x] ^= 1 << changed_pos.dir;
  }
  assert(rc != 2);
  free(changed.start);

  return cycle;
}

int day6() {  
  FILE* input_f = load_input(6);  
  assert(input_f);
  vec_int grid = MK_VEC(int);
  char c;
  int row_size = 0, i = 0;
  pos guard = {-1, -1};
  while ((c = getc(input_f)) != EOF) {
    if (i > 0 && c == '\n') {
      // prevent different sized rows
      // if (row_size && row_size != i)
      //   return 1;
      row_size = i;
      i = 0;
    } else {
      vec_int_push(&grid, c == '#' ? OBSTACLE : UNKNOWN);
      if (c == '^') {
        guard.x = i;
        guard.y = grid.size / row_size;
      }
      i++;
    }
  }
  if (guard.x == -1)
    return 1;

 
  int height = grid.size / row_size;
  int distinct_pos = 0;
  int cycle_obstacles = 0;
  direction dir = NORTH;
  while(is_valid_pos(&guard, row_size, height)) {
    int* cur_field = &grid.start[guard.y * row_size + guard.x];
    // mark // check for previous marking

    distinct_pos += *cur_field == UNKNOWN;
    vec_int grid2 = MK_VEC_ZERO(int, grid.size);
    memcpy(grid2.start, grid.start, sizeof(int) * grid.size);
    cycle_obstacles += creates_cycle(guard, dir, &grid, row_size);
    assert(!memcmp(grid.start, grid2.start, sizeof(int) * grid.size));

    free(grid2.start);

    // this field cannot be marked in the same direction yet
    assert(!(*cur_field > 0 && ((*cur_field) & (1 << dir))));
    
    *cur_field ^= 1 << dir;

    // advance
    pos delta = dir_to_delta(dir);
    pos new_pos = { guard.x + delta.x, guard.y + delta.y};
    if (!is_valid_pos(&new_pos, row_size, height)) break;

    int* next_field = &grid.start[new_pos.y * row_size + new_pos.x];    
    if (*next_field == OBSTACLE) {
      rot_90(&dir);
    } else {
      guard = new_pos;
    }
  }

  panswer(1, "Number of distinct positions: ", distinct_pos);
  panswer(2, "Number of possible obstacles: ", cycle_obstacles);

  // we now check for rectangular regions


  free(grid.start);

  
  return 0;
}
