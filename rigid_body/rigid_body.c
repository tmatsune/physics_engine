#include "rigid_body.h"


// ----------- ALL FUNCTIONS ----------- //

void apply_force(rigid_body *rb) 

void control_body(rigid_body *rb, int left, int right, int up, int down){
    rb->pos.x += (right - left) * 6;
    rb->pos.y += (down - up) * 6;
}



// ------------ BOX FUNCTIONS --------- // 

void box_init(rigid_body *rb, float x, float y, float width, float height, float mass, float density, float restitution, uint32_t color){
  rb->pos = (vec2){x, y};
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
  draw_rect(rb->pos.x, rb->pos.y, rb->height, rb->width, rb->color, renderer);
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
  rb->mass = mass; 
  rb->density = density;
  rb->restitution = restitution;
  rb->radius = radius;
  rb->area = PI * pow(radius, 2);
  rb->color = color;
  rb->shape = CIRCLE;
}

void circle_render(rigid_body *rb, SDL_Renderer *renderer){
  draw_circle(rb->pos.x, rb->pos.y, rb->radius, rb->color, renderer);
}

void circle_update(rigid_body *rb){
  circle_print_contents(rb);
}

void circle_collision(rigid_body *self, rigid_body *other){
  int dist = get_distance(self->pos.x, self->pos.y, other->pos.x, other->pos.y);
  if(dist < self->radius + other->radius){
    print("collision");
  }else print("");
}





