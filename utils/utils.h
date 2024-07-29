#ifndef UTILS_H
#define UTILS_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdint.h>
#include <math.h>
#include "SDL.h"

/*
 640 meters by 640 meters 
 seconds 
*/ 
typedef uint32_t u32;
typedef uint8_t u8;

#define PI 3.1415926
#define RED 0xFFFF0000
#define BLUE 0xFF0000FF
#define GREEN 0xFF00FF00
#define SCREEN_WIDTH 640
#define SCREEN_HEIGHT 640
#define CELL_SIZE 32


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
#define vec_copy(v)({ \
  const vec2 a = (v);  \
  (vec2) {a.x, a.y}; \
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
#define vec_new_rotate(vec, theta) ({ \
  const vec2 v = (vec); \
  (vec2) {(v.x*cos(theta)) - (v.y*sin(theta)), (v.x*sin(theta))+(v.y*cos(theta)}; \
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
#define rad(ang) ({  \
  __typeof__(ang) _ang = (ang); \
  (float) _ang * (PI / 180.0); \
})
void vec_rotate(vec2 *v, float theta);

// DYNAMIC ARRAY 
typedef struct {
  void **items;
  size_t count;
  size_t max_count;
} dynamic_array;

void array_init(dynamic_array *array);

void array_append(dynamic_array *array, const void *item, size_t item_size);

void *array_pop(dynamic_array *array);

void *array_get(dynamic_array *array, size_t index);

void array_free(dynamic_array *array);

// PRINT 
void print(char *str);
void print_int(int num);
void vec_print(const vec2 v);
void print_float(float num);
void endl(void);


#endif // !UTILS_H
