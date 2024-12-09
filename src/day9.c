#include "helpers.h"
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include "day9.h"

#define T int
#include "vec.def"
#undef T


void print_disk(vec_int* disk) {  
  for (int i = 0; i < disk->size; i++) {
    int val = disk->start[i];
    printf("%c", val >= 0 ? '0' + val : '.'); 
  }
  printf("\n");
}

void free_node(node* n) {
  if (n->next != NULL)
    free_node(n->next);
  free(n);
}

void print_node(node* n) {
  if (n == NULL) {
    printf("NULL\n");
  } else {
    printf("(%lu %lu)->", n->size, n->pos);
    print_node(n->next);
  }

}


int day9() {
  FILE* input_f = load_input(9);
  assert(input_f);

  // we save every block on the disk
  // and for every position which file
  // it belongs to

  // this is of course not optimal, but the 
  // input is not that big and it makes 
  // computation way easier
  vec_int disk = MK_VEC(int);
  vec_int disk2 = MK_VEC(int);

  // this is basically a free list as it would
  // be used in memory allocators (but of course
  // way simpler)
  // free blocks from start to end
  node* fblocks = &(node){NULL, 0, 0};

  // used blocks from end to start
  node* ublocks = &(node){NULL, 0, 0};

  char c;
  int values_read = 0;
  node* cur_fblock = fblocks;
  while((c = getc(input_f)) != EOF) {
    if (c < '0' || c > '9') break;
    int val = c - '0';

    if (values_read % 2 == 0) {
      for (int i = 0; i < val; i++) {
        // this is a file
        vec_int_push(&disk, values_read / 2);
        vec_int_push(&disk2, values_read / 2);
      }     

    } else {
      cur_fblock->next = malloc(sizeof(node));
      cur_fblock = cur_fblock->next;
      *cur_fblock = (node){ NULL, val, disk.size};
      
      for (int i = 0; i < val; i++) {
        // free disk space
        vec_int_push(&disk, -1);
        vec_int_push(&disk2, -1);
      }     
    }
    values_read++;
  }

  fblocks = fblocks->next;

  int cur_val = -1;
  node* cur_ublock = ublocks;
  assert(disk.size != 0);
  for (long i = disk.size - 1; i >= 0; i--) {
    int val = disk.start[i];
    if (val == -1) {
      continue;
    }
    if (val != cur_val) {
      // printf("(%ld) %d --> %d\n", i, cur_val, val);
      // allocate new node
      cur_ublock->next = malloc(sizeof(node));
      cur_ublock = cur_ublock->next;
      *cur_ublock = (node){NULL, 0, 0};
    }
    cur_val = val;
    cur_ublock->size++;
    cur_ublock->pos = i;
  }
  ublocks = ublocks->next;

  // we now compact
  size_t first_free_block = 0;

  for (size_t i = disk.size - 1; i >= 0; i--) {
 
    // find free block
    while(disk.start[first_free_block] != -1) {
      first_free_block++;
      if (first_free_block >= disk.size)
        break;
    }
    if (first_free_block >= disk.size)
      break;
    // there is no way to compact,
    // if there is no space before the block
    if (first_free_block >= i)
      break;

    // move block
    disk.start[first_free_block] = disk.start[i];
    disk.start[i] = -1;
  }

  // calculate checksum
  long checksum = 0;
  for (size_t i = 0; i < disk.size; i++)
    checksum += (disk.start[i] != -1) * i * disk.start[i];


  printf("--> Q1: The checksum is: %ld\n", checksum);

  // print_disk(&disk2);
  // printf("USED: \n");
  // print_node(ublocks);
  // printf("free: \n");
  // print_node(fblocks);

  // compact disk2 
  
  for (node* to_move = ublocks; to_move != NULL; to_move = to_move->next) {
    node* free_block;
    // find free_block block that is large enough
    for (free_block = fblocks; free_block != NULL && free_block->pos < to_move->pos; free_block = free_block->next)
      if (free_block->size >= to_move->size) break;

    if (free_block != NULL && free_block->pos < to_move->pos) {
      int id = disk2.start[to_move->pos];
      // printf("%d: %lu, %lu\n", id, to_move->size, free_block->pos);
      for (int i = 0; i < to_move->size; i++) {
        disk2.start[free_block->pos + i] = id;
        disk2.start[to_move->pos + i] = -1;
      }
      free_block->size -= to_move->size;
      free_block->pos += to_move->size;
      // print_node(fblocks);
      // print_disk(&disk2);
      // printf("Trying to compact %d...\n", id);
      // print_disk(&disk2);
    }

  }

  // calculate checksum for disk 2
  checksum = 0;
  for (long long i = 0; i < disk2.size; i++)
    checksum += (disk2.start[i] != -1) * i * disk2.start[i];

  
  printf("--> Q2: The checksum is: %ld\n", checksum);
  // print_disk(&disk2);
  


  free(disk.start);
  free(disk2.start);
  free_node(fblocks);
  free_node(ublocks);
  
  return 0;
}
