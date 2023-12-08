#include "Collider.h"
#include <stdio.h>
#include <stdlib.h>
// #include <string.h>

SDL_Rect* BoxCollider2D_GetColliderBoundingBox(BoxCollider2D* collider) {
    return collider->colliderRectangle;
}

SDL_bool BoxCollider2D_IsColliding(BoxCollider2D* collider, BoxCollider2D* otherCollider) {
    if (collider == NULL || otherCollider == NULL) {
        return SDL_FALSE;
    }
    
    SDL_Rect* temp = otherCollider->colliderRectangle;
    if (temp == NULL) {
        return SDL_FALSE;
    }
    printf("collider temp: %d, %d, %d, %d\n", temp->x, temp->y, temp->w, temp->h); 
    return SDL_HasIntersection(collider->colliderRectangle, temp);
}

BoxCollider2D* CreateBoxCollider2D() {
    BoxCollider2D* boxCollider = (BoxCollider2D*)malloc(sizeof(BoxCollider2D));
    boxCollider->colliderRectangle = (SDL_Rect*)malloc(sizeof(SDL_Rect));
    boxCollider->colliderRectangle->x = 0;
    boxCollider->colliderRectangle->y = 0;
    boxCollider->colliderRectangle->w = 0;
    boxCollider->colliderRectangle->h = 0;
    return boxCollider;
}

void BoxCollider2D_Destroy(BoxCollider2D* collider) {
    free(collider->colliderRectangle);
}

void BoxCollider2D_SetAbsolutePosition(BoxCollider2D* collider, int x, int y) {
    collider->colliderRectangle->x = x;
    collider->colliderRectangle->y = y;
}

void BoxCollider2D_SetDimensions(BoxCollider2D* collider, int w, int h) {
    collider->colliderRectangle ->w = w;
    collider->colliderRectangle ->h = h;
}

SDL_Point BoxCollider2D_SetBoundingBoxAutomatically(BoxCollider2D* collider, SDL_Surface* surface, Uint8 r, Uint8 g, Uint8 b) {
    if (surface == NULL) {
        fprintf(stderr, "SetBoundingBoxAutomatically: NULL detected for bounding box surface\n");
        return (SDL_Point) {0, 0};
    }
    
    SDL_LockSurface(surface);
    int w = surface->w;
    int h = surface->h;
    int pitch = surface->pitch;
    int colorchannels = pitch / w;  // typically 3 or 4
    Uint8* pixels = (Uint8*)surface->pixels;

    SDL_UnlockSurface(surface);

    int xmin = w; // Smallest value in x-axis not part of color key
    int xmax = 0; // Largest value in x-axis not part of color key
    int ymin = h;
    int ymax = 0;

    for (int y = 0; y < h; y++) {
        for (int x = 0; x < w * colorchannels; x += colorchannels) {
            if (pixels[y * (w * colorchannels) + x + 0] == b &&
                pixels[y * (w * colorchannels) + x + 1] == g &&
                pixels[y * (w * colorchannels) + x + 2] == r) {
                // pixel must be transparent
            }
            else {
                // Update the smallest and largest
                // values of non-transparent pixels
                xmin = (x < xmin) ? x : xmin;
                xmax = (x > xmax) ? x : xmax;
                ymin = (y < ymin) ? y : ymin;
                ymax = (y > ymax) ? y : ymax;
            }
        }
    }

    int scale = (1200/300);
    collider->colliderRectangle->w = ((xmax/colorchannels - xmin/colorchannels)/(scale));
    collider->colliderRectangle->h = (ymax - ymin)/scale;

    SDL_Point result;
    result.x = xmin/(colorchannels*scale);
    result.y = ymin/scale;
    return result;
}

void BoxCollider2D_Update(BoxCollider2D* collider) {
    // Implement update logic for BoxCollider2D
}

void BoxCollider2D_Render(BoxCollider2D* collider) {
    // Implement render logic for BoxCollider2D
}