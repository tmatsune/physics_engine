#ifndef RIGID_BODY_H
#define RIGID_BODY_H
#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <math.h>
#include "../utils/utils.h"
#include "../utils/render.h"

typedef enum {
  SHAPE_BOX, SHAPE_CIRCLE,
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

typedef struct {
  float radius;
} circle;

typedef struct {
  float width, height;
  vec2 points[4];
} box; 

typedef struct {
  shape_type type;
  union {
    circle c;
    box b;
  };
} shape_struct;

typedef struct {
  rigid_body body;
  shape_struct shape;
} physics_object;

typedef enum {
  PROP_WIDTH, PROP_HEIGHT, PROP_X, PROP_Y, PROP_COLOR, PROP_ANGLE, PROP_MASS,
} shape_property;

// ----------- ALL FUNCTIONS ----------- // 
void control_body(physics_object *self, int *lrud, int *wasd);

void apply_force(rigid_body *self, vec2 force);

void body_move(rigid_body *self, float dt);

void change_property(rigid_body *self,shape_property prop, float new_num);

// COLLISION 
void circle_on_circle_collision(rigid_body *self, rigid_body *other);

// ------------ BOX FUNCTIONS --------- // 
void box_init(rigid_body *self, float x, float y, float width, float height, float mass, float density, float restitution, uint32_t color);

void box_render(physics_object *self, SDL_Renderer *renderer);

void box_update(physics_object *self, float dt);


// ----------- CIRCLE FUNCTIONS ----------- // 
void circle_init(rigid_body *rb, float x, float y, float mass, float density, float restitution, float radius, uint32_t color);

void circle_render(rigid_body *self, SDL_Renderer *renderer);

void circle_update(rigid_body *self, float dt);

// --------------- PHYSICS OBJECT FUNCTIONS ---------- // 

void box_shape_init(rigid_body *body, shape_struct *shape, float x, \
                           float y, float width, float height, float mass, \
                           float density, float restitution, uint32_t color);

#endif // !RIGID_BODY_H
