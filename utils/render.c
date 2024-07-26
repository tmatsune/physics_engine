#include "render.h"

void draw_rect(int x, int y, int h, int w, uint32_t color, SDL_Renderer *renderer){
  SDL_Rect rect;
  rect.x = x;
  rect.y = y;
  rect.w = w;
  rect.h = h;
  SDL_SetRenderDrawColor(
    renderer,
    (color >> 16) & 0xFF,
    (color >> 8) & 0xFF, 
    color & 0xFF,
    (color >> 24) & 0xFF);
  SDL_RenderFillRect(renderer, &rect);
}

void draw_circle(int x, int y, int radius, uint32_t color, SDL_Renderer *renderer){
    Uint8 r = (color >> 24) & 0xFF;
    Uint8 g = (color >> 16) & 0xFF;
    Uint8 b = (color >> 8) & 0xFF;
    Uint8 a = color & 0xFF;


    SDL_SetRenderDrawColor(renderer, r, g, b, a);

    int offsetX = radius;
    int offsetY = 0;
    int err = 0;

    while (offsetX >= offsetY) {
        SDL_RenderDrawPoint(renderer, x + offsetX, y + offsetY);
        SDL_RenderDrawPoint(renderer, x + offsetY, y + offsetX);
        SDL_RenderDrawPoint(renderer, x - offsetY, y + offsetX);
        SDL_RenderDrawPoint(renderer, x - offsetX, y + offsetY);
        SDL_RenderDrawPoint(renderer, x - offsetX, y - offsetY);
        SDL_RenderDrawPoint(renderer, x - offsetY, y - offsetX);
        SDL_RenderDrawPoint(renderer, x + offsetY, y - offsetX);
        SDL_RenderDrawPoint(renderer, x + offsetX, y - offsetY);

        offsetY += 1;
        if (err <= 0) {
            err += 2 * offsetY + 1;
        }

        if (err > 0) {
            offsetX -= 1;
            err -= 2 * offsetX + 1;
        }
    }
}


void draw_circle_fill(int x, int y, int radius, uint32_t color, SDL_Renderer *renderer){
    // Extract the RGBA components from the color
    Uint8 r = (color >> 24) & 0xFF;
    Uint8 g = (color >> 16) & 0xFF;
    Uint8 b = (color >> 8) & 0xFF;
    Uint8 a = color & 0xFF;

    // Set the draw color
    SDL_SetRenderDrawColor(renderer, r, g, b, a);

    int offsetX = radius;
    int offsetY = 0;
    int err = 0;

    while (offsetX >= offsetY) {
        for (int i = -offsetX; i <= offsetX; ++i) {
            SDL_RenderDrawPoint(renderer, x + i, y + offsetY);
            SDL_RenderDrawPoint(renderer, x + i, y - offsetY);
        }
        for (int i = -offsetY; i <= offsetY; ++i) {
            SDL_RenderDrawPoint(renderer, x + i, y + offsetX);
            SDL_RenderDrawPoint(renderer, x + i, y - offsetX);
        }

        offsetY += 1;
        if (err <= 0) {
            err += 2 * offsetY + 1;
        }

        if (err > 0) {
            offsetX -= 1;
            err -= 2 * offsetX + 1;
        }
    }
}






