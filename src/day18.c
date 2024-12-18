#include "common.h"
#include "helpers.h"
#include <string.h>
#include <stdbool.h>
#include <assert.h>
#include <stdio.h>
#include "day18.h"

// this time, the input size is specified int
// the actual task description
#define GRID_SIZE 71
#define TIMESTEPS 1024

// for saving the input positions
#define T pos
#include "templates/vec.def"
#undef T


__always_inline bool in_bounds(pos p) {  
    return p.i >= 0 && p.j >= 0 && p.i < GRID_SIZE && p.j < GRID_SIZE;
}


void bfs(int (*grid)[GRID_SIZE][GRID_SIZE],
         pos (*possible_pos)[GRID_SIZE][GRID_SIZE]) {
  
    queue to_process = {NULL, NULL, 0};
    // start at the top left corner
    enqueue(&to_process, possible_pos[0][0]);
    (*grid)[0][0] = 0;
    void* pos_ptr;
    while ((pos_ptr = dequeue(&to_process)) != NULL) { 
      pos p = *((pos*)pos_ptr);

      // we reached the position
      if (p.i == GRID_SIZE-1 && p.j == GRID_SIZE-1)
        break;

      pos next[4] = {
        {p.i + 1, p.j},
        {p.i - 1, p.j},
        {p.i, p.j + 1},
        {p.i, p.j - 1},
      };
      for (size_t i = 0; i < sizeof(next)/sizeof(pos); i++) {
        pos next_p = next[i];
        // wall and bounds check
        if (!in_bounds(next_p) || (*grid)[next_p.i][next_p.j] == -2)
          continue;
        // check if we already visited this neighbor
        if ((*grid)[next_p.i][next_p.j] >= 0) 
          continue;
        (*grid)[next_p.i][next_p.j] = (*grid)[p.i][p.j] + 1;
        enqueue(&to_process, &(*possible_pos)[next_p.i][next_p.j]);
      }
    }
  free_nodes(&to_process);  
}


int day18() {
  // -1: uninitialized
  // -2: corrupt location
  // >0: distance
  int grid[GRID_SIZE][GRID_SIZE];
  // this is so I can reference the positions in my queue
  pos possible_pos[GRID_SIZE][GRID_SIZE];
  for (size_t i = 0; i < GRID_SIZE; i++) {
    for (size_t j = 0; j < GRID_SIZE; j++) {   
      grid[i][j] = -1;
      possible_pos[i][j] = (pos){i, j};
    }
  }
  vec_pos b_positions = MK_VEC_WITH_CAP(pos, TIMESTEPS);


  FILE* input_f = load_input(18);  
  assert(input_f);

  size_t x, y;
  int num_read;

  // note that i store the positions as (i, j) where
  // i is the row
  while ((num_read = fscanf(input_f, "%ld,%ld", &x, &y)) != EOF) {
    if (num_read < 2) break;
    vec_pos_push(&b_positions, (pos){y, x});
  }
  if (b_positions.size < TIMESTEPS)
    return 1;

  for (size_t t = 0; t < TIMESTEPS; t++) {
    pos p = b_positions.start[t];
    grid[p.i][p.j] = -2;
  }

  bfs(&grid, &possible_pos);

  panswer(1, "The length of the smallest path is:", grid[GRID_SIZE-1][GRID_SIZE-1]);
  

  // reset grid
    for (size_t i = 0; i < GRID_SIZE; i++) {
      for (size_t j = 0; j < GRID_SIZE; j++) {
        grid[i][j] = -1;
      }  
    }

  // repeatedly find the shortest path until
  // there is no shortest path anymore
  size_t t = 0;
  for (; t < b_positions.size; t++) {
    // reset grid
    for (size_t i = 0; i < GRID_SIZE; i++) {
      for (size_t j = 0; j < GRID_SIZE; j++) {
        if (grid[i][j] >= 0) grid[i][j] = -1;
      }  
    }
    pos new_p = b_positions.start[t];
    grid[new_p.i][new_p.j] = -2;
    bfs(&grid, &possible_pos);
    // stop as soon as there is no path anymore
    if (grid[GRID_SIZE-1][GRID_SIZE-1] < 0)
      break;
  }
  // for (int i = 0; i < b_positions.size; i++) {
  //   printf("(%d, %d)\n", b_positions.start[i].i, b_positions.start[i].j);
  // }
  pos blocking = t < b_positions.size ? b_positions.start[t] : (pos){-1, -1};
  panswer(2, "The coordinates of the first byte that destroys the path is ", 0);
  printf("\t--> (%d, %d)\n", blocking.j, blocking.i);



  
  free(b_positions.start);
  
  return 0;
}
