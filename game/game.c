#include "game.h"
#include "../assert/assert.h"

static void keys_init(key_state *keys){
  keys->w = keys->a = keys->s = keys->d = 0;
  keys->left = keys->right = keys->up = keys->down = 0;
}   

static void gen_objects(game *g){

  for(int i = 0; i < 2; i++){
    int min = 10;
    int max = 520;
    int x = rand() % (max - min + 1) + min;
    int y = rand() % (max - min + 1) + min;
    int w = 20, h = 20;
    physics_object *object = malloc(sizeof(physics_object));
    if(object == NULL){
      perror("Failed to allocate memory for body");
      exit(EXIT_FAILURE); 
    }
    shape_type shape_t = SHAPE_BOX;
    switch(shape_t){
      case SHAPE_BOX: {
        box_shape_init(&object->body, &object->shape, x, y, w, h, 1, 1, 1, (SDL_Color){ 255, 255, 255, 255 });
        object->renderer = g->renderer;
        object->len_points = 4;
        break;
      }
      case SHAPE_CIRCLE: {
        break;
      }
      default:{break;}
    };
    array_append(g->objects, object, sizeof(physics_object));
  
  }
}

static void test_init(game *g){
  gen_objects(g);
}

void game_init(game *g){
  srand(time(NULL));

  key_state *keys = (key_state *)malloc(sizeof(key_state));
  if (keys == NULL) {
      perror("Failed to allocate memory for key_state");
      exit(EXIT_FAILURE);
  }
  keys_init(keys);
  g->screen_width = SCREEN_WIDTH; 
  g->screen_height = SCREEN_HEIGHT;
  g->cell_size = CELL_SIZE;
  g->quit = false;
  g->keys = keys;
  g->dt = 0;

    ASSERT(
        SDL_Init(SDL_INIT_VIDEO),
        "SDL failed to initialize: %s\n",
        SDL_GetError());

    g->window =
        SDL_CreateWindow(
            "PHYSICS ENGINE",
            SDL_WINDOWPOS_CENTERED,
            SDL_WINDOWPOS_CENTERED,
            SCREEN_WIDTH,
            SCREEN_HEIGHT,
            SDL_WINDOW_SHOWN);
    ASSERT(
        g->window == NULL,
        "failed to create SDL window: %s\n", SDL_GetError());

    g->renderer =
        SDL_CreateRenderer(g->window, -1, SDL_RENDERER_PRESENTVSYNC);
    ASSERT(
        g->renderer == NULL,
        "failed to create SDL renderer: %s\n", SDL_GetError());
  
    g->objects = malloc(sizeof(dynamic_array));
    ASSERT(g->objects == NULL, "Failed to allocate memory for all objs");
    array_init(g->objects);

    test_init(g);

}


void game_update(game *g){
  SDL_Event ev;
  while (SDL_PollEvent(&ev)) {
      switch (ev.type) {
        case SDL_QUIT:
          g->quit = true;
          break;
      } 
  }
}

void game_render(game *g){
  SDL_SetRenderDrawColor(g->renderer, 1, 0, 0, 255); // Black
  SDL_RenderClear(g->renderer);

  // ----- RENDER ----- // 

  for(int i = 0; i < (int)g->objects->count; i++){

    physics_object *obj = (physics_object*)array_get(g->objects, i);
    switch(obj->shape.type){
      case SHAPE_BOX:{
        box_render(obj, g->renderer);
        box_update(obj, g->dt);
        break;
      }
      case SHAPE_CIRCLE:{
       
        break;
      }
    }
    if(i == 0){
      int lrud[] = {g->keys->left, g->keys->right, g->keys->up, g->keys->down};
      int wasd[] = {g->keys->w, g->keys->a, g->keys->s, g->keys->d};
      control_body(obj, lrud, wasd);
      physics_object* other = (physics_object*)array_get(g->objects, 1);
      bool colliding = polygon_collision(obj, other); 
      if(colliding) print("COLLIDING");
      else print("NOT COLLIDING");
    }
 
  }


   // ------------------ // 

  SDL_RenderPresent(g->renderer);
}

void game_input(game *g){
  
  const u8 *keystate = SDL_GetKeyboardState(NULL);
 
  if (keystate[SDL_SCANCODE_ESCAPE]){
    g->quit = true;
  } 

  if (keystate[SDL_SCANCODE_RIGHT]){
    g->keys->right = 1;
  }else{
    g->keys->right = 0;
  }

  if (keystate[SDL_SCANCODE_LEFT]){
    g->keys->left = 1;
  }else{
    g->keys->left = 0;
  }

  if (keystate[SDL_SCANCODE_UP]){
    g->keys->up = 1;
  }else{
    g->keys->up = 0;
  }

  if (keystate[SDL_SCANCODE_DOWN]){
    g->keys->down= 1;
  }else{
    g->keys->down = 0;
  }

  if (keystate[SDL_SCANCODE_W])  g->keys->w = 1;
  else g->keys->w = 0;
  if (keystate[SDL_SCANCODE_A])  g->keys->a = 1;
  else g->keys->a = 0;
  if (keystate[SDL_SCANCODE_S]) g->keys->s = 1;
  else g->keys->s = 0;
  if (keystate[SDL_SCANCODE_D]) g->keys->d = 1;
  else g->keys->d = 0;

}

void game_run(game *g){
  
  uint32_t last_time = SDL_GetTicks();
  
  uint32_t frame_count __attribute__((unused)) = 0;
  uint32_t fps_time __attribute__((unused)) = SDL_GetTicks();
  float fps __attribute__((unused))= 0.0f;

  // ------ MAIN LOOP ------ // 
  while(!g->quit){
    uint32_t current_time = SDL_GetTicks();
    float dt = (current_time - last_time) / 1000.0f; // Convert to seconds
    last_time = current_time;
  
    game_update(g);
    game_render(g);
    game_input(g);

    g->dt = dt;

    /* CHECK FPS
    frame_count++;
    if (current_time - fps_time >= 1000) {
        fps = frame_count / ((current_time - fps_time) / 1000.0f);
        printf("FPS: %.2f\n", fps);
        frame_count = 0;
        fps_time = current_time;
    }
    */

    SDL_Delay(16);
  } 
  // ---------------------- // 
  array_free(g->objects);
  free(g->objects);

  free(g->keys);


  SDL_DestroyRenderer(g->renderer);
  SDL_DestroyWindow(g->window);

  SDL_Quit();
}


    
