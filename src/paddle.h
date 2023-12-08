#ifndef paddle_H
#define paddle_H

#include "EntityManager.h"
#include "GameEntity.h"
typedef struct  
{
  char name[16];
  float dy;
} Paddle;


Paddle* Paddle_Create(const char* name, float x, float y, int w, int h);
void Paddle_Destroy(Paddle* paddle);

GameEntity* Paddle_GetGameEntity(Paddle* paddle);
void Paddle_Update(Paddle* paddle, Uint32 delta);
void Paddle_Render(Paddle* paddle);

#endif