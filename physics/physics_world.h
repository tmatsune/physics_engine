#ifndef PHYSICS_WORLD_H
#define PHYSICS_WORLD_H
#include <stdio.h> 
#include <stdlib.h>
#include "SDL.h"
#include "../utils/utils.h"
#include "../physics/rigid_body.h"

// --------- PHYSICS WORLD ---------- //

typedef struct {
    dynamic_array *objects;
    vec2 gravity;
} physics_world;

void physics_world_init(physics_world *self);

void physics_world_run(physics_world *self, int *lrud, int *wasd, float dt, SDL_Renderer *renderer);

physics_object *physics_world_get_object(physics_world *self, int index);


#endif // !PHYSICS_WORLD_H
