#include "DynamicText.h"
#include <stdio.h>

DynamicText* DynamicText_Create(const char* fontfilepath, size_t fontSize) {
    DynamicText* dynamicText = (DynamicText*)malloc(sizeof(DynamicText));
    if (dynamicText != NULL) {
        dynamicText->m_surface = NULL;
        dynamicText->m_texture = NULL;
        dynamicText->m_ourFont = NULL;
        dynamicText->s_ttfInitialized = 0;
        
        // Just like when we initialize our graphics subsystem,
        // we need to do the same for our font system.
        if (!dynamicText->s_ttfInitialized && TTF_Init() == -1) {
            printf("Could not initialize SDL2_ttf, error: %s\n", TTF_GetError());
        } else if (!dynamicText->s_ttfInitialized) {
            // printf("SDL2_ttf system ready to go!\n");
            // Initialize TTF system only once, even if
            // we have multiple DynamicText objects.
            dynamicText->s_ttfInitialized = 1;
        }
        
        // Load our font file and set the font size
        dynamicText->m_ourFont = TTF_OpenFont(fontfilepath, fontSize);
        // Confirm that it was loaded
        if (dynamicText->m_ourFont == NULL) {
            printf("Could not load font\n");
            free(dynamicText);
            dynamicText = NULL;
        }
    }
    return dynamicText;
}

void DynamicText_Destroy(DynamicText* dynamicText) {
    if (dynamicText != NULL) {
        // Destroy our textured text
        if (dynamicText->m_texture != NULL) {
            SDL_DestroyTexture(dynamicText->m_texture);
            dynamicText->m_texture = NULL;
        }

        // Close our font subsystem
        if (dynamicText->m_ourFont != NULL) {
            TTF_CloseFont(dynamicText->m_ourFont);
            dynamicText->m_ourFont = NULL;
        }

        free(dynamicText);
    }
}

void DynamicText_DrawText(DynamicText* dynamicText, SDL_Renderer* renderer, const char* text, int x, int y, int w, int h) {
    // Pixels from our text
    dynamicText->m_surface = TTF_RenderText_Solid(dynamicText->m_ourFont, text, (SDL_Color){ 255, 255, 255 });

    // Setup the texture
    dynamicText->m_texture = SDL_CreateTextureFromSurface(renderer, dynamicText->m_surface);
    // Free the surface
    // We are done with it after we have uploaded to
    // the texture
    SDL_FreeSurface(dynamicText->m_surface);

    // Create a rectangle to draw on
    dynamicText->m_rectangle.x = x;
    dynamicText->m_rectangle.y = y;
    dynamicText->m_rectangle.w = w;
    dynamicText->m_rectangle.h = h;
    // Render our text on a rectangle
    SDL_RenderCopy(renderer, dynamicText->m_texture, NULL, &dynamicText->m_rectangle);
}
void DynamicText_Draw(DynamicText* dynamicText, SDL_Renderer* renderer, const char* text, int x, int y) {
  int text_width, text_height;
  TTF_SizeText(dynamicText ->m_ourFont, text, &text_width, &text_height);
  SDL_Surface *surface = TTF_RenderText_Solid(dynamicText->m_ourFont, text, (SDL_Color){255, 255, 255});
  SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, surface);
  SDL_Rect rect = {x, y, text_width, text_height};
  SDL_RenderCopy(renderer, texture, NULL, &rect);
  SDL_FreeSurface(surface);
  SDL_DestroyTexture(texture);
}
void DynamicText_DrawWithAlignment(DynamicText* dynamicText, SDL_Renderer* renderer, const char* text, int x, int y, int w, int alignment) {
  int text_width, text_height;
  TTF_SizeText(dynamicText ->m_ourFont, text, &text_width, &text_height);
  // 计算对齐后的字符串的起始位置
  int start_x, start_y;
  switch (alignment) {
    case 0: // 左对齐
      start_x = x;
      break;
    case 1: // 中对齐
    //   printf("w: %d, text_width: %d\n", w, text_width);
      start_x = x + (w - text_width) / 2;
      break;
    case 2: // 右对齐
      start_x = x + (w - text_width);
      break;
  }

  // 绘制字符串
  SDL_Surface *surface = TTF_RenderText_Solid(dynamicText->m_ourFont, text, (SDL_Color){255, 255, 255});
  SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, surface);
  SDL_Rect rect = {start_x, y, text_width, text_height};
  SDL_RenderCopy(renderer, texture, NULL, &rect);
  SDL_FreeSurface(surface);
  SDL_DestroyTexture(texture);
}

void DynamicText_SetSize(DynamicText* dynamicText, size_t fontSize) {
    TTF_SetFontSize(dynamicText->m_ourFont, fontSize);
}