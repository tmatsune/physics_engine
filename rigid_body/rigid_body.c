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

static void handle_angle(physics_object *self){
  if(self->body.angle > 359) self->body.angle -= 360;
  if(self->body.angle < 0) self->body.angle += 360;

}

// NON-STATIC FUNCS
void apply_force(rigid_body *self, vec2 force){
  self->accel.x += force.x;
  self->accel.y += force.y;
} 
void control_body(physics_object *self, int *lrud, int *wasd){
  vec2 force = {0, 0};
  if (lrud[0]) force.x -= 600; // Arbitrary force value
  if (lrud[1]) force.x += 600;
  if (lrud[2]) force.y -= 600;
  if (lrud[3]) force.y += 600;
  apply_force(&self->body, force);
  float ang = 0;
  if(wasd[0]){};
  if(wasd[1]){self->body.angle -= 6; ang = -6;}
  if(wasd[2]){};
  if(wasd[3]){self->body.angle += 6; ang = 6;}
  
  for(int i = 0; i < self->len_points; i++){
    vec_rotate(&self->shape.b.points[i], rad(ang)); 
  }

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

void change_property(rigid_body *self, shape_property prop, float new_num){
  switch(prop){
    case PROP_HEIGHT: {
      self->height = new_num;
      break;
    } 
     case PROP_WIDTH: {
      self->width = new_num;
      break;
    } 
     case PROP_X: {
      self->pos.x = new_num;
      break;
    } 
     case PROP_Y: {
      self->pos.y = new_num;
      break;
    } 
     case PROP_COLOR: {
      self->color = new_num;
      break;
    } 
     case PROP_ANGLE: {
      self->angle = new_num;
      break;
    } 
     case PROP_MASS: {
      self->mass = new_num;
      break;
    } 
    default: {
      break;
    } 
  }
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

static void vec_translate_world_pos(vec2 *v, float world_x, float world_y){
  v->x = v->x + world_x;
  v->y = v->y + world_y;
}

static void project_vertices(vec2 pos, vec2 *points, vec2 axis, float *mn, float *mx){
  for(int i = 0; i < 4; i++){
  
    vec2 vertex = points[i];
    vec_translate_world_pos(&vertex, pos.x, pos.y);
    float projection = dot(vertex, axis);
    if(projection < *mn){ *mn = projection; }
    if(projection > *mx){ *mx = projection; }

  }

}

bool polygon_collision(physics_object *self, physics_object *other){
  
  SDL_SetRenderDrawColor(self->renderer, 255, 0, 0, 255);
  
  for(int i = 0; i < self->len_points; i++){
    vec2 va = self->shape.b.points[i];
    vec2 vb = self->shape.b.points[(i + 1) % self->len_points];
    vec_translate_world_pos(&va, self->body.pos.x, self->body.pos.y);
    vec_translate_world_pos(&vb, self->body.pos.x, self->body.pos.y);

    vec2 edge = vec_sub(vb, va);
    vec2 normal_axis = (vec2){-edge.y, edge.x}; // normal that points away from edge polygon

    //SDL_RenderDrawLine(self->renderer, va.x, va.y, va.x + ref_normal_axis.x, va.y + ref_normal_axis.y);
    float max_a = -10000;
    float min_a = 10000;
    float max_b = -10000;
    float min_b = 10000;

    project_vertices(self->body.pos, self->shape.b.points, normal_axis, &min_a, &max_a);
    project_vertices(other->body.pos, other->shape.b.points, normal_axis, &min_b, &max_b);


    if( min_a >= max_b || min_b >= max_a ){
      return false;
    }
    
  }

  for(int i = 0; i < self->len_points; i++){
    vec2 va = other->shape.b.points[i];
    vec2 vb = other->shape.b.points[(i + 1) % self->len_points];
    vec_translate_world_pos(&va, other->body.pos.x, other->body.pos.y);
    vec_translate_world_pos(&vb, other->body.pos.x, other->body.pos.y);

    vec2 edge = vec_sub(vb, va);
    vec2 normal_axis = (vec2){-edge.y, edge.x}; // normal that points away from edge polygon
    float max_a = -10000;
    float min_a = 10000;
    float max_b = -10000;
    float min_b = 10000;

    project_vertices(other->body.pos, other->shape.b.points, normal_axis, &min_a, &max_a);
    project_vertices(self->body.pos, self->shape.b.points, normal_axis, &min_b, &max_b);
    // SDL_RenderDrawLine(self->renderer, va.x, va.y, vb.x, vb.y);

    if( min_a >= max_b || min_b >= max_a ){
      return false;
    }
    
  }
  return true;
}

// ------------ BOX FUNCTIONS --------- // 

void box_init(rigid_body *self, float x, float y, float width, float height, float mass, float density, float restitution, uint32_t color){
  self->pos = (vec2){x, y};
  self->vel = (vec2){0, 0};
  self->accel = (vec2){0, 0};
  self->angle = 0;
  self->mass = mass; 
  self->density = density;
  self->restitution = restitution;
  self->height = height;
  self->width = width;
  self->area = height * width;
  self->color = color;
  self->shape = SHAPE_BOX;
}

void box_render(physics_object *self, SDL_Renderer *renderer){
  //draw_rect(self->body.pos.x, self->body.pos.y, self->shape.b.width, self->shape.b.height, self->body.color, self->body.angle, renderer);
  SDL_SetRenderDrawColor(self->renderer, 255, 0, 0, 255);
  for(int i = 0; i < 4; i++){
    draw_rect(self->body.pos.x + (self->shape.b.points[i].x) , self->body.pos.y + (self->shape.b.points[i].y), 2, 2, self->body.sdl_color, 0, renderer);
    vec2 va = self->shape.b.points[i];
    vec2 vb = self->shape.b.points[(i + 1) % self->len_points];
    vec_translate_world_pos(&va, self->body.pos.x, self->body.pos.y);
    vec_translate_world_pos(&vb, self->body.pos.x, self->body.pos.y);
    SDL_RenderDrawLine(self->renderer, va.x, va.y, vb.x, vb.y);
  }
}   

void box_update(physics_object *self, float dt){
  body_move(&self->body, dt);
  apply_friction_beta(&self->body);
  handle_angle(self);
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
  rb->angle = 0;
  rb->mass = mass; 
  rb->density = density;
  rb->restitution = restitution;
  rb->radius = radius;
  rb->area = PI * pow(radius, 2);
  rb->color = color;
  rb->shape = SHAPE_CIRCLE;
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


// --------------- PHYSICS OBJECT FUNCS ---------------- // 

void box_shape_init(rigid_body *body, shape_struct *shape, float x, \
                           float y, float width, float height, float mass, \
                           float density, float restitution, SDL_Color color ){
  body->pos = (vec2){x, y};
  body->vel = (vec2){0, 0};
  body->accel = (vec2){0, 0};
  body->angle = 0;
  body->mass = mass;
  body->density = density;
  body->restitution = restitution;
  body->radius = width * height;
  body->sdl_color = color; 
  body->hit_color =  (SDL_Color){ 255, 0, 0, 255}; 
  shape->type = SHAPE_BOX;
  shape->b.height = height;
  shape->b.width = width;
  shape->b.points[0] = (vec2){-6,-6};
  shape->b.points[1] = (vec2){6,-6};
  shape->b.points[2] = (vec2){6,6};
  shape->b.points[3] = (vec2){-6,6};
}


