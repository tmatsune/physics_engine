#include "rigid_body.h"

// ----------- ALL FUNCTIONS ----------- //

#define UK .06


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
//static void sub_from_vel(rigid_body *self, const float dx, const float dy){ self->vel.x -= dx; self->vel.y -= dy; }
static void change_pos(rigid_body *self, const vec2 vel){ self->pos.x += vel.x, self->pos.y += vel.y; }

static void handle_angle(physics_object *self){
  if(self->body.angle > 359) self->body.angle -= 360;
  if(self->body.angle < 0) self->body.angle += 360;
}

static void move(physics_object *object, vec2 vel){
  change_pos(&object->body, vel);
}

static void vec_translate_world_pos(vec2 *v, float world_x, float world_y){
  v->x = v->x + world_x;
  v->y = v->y + world_y;
}

static vec2 arithmetic_mean(const vec2 pos, const vec2 *points, int N){
  float sum_x = 0;
  float sum_y = 0;
  for(int i = 0; i < N; i++){
    vec2 v = points[i];
    vec_translate_world_pos(&v, pos.x, pos.y);
    sum_x += v.x; sum_y += v.y;
  }
  return (vec2){ sum_x / (float)N, sum_y / (float)N };
} 

// NON-STATIC FUNCS
void apply_force(rigid_body *self, vec2 force){
  self->accel.x += force.x;
  self->accel.y += force.y;
} 
void control_body(physics_object *self, int *lrud, int *wasd){
  vec2 force = {0, 0};
  if (lrud[0]) force.x -= 700; // Arbitrary force value
  if (lrud[1]) force.x += 700;
  if (lrud[2]) force.y -= 700;
  if (lrud[3]) force.y += 700;
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

/* 
static void resolve_collision(rigid_body *body_a, rigid_body *body_b, vec2 normal, float depth){
  float j = 0;
  (void)depth;
  vec2 impulse_a = { (j / body_a->mass) * normal.x, (j / body_a->mass) * normal.y };
  vec2 impulse_b = { (j / body_a->mass) * normal.x, (j / body_a->mass) * normal.y };
  add_to_vel(body_a, impulse_a.x, impulse_a.y);
  sub_from_vel(body_b, impulse_b.x, impulse_b.y);
}
*/


// ------------------- POLYGON COLLISIONS ----------------- // 

static void project_vertices(vec2 pos, vec2 *points, vec2 axis, float *mn, float *mx){
  for(int i = 0; i < 4; i++){
  
    vec2 vertex = points[i];
    vec_translate_world_pos(&vertex, pos.x, pos.y); // get translated points from all 4 vector points on polygon 
    float projection = dot(vertex, axis);           // project curr vertex on axis 
    if(projection < *mn){ *mn = projection; }
    if(projection > *mx){ *mx = projection; }

  }

}

static void resolve_polygon_collision(physics_object *self, physics_object *other, vec2 normal, float depth){
  vec2 relative_vel = vec_sub(other->body.vel, self->body.vel);
  if ( dot(relative_vel, normal) > 0 ){
    return;
  }
  float e = min(self->body.restitution, other->body.restitution);
  float j = -(1 + e) * dot(relative_vel, normal);
  j /= self->body.inv_mass + other->body.inv_mass;

  vec2 impulse = (vec2){normal.x * j, normal.y * j};
  self->body.vel.x -= self->body.inv_mass * impulse.x;    // apply movment to vel for A body 
  self->body.vel.y -= self->body.inv_mass * impulse.y;  

  other->body.vel.x += other->body.inv_mass * impulse.x;
  other->body.vel.y += other->body.inv_mass * impulse.y;
  
  (void)depth;
}

/*  
  systematically checks if there is any axis where the two polygons don’t overlap
  If such an axis is found, they are not colliding. If they overlap on all axes, 
  the polygons are colliding, and the function computes and applies the necessary movement to separate them.
*/
bool polygon_collision(physics_object *self, physics_object *other){
    
  vec2 normal = (vec2){0,0};
  float depth = 1000;
  SDL_SetRenderDrawColor(self->renderer, 255, 0, 0, 255);
  
  for(int i = 0; i < self->len_points; i++){
    vec2 va = self->shape.b.points[i];
    vec2 vb = self->shape.b.points[(i + 1) % self->len_points];
    vec_translate_world_pos(&va, self->body.pos.x, self->body.pos.y);
    vec_translate_world_pos(&vb, self->body.pos.x, self->body.pos.y);

    vec2 edge = vec_sub(vb, va);
    vec2 normal_axis = (vec2){-edge.y, edge.x}; // normal that points away from edge polygon

    //SDL_RenderDrawLine(self->renderer, va.x, va.y, va.x + ref_normal_axis.x, va.y + ref_normal_axis.y);
    float max_a = -1000000;
    float min_a = 1000000;
    float max_b = -1000000;
    float min_b = 1000000;

    project_vertices(self->body.pos, self->shape.b.points, normal_axis, &min_a, &max_a);
    project_vertices(other->body.pos, other->shape.b.points, normal_axis, &min_b, &max_b);

    if( min_a >= max_b || min_b >= max_a ){
      return false;
    }

    float axis_depth = min(max_b - min_a, max_a - min_b);
    if(axis_depth < depth){ 
      depth = axis_depth;
      normal = normal_axis;
    }

  }

  for(int i = 0; i < self->len_points; i++){
    vec2 va = other->shape.b.points[i];
    vec2 vb = other->shape.b.points[(i + 1) % self->len_points];
    vec_translate_world_pos(&va, other->body.pos.x, other->body.pos.y);
    vec_translate_world_pos(&vb, other->body.pos.x, other->body.pos.y);

    vec2 edge = vec_sub(vb, va);
    vec2 normal_axis = (vec2){-edge.y, edge.x}; // normal that points away from edge polygon
    float max_a = -1000000;
    float min_a = 1000000;
    float max_b = -1000000;
    float min_b = 1000000;

    project_vertices(other->body.pos, other->shape.b.points, normal_axis, &min_a, &max_a);
    project_vertices(self->body.pos, self->shape.b.points, normal_axis, &min_b, &max_b);
    // SDL_RenderDrawLine(self->renderer, va.x, va.y, vb.x, vb.y);
    
    if( min_a >= max_b || min_b >= max_a ){
      return false;
    }
    
    float axis_depth = min(max_b - min_a, max_a - min_b);
    if(axis_depth < depth){ 
      depth = axis_depth;
      normal = normal_axis;
    }
    
  }

  depth /= length(normal);
  normal = normalize(normal);
  
  vec2 center_a = arithmetic_mean(self->body.pos, self->shape.b.points, self->len_points);    // get center of curr shape 
  vec2 center_b = arithmetic_mean(other->body.pos, other->shape.b.points, other->len_points); // get center of other shape
  vec2 dir = vec_sub(center_b, center_a);

  vec_scale(dir, 2);
  SDL_RenderDrawLine(self->renderer, center_a.x, center_a.y, center_a.x + dir.x, center_a.y + dir.y);

  if(dot(dir, normal) < 0){ // reverse direction of normal if collision for a is on bottom and a is colliding on left 
    normal.x = -normal.x;
    normal.y = -normal.y;
  }
  
  vec2 movement_a = (vec2){normal.x * (depth/2), normal.y * (depth/2)};   // movement vector by getting the normal and scaling 
  vec2 movement_b = (vec2){-normal.x * (depth/2), -normal.y * (depth/2)};
 
  if(self->body.is_static){
    move(other, movement_a);
  }else if (other->body.is_static){
    move(self, movement_b);
  }else{
    move(other, movement_a);
    move(self, movement_b);
  }
  resolve_polygon_collision(self, other, normal, depth);
  
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
  //draw_rect(self->body.pos.x, self->body.pos.y, self->shape.b.width, self->shape.b.height, self->body.sdl_color, self->body.angle, renderer);
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

// ------- BOX UPDATE
void box_update(physics_object *self, const float dt){
  body_move(&self->body, dt);
  self->body.vel.y += 490 * dt;
  //apply_friction_beta(&self->body);
  handle_angle(self);
}
// ------- STATIC BOX UPDATE 
void static_box_update(physics_object *self, const float dt){
  (void)self;
  (void)dt;
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
  body->inv_mass = 1 / mass;
  body->density = density;
  body->restitution = restitution;
  body->radius = width * height;
  body->sdl_color = color; 
  body->hit_color =  (SDL_Color){ 255, 0, 0, 255}; 
  body->is_static = false;
  shape->type = SHAPE_BOX;
  shape->b.height = height;
  shape->b.width = width;
  shape->b.points[0] = (vec2){-16,-16};
  shape->b.points[1] = (vec2){16,-16};
  shape->b.points[2] = (vec2){16,16};
  shape->b.points[3] = (vec2){-16,16};
}

 
void static_box_shape_init(rigid_body *body, shape_struct *shape, float x, float y, float width, float height, float mass, float density, float restitution, SDL_Color color){
  body->pos = (vec2){x, y};
  body->vel = (vec2){0, 0};
  body->accel = (vec2){0, 0};
  body->angle = 0;
  body->mass = mass;
  body->inv_mass = 0;
  body->density = density;
  body->restitution = restitution;
  body->radius = width * height;
  body->sdl_color = color; 
  body->hit_color =  (SDL_Color){ 255, 0, 0, 255}; 
  body->is_static = true;
  shape->type = SHAPE_STATIC_BOX;
  shape->b.height = height;
  shape->b.width = width;
  shape->b.points[0] = (vec2){-186,-16};
  shape->b.points[1] = (vec2){186,-16};
  shape->b.points[2] = (vec2){186,16};
  shape->b.points[3] = (vec2){-186,16};
}





