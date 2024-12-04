

#include "helpers.h"
#include <assert.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>

#define T char
#include "vec.def"
#undef T

typedef struct {
  vec_char inner;
  size_t row_size;
} input_map;

// i is the line, j is the character
// NOTE: bounds checking is on the caller
inline char get(input_map* map, size_t i, size_t j) {
  return map->inner.start[i * map->row_size + j];
}

const int check_dirs[8][2] = {
  {0, -1},
  {0, 1},
  {1, -1},
  {1, 0},
  {1, 1},
  {-1, -1},
  {-1, 0},
  {-1, 1},
};
 
const int dirs_ll_ru[2][2] = {
  {-1, 1},
  {1, -1}
};

const int dirs_lu_rl[2][2] = {
  {-1, -1},
  {1, 1}
};

// counts the number of occurrences of this word in all directions
// N E S W and diagonally in all four directions
unsigned int count_star(input_map* map, size_t i, size_t j, char* w, const int (*directions)[2], size_t num_dir) {
  size_t wl = strlen(w);
  // size of the map (matrix)
  int n = map->row_size;
  int m = map->inner.size / n;
  unsigned int sum = 0;
  // bi, bj are the maximal positions (-1) we need to check
  // di, dj the direction we need to go to
  int di, dj;
  int bi, bj;
  // printf("--------------\n");
  for (unsigned int h = 0; h < num_dir; ++h) {
    di = directions[h][0]; 
    dj = directions[h][1]; 
    assert(!(di == 0 && dj == 0));
    bi = wl * di  + i;
    bj = wl * dj  + j;
    assert(!(bi == i && bj == j));

    if (bi < -1 || bi > m || bj < -1 || bj > n)
      continue;

    bool matching = true;
    unsigned int word_position = 0;
    for (unsigned int k = i, l = j; k != bi || l != bj; k += di, l += dj) {
        matching = matching && get(map, k, l) == w[word_position];  
        if (!matching) break;
        word_position++;
    }
    sum += matching;
  }
  return sum;
}


char X_patterns[4][3][3] = {0};


int day4() {
  FILE* input_f = load_input(4);
  assert(input_f);
  fseek(input_f, 0, SEEK_END);
  size_t length = ftell(input_f);
  fseek(input_f, 0, SEEK_SET);


  
  // read input
  input_map map = {
    .inner = MK_VEC_ZERO(char, length),
    .row_size = 0    
  };

  char c;
  int row_start = 0;
  for (size_t i = 0; (c = getc(input_f)) != EOF; ++i) {
      if (c == '\n') {
        size_t row_size = i - row_start;
        // check if all rows have the same size
        if (map.row_size && row_size != map.row_size) {
          return 1;
        }
        map.row_size = row_size;
        row_start = i + 1;
      } else {
        vec_char_push(&map.inner, c);
      }
  }


  // count occurrences

  int rows = map.inner.size / map.row_size;
  int cols = map.row_size;
  unsigned long sum = 0;
  unsigned long sum_2 = 0;
  for (unsigned int i = 0; i < rows; ++i) {
    for (unsigned int j = 0; j < cols; ++j) {
      // task 1;
      int c = count_star(&map, i, j, "XMAS", check_dirs, 8);
      sum += c;
      int diag1_correct = count_star(&map, i, j, "AM", dirs_ll_ru, 2) *
                          count_star(&map, i, j, "AS", dirs_ll_ru, 2);
      int diag2_correct = count_star(&map, i, j, "AM", dirs_lu_rl, 2) *
                          count_star(&map, i, j, "AS", dirs_lu_rl, 2);
      sum_2 += diag1_correct * diag2_correct;

    }
  }

  
  printf("--> Q1: The number of occurences of XMASis: %lu\n", sum);
  printf("--> Q2: The number of occurences of an X of MAS is: %lu\n", sum_2);
  free(map.inner.start);

  return 0;
}
