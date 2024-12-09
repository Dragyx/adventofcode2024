#include "helpers.h"
#include "sys/cdefs.h"
#include <assert.h>
#include <ctype.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include "day8.h"

#define T u_char
#include "vec.def"
#undef T

#define T pos_t
#include "vec.def"
#undef T


int read_input(
               FILE* f, 
               vec_pos_t antennas[128], 
               vec_u_char *used_frequencies, 
               vec_u_char* grid,
               size_t *w, 
               size_t *h
               ) {

  char c;
  int row_size = 0, i = 0, values_read = 0;
  while ((c = getc(f)) != EOF) {
    if (i > 0 && c == '\n') {
      // prevent different sized rows
      // if (row_size && row_size != i)
      //   return 1;
      row_size = i;
      i = 0;
    } else {
      values_read++;
      if (isalnum(c)) {
        if (antennas[(size_t)c].size == 0) {
          vec_u_char_push(used_frequencies, c);
        }
        vec_pos_t_push(&antennas[(size_t)c], (pos_t){row_size ? (values_read - 1) / row_size : 0, row_size ? (values_read - 1) % row_size: values_read - 1});
      }
      i++;
    }
  }

  *grid = (vec_u_char)MK_VEC_ZERO(u_char, values_read);
  *w = row_size;
  *h = values_read / row_size; 

  return 0;
}

__always_inline bool in_bounds(pos_t p, size_t w, size_t h) {
  return (p.i >= 0 && p.i < h && p.j >= 0 && p.j < w); 
}

void print_antennas(vec_pos_t antennas[128]) {
  for (u_char c = 0; c < 128; c++) {
    if (antennas[c].size != 0) {
      printf("%c: ", c);
      for (int i = 0; i < antennas[c].size; i++) {
        pos_t p = antennas[c].start[i];
        printf("(%2d, %2d) ", p.i, p.j);
      }
      printf("\n");
    }
  }
}

int gcd(int a, int b) {
  while (b != 0) {
    int t = b;
    b = a % b;
    a = t;
  }
  return a;
}


int day8() {
  FILE* input_f = load_input(8);
  assert(input_f);

  // for every frequency, we save the locations on the map.
  //  (Yes, what I really want is a hashmap, but I'm too lazy
  //   to implement one)
  // 128, because ASCII is 7 bit (pos_titive range of unsigned char)
  vec_pos_t antennas[128];  

  for (u_char c = 0; c < 128; c++)
    antennas[c] = (vec_pos_t)MK_VEC(pos_t);

  // this array contains all frequencies that are used on the map
  // (=> the indices of grid that contain a non-empty vec)
  vec_u_char used_frequencies = MK_VEC(u_char);

  // a grid containing information on wheter there already is an antinode
  // on a given location
  vec_u_char grid;
  size_t w, h;

  read_input(input_f, antennas, &used_frequencies, &grid, &w, &h);


  
  // print_antennas(antennas);:w
  
  for (int i = 0; i < grid.size; i++)
    assert(grid.start[i] == 0);

  int antinode_count = 0;
  // search for antinodes
  for (size_t i = 0; i < used_frequencies.size; i++) {
    u_char f = used_frequencies.start[i];
    vec_pos_t* locations = &antennas[f];
    for (int k = 0; k < locations->size; k++) {
      for (int l = 0; l < locations->size; l++) {
        if (k == l) continue;
        pos_t p1 = locations->start[k];
        pos_t p2 = locations->start[l];
        pos_t anode = {
          p1.i + (p1.i - p2.i),
          p1.j + (p1.j - p2.j),
        };
        if (in_bounds(anode, w, h) && grid.start[anode.i * w + anode.j] != ANTINODE) {
          grid.start[anode.i * w + anode.j] = ANTINODE;
          antinode_count++;
                    
        }
      }
    }

  }
  printf("--> Q1: The number of locations with antinodes is %d\n", antinode_count);

  // reset array
  for (int i = 0; i < grid.size; i++)
    grid.start[i] = 0;


  int antinode_count2 = 0;
  // search for antinodes
  for (size_t i = 0; i < used_frequencies.size; i++) {
    u_char f = used_frequencies.start[i];
    vec_pos_t* locations = &antennas[f];
    for (int k = 0; k < locations->size; k++) {
      for (int l = 0; l < locations->size; l++) {
        if (k == l) continue;
        pos_t p1 = locations->start[k];
        pos_t p2 = locations->start[l];
        if (f == 'c') printf("::: Starting walk at (%d, %d) - (%d, %d)\n", p1.i, p1.j, p2.i, p2.j);
        int point_gcd = gcd(abs(p1.i - p2.i), abs(p1.j - p2.j));
        pos_t delta = {
          (p1.i - p2.i) / point_gcd, 
          (p1.j - p2.j) / point_gcd,
        };

        for (int dir = -1; dir <= 1; dir += 2) {
          pos_t anode = {p1.i, p1.j};
          while (in_bounds(anode, w, h)) {
            if (f == 'c') {
              printf("(%d, %d) -> \n", anode.i, anode.j);
            }
            if (grid.start[anode.i * w + anode.j] != ANTINODE) {
              grid.start[anode.i * w + anode.j] = ANTINODE;
              antinode_count2++;
            }
            anode.i += delta.i * dir;
            anode.j += delta.j * dir;
          
          }
        }
      }
    }

  }
  printf("--> Q2: The number of new antinodes is %d\n", antinode_count2);
  


  
  // clean up
  free(used_frequencies.start);
  free(grid.start);
  for (u_char c = 0; c < 128; c++) {
    free(antennas[c].start);
  }

  return 0;
}
