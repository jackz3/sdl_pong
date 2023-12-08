#ifndef ball_h
#define ball_h

#include <SDL2/SDL.h>
#include "GameEntity.h"

typedef struct {
  float dx;
  float dy;
  char name[16];
} Ball;

Ball* ball_create(float x, float y, int w, int h);
void ball_destroy(Ball* ball);
void ball_reset(Ball* ball);
void ball_update(Ball* ball, Uint32 dt);
void ball_render(Ball* ball);
bool ball_collides(Ball* ball, GameEntity* entity);

#endif