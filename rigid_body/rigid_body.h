#ifndef RIGID_BODY_H
#define RIGID_BODY_H
#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <math.h>
#include "../utils/utils.h"
#include "../utils/render.h"

typedef enum {
  BOX, CIRCLE,
} shape_type;

typedef struct {
  vec2 pos;
  vec2 vel;
  vec2 accel;
  float mass;
  float angle;
  float density;
  float restitution;
  float area;

  float height;
  float width;

  float radius;
  shape_type shape; 
  uint32_t color;

} rigid_body;
  
// ----------- ALL FUNCTIONS ----------- // 
void control_body(rigid_body *self, int left, int right, int up, int down);

void apply_force(rigid_body *self, vec2 force);

void body_move(rigid_body *self, float dt);

// COLLISION 
void circle_on_circle_collision(rigid_body *self, rigid_body *other);

// ------------ BOX FUNCTIONS --------- // 
void box_init(rigid_body *rb, float x, float y, float width, float height, float mass, float density, float restitution, uint32_t color);

void box_render(rigid_body *rb, SDL_Renderer *renderer);

void box_update(rigid_body *rb);


// ----------- CIRCLE FUNCTIONS ----------- // 
void circle_init(rigid_body *rb, float x, float y, float mass, float density, float restitution, float radius, uint32_t color);

void circle_render(rigid_body *self, SDL_Renderer *renderer);

void circle_update(rigid_body *self, float dt);

#endif // !RIGID_BODY_H
