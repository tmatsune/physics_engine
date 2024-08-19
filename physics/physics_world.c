#include "physics_world.h"


void physics_world_init(physics_world *self){
  self->objects = malloc(sizeof(dynamic_array));
  if(self->objects == NULL){
    perror("ERROR REALLOC MEMORY, APPENDING");
    exit(EXIT_FAILURE);
  }
  array_init(self->objects);

}


physics_object *physics_world_get_object(physics_world *self, int index){
  return (physics_object*)array_get(self->objects, index);
}


void physics_world_run(physics_world *self, int *lrud, int *wasd, float dt, SDL_Renderer *renderer){
 
  for(int i = 0; i < (int)self->objects->count; i++){
    
    
    physics_object *obj = physics_world_get_object(self, i);
    switch(obj->shape.type){
      case SHAPE_BOX:{
        box_render(obj, renderer);
        box_update(obj, dt);
        break;
      }
      case SHAPE_STATIC_BOX: {
        box_render(obj, renderer);
        static_box_update(obj, dt);
        break;

      }
      case SHAPE_CIRCLE:{
        break;
      }
    }
  
    if(i == 0){
      //int lrud[] = {g->keys->left, g->keys->right, g->keys->up, g->keys->down};
      //int wasd[] = {g->keys->w, g->keys->a, g->keys->s, g->keys->d};
      control_body(obj, lrud, wasd);
      physics_object *other = physics_world_get_object(self, 1);
      physics_object *static_other = physics_world_get_object(self, 2);
      switch(other->shape.type){
        case SHAPE_BOX: {
          bool colliding __attribute__((unused)) = polygon_collision(obj, other);
          break;
        }
        case SHAPE_STATIC_BOX: {
          bool colliding __attribute__((unused)) = polygon_collision(obj, other);
          printf("%d\n", colliding);
          break;
        }
        case SHAPE_CIRCLE: {
          break;
        }
      }
      switch(static_other->shape.type){
        case SHAPE_BOX: {
          bool colliding __attribute__((unused)) = polygon_collision(obj, other);
          break;
        }
        case SHAPE_STATIC_BOX: {
          bool colliding __attribute__((unused)) = polygon_collision(obj, static_other);
          break;
        }
        case SHAPE_CIRCLE: {
          break;
        }
      }

    }
  
  
  }
  

}


