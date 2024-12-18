#ifndef DAY17_H_
#define DAY17_H_


int day17();

typedef struct {
  long A, B, C;
} reg_file;

typedef enum {
  ADV = 0,
  BXL = 1,
  BST = 2,
  JNZ = 3,
  BXC = 4,
  OUT = 5,
  BDV = 6,
  CDV = 7
} INST_T;

typedef struct {
  INST_T t;
  int op;
} inst;

#endif
