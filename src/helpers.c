#include "helpers.h"
#include "colors.h"
#include <stdio.h>
#include <string.h>

char* BASE_PATH = "inputs";

FILE* load_input(unsigned int day) {
  // prefix: input, suffix .txt
  char filename[strlen(BASE_PATH) + 6 + sizeof(day) + 5];
  sprintf(filename, "%s/input%d.txt", BASE_PATH, day);
  printf("Loading file %s\n", filename);
  return fopen(filename, "r");
}

void panswer(int q, char* s, long answer) {
  printf("%s--> Q%d:%s %s%s%ld%s\n", C_BLUE, q, C_RESET, s, C_GREEN, answer, C_RESET);
}

