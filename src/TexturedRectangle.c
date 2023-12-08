#include <stdlib.h>
#include <stdio.h>
#include "TexturedRectangle.h"
#include "ResourceManager.h"

TexturedRectangle* TexturedRectangle_Create(SDL_Renderer* renderer, const char* filepath, SDL_Rect* srcRect) {
    return TexturedRectangle_CreateWithColorKey(renderer, filepath, srcRect, 0x00, 0x00, 0x00);
}

TexturedRectangle* TexturedRectangle_CreateWithColorKey(SDL_Renderer* renderer, const char* filepath, SDL_Rect* srcRect, int redColorKey, int greenColorKey, int blueColorKey) {
    TexturedRectangle* rectangle = (TexturedRectangle*)malloc(sizeof(TexturedRectangle));
    if (rectangle == NULL) {
        printf("Failed to allocate memory for TexturedRectangle.\n");
        return NULL;
    }
    rectangle->m_renderer = renderer;
    // SDL_Rect* rect = malloc(sizeof(SDL_Rect));
    // memcpy(rect, &srcRect, sizeof(SDL_Rect));
    rectangle->srcRect = srcRect;
    SDL_Surface* retrieveSurface = ResourceManager_GetSurface(ResourceManager_GetInstance(), filepath);
    SDL_SetColorKey(retrieveSurface, SDL_TRUE, SDL_MapRGB(retrieveSurface->format, redColorKey, greenColorKey, blueColorKey));
    rectangle->texture = SDL_CreateTextureFromSurface(renderer, retrieveSurface);

    TexturedRectangle_InitDefaults(rectangle);

    rectangle->redColorKey = redColorKey;
    rectangle->greenColorKey = greenColorKey;
    rectangle->blueColorKey = blueColorKey;
    return rectangle;
}

void TexturedRectangle_InitDefaults(TexturedRectangle* rectangle) {
    rectangle->redColorKey = 0xFF;
    rectangle->greenColorKey = 0x00;
    rectangle->blueColorKey = 0xFF;
}

void TexturedRectangle_Destroy(TexturedRectangle* rectangle) {
    if (rectangle != NULL) {
        SDL_DestroyTexture(rectangle->texture);
        free(rectangle);
    }
}

// SDL_Rect* TexturedRectangle_GetSDLRect(TexturedRectangle* rectangle) {
//     return &rectangle->rectangle;
// }

SDL_Texture* TexturedRectangle_GetSDLTexture(TexturedRectangle* rectangle) {
    return rectangle->texture;
}

// void TexturedRectangle_SetPosition(TexturedRectangle* rectangle, int x, int y) {
//     rectangle->rectangle.x = x;
//     rectangle->rectangle.y = y;
// }

// int TexturedRectangle_GetPositionX(TexturedRectangle* rectangle) {
//     return rectangle->rectangle.x;
// }

// int TexturedRectangle_GetPositionY(TexturedRectangle* rectangle) {
//     return rectangle->rectangle.y;
// }

// void TexturedRectangle_SetDimensions(TexturedRectangle* rectangle, int w, int h) {
//     rectangle->rectangle.w = w;
//     rectangle->rectangle.h = h;
// }

// int TexturedRectangle_GetWidth(TexturedRectangle* rectangle) {
//     return rectangle->rectangle.w;
// }

// int TexturedRectangle_GetHeight(TexturedRectangle* rectangle) {
//     return rectangle->rectangle.h;
// }

int TexturedRectangle_GetRedColorKey(TexturedRectangle* rectangle) {
    return rectangle->redColorKey;
}

int TexturedRectangle_GetGreenColorKey(TexturedRectangle* rectangle) {
    return rectangle->greenColorKey;
}

int TexturedRectangle_GetBlueColorKey(TexturedRectangle* rectangle) {
    return rectangle->blueColorKey;
}

void TexturedRectangle_Update(TexturedRectangle* rectangle) {
    // Do update logic here
}

void TexturedRectangle_Render(TexturedRectangle* rectangle, float x, float y, int w, int h) {
    SDL_Rect rect = {
        x, y, w, h
    };
    SDL_RenderCopy(rectangle->m_renderer, rectangle->texture, rectangle->srcRect, &rect);
}
