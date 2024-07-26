#ifndef GAME_H
#define GAME_H

#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <time.h> 
#include "SDL.h"
#include "../utils/utils.h"
#include "../rigid_body/rigid_body.h"
#include "../utils/render.h"

#define SCREEN_WIDTH 640
#define SCREEN_HEIGHT 640
#define CELL_SIZE 32

typedef uint32_t u32;
typedef uint8_t u8;

typedef struct {
  int w, a, s, d, left, right, up , down;
} key_state;

typedef struct {
  int screen_width;
  int screen_height;
  int cell_size;
  key_state *keys;
  bool quit;
  SDL_Window *window;
  SDL_Renderer *renderer;
  dynamic_array *circles; 
  rigid_body *main_body;
} game;

void game_init(game *g);
void game_update(game *g);
void game_render(game *g);
void game_input(game *g);
void game_run(game *g);



#endif // !GAME_H

