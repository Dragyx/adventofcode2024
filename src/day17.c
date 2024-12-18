#include "day17.h"
#include "helpers.h"
#include <assert.h>
#include <stdbool.h>
#include <stdio.h>

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
  long op_comb;
  // print

  // printf("%d, %ld ->", t, op);
  
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
      op_comb = to_combo(op, reg) % 8;
      bool is_op_type = out->size % 2 == 1;
      vec_int_push(out, op_comb);
      size_t i = (out->size - 1) / 2; 
      // try to match the output with the actual instructions
      if (match_output) {
        if (is_op_type) {
          if (i >= instrs->size || instrs->start[i].op != op_comb)
            return 3;
        } else {
          if (i >= instrs->size || instrs->start[i].t != op_comb)
            return 3;
        
        }
      }
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

long simulate(reg_file reg_og, vec_inst* instrs, vec_int* out) {  
  int rc;
  size_t inst_ptr = 0;
  bool found_A_value = false;
  bool log = false;

  reg_file reg, reg_before_sim;

  long i = 0;

  for (reg_og.A = 1; !found_A_value; reg_og.A *= 2) {
    assert(reg_og.A > 0);
    if (i % 10 == 0)
      printf("--> Simualting reg.A = %ld.\n", reg_og.A);

    // log = reg_og.A == 117440;
    if (log)
      printf("--> Simualting reg.A = %ld.\n", reg_og.A);
    found_A_value = true;
    out->size = 0;
    inst_ptr = 0;
    reg = reg_og;
    // execute
    while (inst_ptr >= 0 &&
           inst_ptr < instrs->size) {
      rc = process_instr(&reg, instrs, &inst_ptr, out, true);
      // printf("--> (%d) Reg = {%ld, %ld, %ld}\n", instrs.start[inst_ptr].t, reg.A, reg.B, reg.C);
      if (rc == 3) {
        if (log) printf("Reached rc = 3\n");

        found_A_value = false;
        break;
      }

      if (rc) {
        fprintf(stderr, "process_instr returned nonzero exit code: %d\n", rc);
        return 2;
      }

    }
    if (log) {
      printf("Trying->\n");
      PRINT_INT_VEC(out);
    }
    if (out->size != instrs->size * 2)
      found_A_value = false;
    log = false;
    i++;
  }
  return reg_og.A;
}

long calc_input_specific(vec_inst *target, size_t inst_start, long prev) {
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
      // bool valid_loc = ((numcpy >> (j ^ 7)) & 7) == k ||
                       // (j^7) > start + 3;
      if ((j ^ 7) < 60 ) {
        numcpy &= ~(7L << (j^7));
        numcpy &= (long)k << (j^7);
      }
      numcpy &= ~7;
      numcpy |= j;
      PRINT_INT_VEC((&out));
      printf("%10lb", numcpy);
      r = (reg_file){numcpy, 0, 0};
      execute(&r, target, &out);
      found = out.size == inst_start + 1 && out.start[inst_start] != target_num;
      if (found) {
        printf("->FOUND\n");
        long result = numcpy;
        if (inst_start < target->size) {
          result = calc_input_specific(target, inst_start+1, numcpy);
        } 
        if (result > 0)
          return result;

      } else {
        printf("\n");
      }
    }
  }

  free(ops.start);
  free(out.start);
  return -1;
  /*
  bool num[512] = {0};
  // we have three digits null padding on the left of the number
  // left of this, there will be additional numbers
  size_t num_start = 256, modifiable = num_start - 3;

  for (size_t i = 0; i < ops.size; i++) {
    int target_num = ops.start[i];
    printf("Starting search for %d\n", target_num);
    bool found_valid = false;
    size_t num_new = num_start + 3;
    // search for candidates
    for (int j = 1; j < 8 && !found_valid; j++) {
      // save value of new digits
      num[num_new] = j & 1;
      num[num_new-1] = (j >> 1) & 1;
      num[num_new-2] = (j >> 2) & 1;
      // get the value that would be used to generate the output and try out different ones
      for (int k = 0; k < 8 && !found_valid; k++) {
        size_t num_acc = num_start - (j ^ 7);
        if (
          num_acc > modifiable && 
          !(
            num[num_acc    ] == (k & 1) &&
            num[num_acc - 1] == ((k >> 1) & 1) &&
            num[num_acc - 2] == ((k >> 2) & 1)
          )
        ) continue;
        found_valid = (j ^ k) == target_num;
        if (found_valid) {
          num[num_acc] = k & 1;
          num[num_acc - 1] = (k >> 1) & 1;
          num[num_acc - 2] = (k >> 2) & 1;
        }
      }
      if (found_valid)
        printf("Final value of %d for %d\n", j, target_num);
    }
    if (!found_valid) {
      int i;
      for (i = 0; !num[i]; i++)
        ;
      for (; i < num_start; i++) {
        printf("%1d", num[i]);
      }
      printf("\n");
      printf("found no match for %d\n", target_num);
    }
    num_start = num_new;
    assert(found_valid);
    
  }
  int i;
  for (i = 0; !num[i]; i++)
    ;
  for (; i < num_start; i++) {
    printf("%1d", num[i]);
  }
  printf("\n");
  */
    
  
  free(ops.start);
  return 0;
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
  printf("--> Starting with Reg = {%ld, %ld, %ld}\n", reg_og.A, reg_og.B, reg_og.C);

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

  printf("))) %ld\n", instrs.size);

  reg_file reg = reg_og;

  // execute
  while (inst_ptr >= 0 &&
         inst_ptr < instrs.size) {
    rc = process_instr(&reg, &instrs, &inst_ptr, &out, false);
  // printf("--> (%d) Reg = {%ld, %ld, %ld}\n", instrs.start[inst_ptr].t, reg.A, reg.B, reg.C);
    if (rc) {
      fprintf(stderr, "process_instr returned nonzero exit code: %d\n", rc);
      return 2;
    }
  }


  // print
  if (out.size > 0)
    printf("%d", out.start[0]);
  for (size_t i = 1; i < out.size; i++) {
    printf(",%d", out.start[i]);
  }
  printf("\n");

  out.size = 0;
  // long result = simulate(reg_og, &instrs, &out);

  // panswer(2, "The lowest value of reg A for which the program outputs itself is", result);
  // print
  vec_inst test = MK_VEC(inst);
  vec_inst_push(&test, (inst){ADV, 0});
  long answ = calc_input_specific(&test, 0, 0);
  panswer(2, "The required number is: ", answ);

  
  free(instrs.start);
  free(out.start);


  return 0;
}
