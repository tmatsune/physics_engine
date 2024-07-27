#include "rigid_body.h"


// ----------- ALL FUNCTIONS ----------- //

#define UK .04
//  STATIC FUNCS
static void apply_friction_beta(rigid_body *self){ // later apply normal force
  vec2 friction = vec_scale(self->vel, -UK);
  self->vel.x += friction.x;
  self->vel.y += friction.y;
}

static void handle_borders_circle(rigid_body *self){
  if(self->pos.x + self->radius > SCREEN_WIDTH || self->pos.x - self->radius < 0){self->vel.x *= -1;}
  if(self->pos.y + self->radius > SCREEN_HEIGHT || self->pos.y - self->radius < 0){self->vel.y *= -1;}
}

static void add_to_pos(rigid_body *self, const float dx, const float dy){ self->pos.x += dx; self->pos.y += dy; }
static void sub_from_pos(rigid_body *self, const float dx, const float dy){ self->pos.x -= dx; self->pos.y -= dy; }
static void add_to_vel(rigid_body *self, const float dx, const float dy){ self->vel.x += dx; self->vel.y += dy; }

// NON-STATIC FUNCS

void apply_force(rigid_body *self, vec2 force){
  self->accel.x += force.x;
  self->accel.y += force.y;
} 
void control_body(rigid_body *self, int left, int right, int up, int down){
  vec2 force = {0, 0};
  if (left) force.x -= 600; // Arbitrary force value
  if (right) force.x += 600;
  if (up) force.y -= 600;
  if (down) force.y += 600;
  apply_force(self, force);
}

void body_move(rigid_body *self, float dt){
  self->vel.x += self->accel.x * dt;
  self->vel.y += self->accel.y * dt;

  // Integrate velocity to update position
  add_to_pos(self, self->vel.x * dt, self->vel.y * dt);

  // Reset acceleration after applying
  self->accel.x = 0;
  self->accel.y = 0;
}

// COLLISIONS
void circle_on_circle_collision(rigid_body *self, rigid_body *other){
  int dist = get_distance(self->pos.x, self->pos.y, other->pos.x, other->pos.y);
  if(dist < self->radius + other->radius){

    vec2 impact = vec_sub(other->pos, self->pos); // vector where two vectors hit each other 
    float overlap = dist - (self->radius + other->radius);

    vec2 dir = vec_copy(impact);
    dir = normalize(dir);
    dir = vec_scale(dir, overlap/2);
    add_to_pos(self, dir.x, dir.y);
    sub_from_pos(other, dir.x, dir.y);

    dist = self->radius + other->radius;
  
    float total_mass = self->mass + other->mass;
    vec2 vel_diff = vec_sub(other->vel, self->vel);

    float numerator = 2 * other->mass * (float)dot(vel_diff, impact);
    float denominator = total_mass * dist * dist;
    vec2 delta_va = vec_copy(impact);
    
    delta_va = vec_scale(delta_va, numerator/denominator);
    add_to_vel(self, delta_va.x, delta_va.y);

    vec2 delta_vb = vec_copy(impact);
    delta_vb = vec_scale(delta_vb, -numerator/denominator);

    add_to_vel(other, delta_vb.x, delta_vb.y);
  }
}

// ------------ BOX FUNCTIONS --------- // 

void box_init(rigid_body *rb, float x, float y, float width, float height, float mass, float density, float restitution, uint32_t color){
  rb->pos = (vec2){x, y};
  rb->vel = (vec2){0, 0};
  rb->accel = (vec2){0, 0};
  rb->mass = mass; 
  rb->density = density;
  rb->restitution = restitution;
  rb->height = height;
  rb->width = width;
  rb->area = height * width;
  rb->color = color;
  rb->shape = BOX;
}

void box_render(rigid_body *rb, SDL_Renderer *renderer){
  draw_rect(rb->pos.x, rb->pos.y, rb->width, rb->height, rb->color, renderer);
} 

void box_update(rigid_body *rb){
  (void)rb;
}


// ----------- CIRCLE FUNCTIONS ----------- // 

static void circle_print_contents(rigid_body *rb){
  (void)rb;
  //printf("X: %f, Y: %f, RADIUS: %f\n", rb->pos.x, rb->pos.y, rb->radius);
}

void circle_init(rigid_body *rb, float x, float y, float mass, float density, float restitution, float radius, uint32_t color){
  rb->pos = (vec2){x, y};
  rb->vel = (vec2){0, 0};
  rb->accel = (vec2){0, 0};
  rb->mass = mass; 
  rb->density = density;
  rb->restitution = restitution;
  rb->radius = radius;
  rb->area = PI * pow(radius, 2);
  rb->color = color;
  rb->shape = CIRCLE;
}

void circle_render(rigid_body *self, SDL_Renderer *renderer){
  draw_circle(self->pos.x, self->pos.y, self->radius, self->color, renderer);
}

void circle_update(rigid_body *self, float dt){
  circle_print_contents(self);
  body_move(self, dt);
  apply_friction_beta(self);
  handle_borders_circle(self);
}





