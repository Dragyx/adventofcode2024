#include "day17.h"
#include "helpers.h"
#include <assert.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>

#define T inst
#include "templates/vec.def"
#undef T

#define T int
#include "templates/vec.def"
#undef T


long to_combo(long op, reg_file* reg) {
  switch (op) {
    case 4:
      return reg->A;
    case 5: 
      return reg->B;
    case 6: 
      return reg->C;
    default: 
      return op; 
  }
}

int process_instr(reg_file* reg, vec_inst* instrs, size_t *inst_ptr, vec_int* out, bool match_output) {
  long op = instrs->start[*inst_ptr].op;
  INST_T t = instrs->start[*inst_ptr].t;
  
  // arithmetic operations
  switch(t) {
    case ADV:
      reg->A /= 1 << to_combo(op, reg);
      break;
    case BXL:
      reg->B ^= op;
      break;
    case BST:
      reg->B = to_combo(op, reg) % 8;
      break;
    case JNZ: // the pointer arithmetic is done later
      break;
    case BXC:
      reg->B ^= reg->C;
      break;
    case OUT:
      vec_int_push(out, to_combo(op, reg) % 8);
      break;
    case BDV:
      reg->B = reg->A / (1 << to_combo(op, reg));
      break;
    case CDV:
      reg->C = reg->A / (1 << to_combo(op, reg));
      break;
    default: 
      fprintf(stderr, "Received unknown instruction! (opcode: %d)\n", t);
      return 1; 
  }

  // updating the instruction pointer
  if (t == JNZ && reg->A != 0)
    *inst_ptr = (op / 2);
  else
    *inst_ptr += 1;

  
  return 0;
}

void execute(reg_file* reg, vec_inst* instrs, vec_int* out) {
  size_t inst_ptr = 0;
  int rc;
  while (inst_ptr >= 0 &&
         inst_ptr < instrs->size) {
    rc = process_instr(reg, instrs, &inst_ptr, out, false);
    if (rc) {
      // fprintf(stderr, "process_instr returned nonzero exit code: %d\n", rc);
      return;
    }

  }
  
}


long calc_input_specific(vec_inst *target, long inst_start, long prev) {
  if (inst_start < 0) return prev;
  // reconstruct the original sequence of numbers
  vec_int ops = MK_VEC_WITH_CAP(int, target->size);
  for (size_t i = 0; i < target->size; i++) {
    vec_int_push(&ops, target->start[i].t);
    vec_int_push(&ops, target->start[i].op);
  }
  vec_int out = MK_VEC(int);

  long num = prev, numcpy = 0;
  size_t start = 0;
  reg_file r;

  int target_num = ops.start[inst_start];
  bool found = false;
  num <<= 3;
  start += 3;

  for (int k = 0; k < 8; k++) {
    for (int j = 1; j < 8; j++) {
      numcpy = num;
      out.size = 0;
      numcpy ^= numcpy & (7L << (j^7));
      numcpy |= (long)k << (j^7);
      numcpy &= ~7L;
      numcpy |= j;
      r = (reg_file){numcpy, 0, 0};
      execute(&r, target, &out);
      int last_same = ops.size - inst_start;
      if (out.size == 3) {
      }
      found = out.size == last_same && out.start[inst_start] == target_num;
      found = out.size == last_same && memcmp(
                out.start,
                &ops.start[inst_start],
                last_same * sizeof(int)
              ) == 0;
      if (found) {
        long result = result = calc_input_specific(
          target, 
          inst_start-1, 
          numcpy
        );
        if (result > 0) {
          free(ops.start);
          free(out.start);
          return result;
        }
      }     
    }
  }

  free(ops.start);
  free(out.start);
  return -1;    
}


int day17() {

  FILE* input_f = load_input(17);  
  assert(input_f);


  reg_file reg_og = {0, 0, 0};
  vec_inst instrs = MK_VEC(inst);
  vec_int out = MK_VEC(int);

  
  fscanf(input_f, "Register A: %ld\n", &reg_og.A);
  fscanf(input_f, "Register B: %ld\n", &reg_og.B);
  fscanf(input_f, "Register C: %ld\n", &reg_og.C);

  fscanf(input_f, "Program: ");

  // read instructions
  INST_T inst_t;
  int op;
  char _unused[2];
  int matched;
  while ((matched = fscanf(input_f, "%d,%d%1s", (int *)&inst_t, &op, _unused)) != EOF) {
    if (matched < 2) break;
    if (inst_t > 7) return 1;
    if (op > 7) return 1;
    vec_inst_push(&instrs, (inst){inst_t, op});
  }

  size_t inst_ptr = 0;
  int rc;

  reg_file reg = reg_og;

  // execute
  while (inst_ptr >= 0 &&
         inst_ptr < instrs.size) {
    rc = process_instr(
      &reg, 
      &instrs, 
      &inst_ptr, 
      &out, 
      false
    );
    if (rc) {
      fprintf(stderr, "process_instr returned nonzero exit code: %d\n", rc);
      return 2;
    }
  }
  panswer(1, "The output of the program is: ", -1);
  printf("\t-> ");
  for (size_t i = 0; i < out.size - 1; i++)
    printf("%d,", out.start[i]);
  if (out.size > 0)
    printf("%d\n", out.start[out.size-1]);

  out.size = 0;

  // print
  long answ = calc_input_specific(&instrs, instrs.size * 2 - 1, 0);
  panswer(2, "The required value for register A is: ", answ);

  
  free(instrs.start);
  free(out.start);


  return 0;
}
