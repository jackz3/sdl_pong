#include <stdlib.h>
#include <string.h>
#include "ball.h"
#include "util.h"
#include "sdl_app.h"
#include "EntityManager.h"
#include "Collider.h"


extern SDLApp* app;

Ball* ball_create(float x, float y, int w, int h) {
  Ball* ball = (Ball*)malloc(sizeof(Ball));
  ball->dy = 0;
  ball->dx = 0;
  strcpy(ball->name, "ball");
  EntityManager_CreateEntity(EntityManager_GetInstance(), ball->name);
  GameEntity* entity = EntityManager_GetEntityRef(EntityManager_GetInstance(), ball->name);
  SDL_Rect* rect = malloc(sizeof(SDL_Rect));
  rect->x = 102;
  rect->y = 2;
  rect->w = 48;
  rect->h = 48;
  GameEntity_AddTexturedRectangleComponent(entity, app->renderer, "./assets/images/sprites.png", rect);
  GameEntity_AddBoxCollider2D(entity);
  GameEntity_SetPosition(entity, x, y);
  GameEntity_SetDimensions(entity, w, h);
  return ball;
}

void ball_destroy(Ball* ball) {
  free(ball);
}
void ball_reset(Ball* ball) {
  GameEntity* entity = EntityManager_GetEntityRef(EntityManager_GetInstance(), ball->name);
  entity->x = SDLApp_GetWindowWidth(app) / 2 - 6;
  entity->y = SDLApp_GetWindowHeight(app) / 2 - 6;
  ball->dx = 0;
  ball->dy = 0;
}

void ball_update(Ball* ball, Uint32 dt) {
  GameEntity* entity = EntityManager_GetEntityRef(EntityManager_GetInstance(), ball->name);
  entity->x += ball->dx * dt / 1000;
  entity->y += ball->dy * dt / 1000;
}

void ball_render(Ball* ball) {
  GameEntity* entity = EntityManager_GetEntityRef(EntityManager_GetInstance(), ball->name);
  GameEntity_Render(entity);
}

bool ball_collides(Ball* ball, GameEntity* entity) {
  GameEntity* ballEntity = EntityManager_GetEntityRef(EntityManager_GetInstance(), ball->name);
  SDL_Rect ballRect = {ballEntity->x, ballEntity->y, ballEntity->w, ballEntity->h};
  SDL_Rect entityRect = {entity->x, entity->y, entity->w, entity->h};
  return SDL_HasIntersection(&ballRect, &entityRect);
  // return BoxCollider2D_IsColliding(GameEntity_GetBoxCollider2D(ballEntity, 0), GameEntity_GetBoxCollider2D(entity, 0));  
}
