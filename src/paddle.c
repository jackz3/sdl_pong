#include <string.h>
#include <stdlib.h>
#include "paddle.h"
#include "sdl_app.h"
#include "util.h"
#include "EntityManager.h"

extern SDLApp* app;

Paddle* Paddle_Create(const char* name, float x, float y, int w, int h) {
  Paddle* paddle = (Paddle*)malloc(sizeof(Paddle));
  paddle->dy = 0;
  strcpy(paddle->name, name);
  EntityManager* entityMgr = EntityManager_GetInstance();
  EntityManager_CreateEntity(entityMgr, name);
  GameEntity* entity = EntityManager_GetEntityRef(entityMgr, name); 
  SDL_Rect* rect = malloc(sizeof(SDL_Rect));
  if (strcmp(name, "left") == 0) {
    rect->x = 12;
  } else {
    rect->x = 64;
  }
  rect->y = 12;
  rect->w = 28;
  rect->h = 124;
  GameEntity_AddTexturedRectangleComponent(entity, app->renderer, "./assets/images/sprites.png", rect);
  GameEntity_AddBoxCollider2D(entity);
  GameEntity_SetPosition(entity, x, y);
  GameEntity_SetDimensions(entity, w, h);
  return paddle;
}

GameEntity* Paddle_GetGameEntity(Paddle* paddle) {
  return EntityManager_GetEntityRef(EntityManager_GetInstance(), paddle->name);
}

void Paddle_Update(Paddle* paddle, Uint32 delta) {
  GameEntity* entity = EntityManager_GetEntityRef(EntityManager_GetInstance(), paddle->name);
  if (paddle->dy < 0) {
    entity->y = MAX(0, entity->y + paddle->dy * delta / 1000);
  } else {
    entity->y = MIN(app->height - entity->h, entity->y + paddle->dy * delta / 1000);
  }
}

void Paddle_Render(Paddle* paddle) {
  GameEntity* entity = EntityManager_GetEntityRef(EntityManager_GetInstance(), paddle->name);
  GameEntity_Render(entity);
}

void Paddle_Destroy(Paddle* paddle) {
  free(paddle);
}