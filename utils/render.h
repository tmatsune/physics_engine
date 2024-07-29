#ifndef RENDER_H
#define RENDER_H 
#include <stdio.h>
#include <stdint.h>
#include "SDL.h"

void draw_rect(int x, int y, int w, int h, SDL_Color color, float angle ,SDL_Renderer *renderer);
void draw_circle(int x, int y, int radius, uint32_t color, SDL_Renderer *renderer);

#endif 
