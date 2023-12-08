#ifndef ENTITYMANAGER_H
#define ENTITYMANAGER_H

#include "GameEntity.h"

// #include <stdbool.h>
// #include <stdint.h>
// #include <stddef.h>
// #include <limits.h>

typedef struct EntityNode {
    char name[64];
    GameEntity* entity;
    struct EntityNode* next;
} EntityNode;

typedef struct {
    EntityNode* entities;
    size_t entityCount;
} EntityManager;

EntityManager* EntityManager_GetInstance();

bool EntityManager_CreateEntity(EntityManager* entityManager, const char* name);

GameEntity* EntityManager_GetEntityRef(EntityManager* entityManager, const char* name);

void EntityManager_RemoveEntity(EntityManager* entityManager, const char* name);

void EntityManager_UpdateAll(EntityManager* entityManager);

void EntityManager_RenderAll(EntityManager* entityManager);

void EntityManager_DeleteAll(EntityManager* entityManager);

#endif
