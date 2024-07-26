#ifndef RENDER_H
#define RENDER_H 
#include <stdio.h>
#include <stdint.h>
#include "SDL.h"


void draw_rect(int x, int y, int h, int w, uint32_t color, SDL_Renderer *renderer);
void draw_circle(int x, int y, int radius, uint32_t color, SDL_Renderer *renderer);
void draw_circle_fill(int x, int y, int radius, uint32_t color, SDL_Renderer *renderer);


#endif 
