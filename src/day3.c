#include <assert.h>
#include <string.h>
#include <stdio.h>
#include <regex.h>
#include <stdlib.h>
#include <stdbool.h>

#include "day3.h"
#include "datastructures.h"
#include "helpers.h"


#define ARRAY_SIZE(arr) (sizeof((arr)) / sizeof((arr)[0]))

DEF_VEC(inst);
DEF_VEC_POP(inst);
DEF_VEC_PUSH(inst);

regex_t inst_fmt, do_fmt, dont_fmt;
int inst_fmt_rc, do_fmt_rc, dont_fmt_rc;

int day3() {
  inst_fmt_rc = regcomp(&inst_fmt, "(mul\\(([0-9]{1,3}),([0-9]{1,3})\\))|(do)|(don't)", REG_EXTENDED);
    
  assert(!inst_fmt_rc);

  // read input
  FILE* input_f = load_input(3);
  assert(input_f);
  fseek(input_f, 0, SEEK_END);
  long length = ftell(input_f);
  fseek(input_f, 0, SEEK_SET);
  char* input = malloc(length);
  assert(input);

  fread(input, 1, length, input_f);
  fclose(input_f);

  char* cur_pos = input;
  regmatch_t pmatch[4];
  unsigned int sum = 0;
  unsigned int sum_t2 = 0;

  bool enabled = true;

  // printf("%s", input);
  // largely taken from the regex manpage
  while (cur_pos <= input + length - 1) {
    int rc = regexec(&inst_fmt, cur_pos, ARRAY_SIZE(pmatch), pmatch, 0);
    // no more matches
    if (rc == 1) break;


    size_t match_size = pmatch[0].rm_eo - pmatch[0].rm_so;
    char match[13] = { 0 };
    strncpy(match, cur_pos + pmatch[0].rm_so, match_size);
    if (!strcmp(match, "don't")) {
       enabled = false;
    } else if (!strcmp(match, "do")) {
       enabled = true;   
    } else {
      // the digits can only ever have a maximum size of 3
      char numl[4];
      char numr[4];
      strncpy(numl, cur_pos + pmatch[2].rm_so, 3);
      strncpy(numr, cur_pos + pmatch[3].rm_so, 3);
      int lval = strtol(numl, NULL, 10);
      int rval = strtol(numr, NULL, 10);
      sum += lval * rval;
      sum_t2 += (lval * rval) * enabled;
    }

    cur_pos += pmatch[0].rm_eo;
  }

  printf("--> Q1: Sum of muls: %d\n", sum);
  printf("--> Q2: Sum of muls (enable + disable): %d\n", sum_t2);
  

  free(input);
  regfree(&inst_fmt);
  return 0;
}

