#ifndef DYNAMICTEXT_H
#define DYNAMICTEXT_H

#include <SDL2/SDL.h> // For Mac, use <SDL.h>
#include <SDL2/SDL_ttf.h>

typedef struct {
    SDL_Rect m_rectangle;
    SDL_Texture* m_texture;
    SDL_Surface* m_surface;
    TTF_Font* m_ourFont;
    int s_ttfInitialized;
} DynamicText;

// Constructor
DynamicText* DynamicText_Create(const char* fontfilepath, size_t fontSize);

// Destructor
void DynamicText_Destroy(DynamicText* dynamicText);

void DynamicText_SetSize(DynamicText* dynamicText, size_t fontSize);
// Function for drawing text
void DynamicText_DrawText(DynamicText* dynamicText, SDL_Renderer* renderer, const char* text, int x, int y, int w, int h);
void DynamicText_Draw(DynamicText* dynamicText, SDL_Renderer* renderer, const char* text, int x, int y);
void DynamicText_DrawWithAlignment(DynamicText* dynamicText, SDL_Renderer* renderer, const char* text, int x, int y, int w, int alignment);

#endif