#ifndef COLLIDER_H
#define COLLIDER_H

#include <SDL2/SDL.h>
#include "TexturedRectangle.h"

typedef struct BoxCollider2D {
    SDL_Rect* colliderRectangle;
    void (*Update)();
    void (*Render)();
} BoxCollider2D;

BoxCollider2D* CreateBoxCollider2D();
void BoxCollider2D_Destroy(BoxCollider2D* collider);
SDL_Rect* BoxCollider2D_GetColliderBoundingBox(BoxCollider2D* collider);
SDL_bool BoxCollider2D_IsColliding(BoxCollider2D* collider1, BoxCollider2D* collider2);
void BoxCollider2D_SetAbsolutePosition(BoxCollider2D* collider, int x, int y);
void BoxCollider2D_SetDimensions(BoxCollider2D* collider, int w, int h);
SDL_Point BoxCollider2D_SetBoundingBoxAutomatically(BoxCollider2D* collider, SDL_Surface* surface, Uint8 r, Uint8 g, Uint8 b);
void BoxCollider2D_Update(BoxCollider2D* collider);
void BoxCollider2D_Render(BoxCollider2D* collider);

#endif