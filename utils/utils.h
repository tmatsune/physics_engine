#ifndef UTILS_H
#define UTILS_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdint.h>
#include <math.h>
#include "../assert/assert.h"

typedef uint32_t u32;
typedef uint8_t u8;

#define PI 3.1415926

#define RED 0xFFFF0000
#define BLUE 0xFF0000FF
#define GREEN 0xFF00FF00


void print(char *str);
void print_int(int num);

typedef struct {
  float x, y;
} vec2;

#define dot(v1, v2) ({ \
  const vec2 a = (v1), b = (v2); \
  (a.x * b.x) + (a.y * b.y); \
})

#define length(v) ({ const vec2 _v = (v); sqrt(dot(_v, _v)); })

#define normalize(u) ({ \
  const vec2 _u = (u); \
  const float l = length(_u); \
  (vec2) {_u.x / l, _u.y / l}; \
})

#define vec_add(v0, v1)({ \
  const vec2 a = (v0), b = (v1); \
  (vec2) {a.x + b.x, a.y + b.y}; \
})
#define vec_sub(v0, v1)({ \
  const vec2 a = (v0), b = (v1); \
  (vec2) {a.x - b.x, a.y - b.y}; \
})
#define vec_scale(v0, scale)({  \
  const vec2 v = (v0);  \
  (vec2) {v.x * scale, v.y * scale}; \
}) 
#define min(a, b) ({ \
  __typeof__(a) _a = (a), _b = (b); \
  _a < _b ? _a : _b; \
})
#define max(a, b) ({ \
  __typeof__(a) _a = (a), _b = (b); \
  _a > _b ? _a : _b; \
})
#define get_distance(x1, y1, x2, y2)({ \
  const int _x1 = (x1), _y1 = (y1), _x2 = (x2), _y2 = (y2); \
  (int) sqrt( pow( abs(_x1 - _x2), 2 ) + pow( abs(_y1 - _y2), 2 ) );  \
})


typedef struct {
  int count;
  int max_count;
  void **items;
} dynamic_array;

void array_init(dynamic_array *array);
void array_append(dynamic_array *array, const void *item, size_t item_size);
void *array_pop(dynamic_array *array);
void *array_get(dynamic_array *array, size_t index);
void array_free(dynamic_array *array);

void vec_print(const vec2 v);


#endif // !UTILS_H

