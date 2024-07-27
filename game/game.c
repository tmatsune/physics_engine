#include "game.h"
#include "../assert/assert.h"

static void keys_init(key_state *keys){
  keys->w = keys->a = keys->s = keys->d = 0;
  keys->left = keys->right = keys->up = keys->down = 0;
}   
static void create_circles(game *g){
  
  srand(time(NULL));

  for(int i = 0; i < 0; i++){
    int min = 10;
    int max = 520;
    int x = rand() % (max - min + 1) + min;
    int y = rand() % (max - min + 1) + min;
    int radius = rand() % (20 - 10 + 1) + 10;
    
    rigid_body *body = malloc(sizeof(rigid_body));
    if (body == NULL) {
        perror("Failed to allocate memory for body");
        exit(EXIT_FAILURE);
    }

    circle_init(body, x, y, 1, 1, 1, radius, BLUE);   

    array_append(g->circles, body, sizeof(rigid_body));

    if(i == 0) 
      g->main_body = body; 
  
  }

}

static void create_boxes(game *g){
  srand(time(NULL));

  for(int i = 0; i < 0; i++){
    int min = 10;
    int max = 520;
    int x = rand() % (max - min + 1) + min;
    int y = rand() % (max - min + 1) + min;
    int w = 20, h = 20;
    
    rigid_body *body = malloc(sizeof(rigid_body));
    ASSERT(body == NULL, "FAILED TO ALLOCATE MEMORY FOR BODY");
    box_init(body, x, y, w, h, 1, 1, 1, GREEN);
    array_append(g->boxes, body, sizeof(rigid_body));

    if(i == 0) g->main_body = body;

  }

}

static void test_init(game *g){
  create_circles(g);
  create_boxes(g);
}

void game_init(game *g){

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

  g->circles = malloc(sizeof(dynamic_array));
  array_init(g->circles);

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




  /*
  for(int i = 0; i < g->circles->count; i++){
    rigid_body *circle = (rigid_body*)array_get(g->circles, i);
    circle_render(circle, g->renderer);
    circle_update(circle, g->dt);
    if(i == 0){
      control_body(circle, g->keys->left, g->keys->right, g->keys->up, g->keys->down); 
      rigid_body *other = (rigid_body*) array_get(g->circles, 1);
      circle_on_circle_collision(circle, other);
    }
  }
  */




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

  SDL_DestroyRenderer(g->renderer);
  SDL_DestroyWindow(g->window);

  SDL_Quit();
}


    
