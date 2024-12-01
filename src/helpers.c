#include "helpers.h"
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
