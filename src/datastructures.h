#ifndef DATASTRUCTURES_H_
#define DATASTRUCTURES_H_

#include <stdlib.h>

#define DEF_VEC(T) typedef struct { \
   T* start;                                 \
   size_t size, capacity;                    \
} vec_ ## T; 

#define MK_VEC_WITH_CAP(T, CAP) { .start = malloc(CAP * sizeof(T)), .size = 0, .capacity = CAP }
#define MK_VEC(T) MK_VEC_WITH_CAP(T, 1)
#define DEF_VEC_PUSH(T) int vec_## T ## _push(vec_## T* vec, T el) { \
   if (vec->size >= vec->capacity) {     \
      vec->capacity <<= 1;               \
      vec->start = (T *)realloc(         \
         vec->start,                     \
         sizeof(T) * vec->capacity       \
      );                                 \
      if(vec->start == NULL) return 1;   \
   }                                     \
   vec->start[vec->size++] = el;         \
   return 0;                             \
}

#define DEF_VEC_POP(T) int vec_## T ## _pop(vec_## T* vec, T* value) { \
   if (vec->size == 0) return 1;         \
   *value = vec->start[vec->size--];     \
   if (vec->size <= vec->capacity / 2) { \
      vec->capacity >>= 1;               \
      vec->start = (T *)realloc(         \
         vec->start,                     \
         vec->capacity * sizeof(T)       \
      );                                 \
      if (vec->start == NULL)            \
         return 1;                       \
   }                                     \
   return 0;                             \
}

#define DEC_VEC_FUNCS(T) \
int vec_## T ## _push(vec_## T* vec, T el);  \
int vec_## T ## _pop(vec_## T* vec, T* value);

DEF_VEC(int)
DEC_VEC_FUNCS(int);


DEF_VEC(vec_int)
DEC_VEC_FUNCS(vec_int)


#endif
