#include "render.h"

// --- STATIC FUNCS --- // 
static SDL_Texture* create_filled_rect_texture(SDL_Renderer *renderer, int w, int h, SDL_Color color) {
    SDL_Texture *texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, w, h);
    if (texture == NULL) {
        printf("SDL_CreateTexture Error: %s\n", SDL_GetError());
        return NULL;
    }

    SDL_SetRenderTarget(renderer, texture);
    SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);
    SDL_RenderClear(renderer);
    SDL_SetRenderTarget(renderer, NULL);

    return texture;
}

void draw_rect(int x, int y, int w, int h, uint32_t color, float angle, SDL_Renderer *renderer){
  (void)color;
  SDL_Color rect_color = { 255, 0, 0, 255 };
  SDL_Texture *texture = create_filled_rect_texture(renderer, w, h, rect_color);
  SDL_Rect dstrect = { x, y, w, h };
  SDL_Point center = { w / 2, h / 2 };
  SDL_RenderCopyEx(renderer, texture, NULL, &dstrect, angle, &center, SDL_FLIP_NONE);
}

void draw_circle(int x, int y, int radius, uint32_t color, SDL_Renderer *renderer){
    Uint8 r = (color >> 24) & 0xFF;
    Uint8 g = (color >> 16) & 0xFF;
    Uint8 b = (color >> 8) & 0xFF;
    Uint8 a = color & 0xFF;


    SDL_SetRenderDrawColor(renderer, r, g, b, a);

    int offsetX = radius;
    int offsetY = 0;
    int err = 1 - radius;

    while (offsetX >= offsetY) {
        SDL_RenderDrawPoint(renderer, x + offsetX, y + offsetY);
        SDL_RenderDrawPoint(renderer, x + offsetY, y + offsetX);
        SDL_RenderDrawPoint(renderer, x - offsetY, y + offsetX);
        SDL_RenderDrawPoint(renderer, x - offsetX, y + offsetY);
        SDL_RenderDrawPoint(renderer, x - offsetX, y - offsetY);
        SDL_RenderDrawPoint(renderer, x - offsetY, y - offsetX);
        SDL_RenderDrawPoint(renderer, x + offsetY, y - offsetX);
        SDL_RenderDrawPoint(renderer, x + offsetX, y - offsetY);

        offsetY++;
        if (err <= 0) {
            err += 2 * offsetY + 1;
        }

        if (err > 0) {
            offsetX -= 1;
            err -= 2 * offsetX + 1;
        }
    }
}


