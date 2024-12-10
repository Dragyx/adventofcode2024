#include "day10.h"
#include "helpers.h"
#include "assert.h"
#include "stdbool.h"


#define T char
#include "templates/vec.def"
#undef T



// this is just simple depth first search (without culling recursive
// calls to already visited positions). Since the path is limited to a length
// of 9, the number of recursive calls is bounded pretty tightly
// TODO: It should be possible to compute the number of reachable "peaks" from
// a position and the number of distinct paths to a peak for each position
// with a single DFS call for a position, without erasing the visited grid
// afterwards 
scores calc_trailhead_score(vec_char* grid, vec_char* vis, int w, int h, int i, int j, char target) {
  if (i < 0 || j < 0 || i >= h || j >= w) return (scores){0, 0};
  
  if (grid->start[i * w + j] != target)
    return (scores){0, 0};

  bool visited = vis->start[i * w + j];

  vis->start[i * w + j] = 1;

  if (grid->start[i * w + j] == '9')
    return (scores){1, 1};
  
  scores s1 = calc_trailhead_score(grid, vis, w, h, i-1, j, target+1);
  scores s2 = calc_trailhead_score(grid, vis, w, h, i+1, j, target+1);
  scores s3 = calc_trailhead_score(grid, vis, w, h, i, j-1, target+1);
  scores s4 = calc_trailhead_score(grid, vis, w, h, i, j+1, target+1);

  return (scores){ 
    .reachable = !visited ? s1.reachable + s2.reachable + s3.reachable + s4.reachable : 0,
    .distinct_trails = s1.distinct_trails + s2.distinct_trails + s3.distinct_trails + s4.distinct_trails
  };
}

void erase_vis(vec_char* vis, int w, int h, int i, int j) {
  if (i < 0 || j < 0 || i >= h || j >= w) return;
  if (!vis->start[i * w + j]) return;
  vis->start[i * w + j] = 0;
  erase_vis(vis, w, h, i+1, j);
  erase_vis(vis, w, h, i-1, j);
  erase_vis(vis, w, h, i, j+1);
  erase_vis(vis, w, h, i, j-1);
}



int day10() {
  
  FILE* input_f = load_input(10);  
  assert(input_f);
  vec_char grid = MK_VEC(char);
  char c;
  int row_size = 0, i = 0;
  while ((c = getc(input_f)) != EOF) {
    if (i > 0 && c == '\n') {
      row_size = i;
      i = 0;
    } else {
      vec_char_push(&grid, c);
      i++;
    }
  }
  int height = grid.size / row_size;
  vec_char visited = MK_VEC_ZERO(char, grid.size);

  long score = 0, distinct_trails = 0;
  for (int i = 0; i < height; i++) {
    for (int j = 0; j < row_size; j++) {
      if (grid.start[i * row_size + j] != '0') continue;
      scores s =  calc_trailhead_score(&grid, &visited, row_size, height, i, j, '0');
      erase_vis(&visited, row_size, height, i, j);
      score += s.reachable;
      distinct_trails += s.distinct_trails;

    }
  }
  panswer(1, "The trailhead score is ", score);
  panswer(2, "The number of distinct trails is ", distinct_trails);


  free(grid.start);
  free(visited.start);

  return 0;
}
