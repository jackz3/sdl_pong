#include "EntityManager.h"
#include <stdio.h>
#include <stdlib.h>

EntityManager* EntityManager_GetInstance() {
    static EntityManager* instance;
    if (instance == NULL) {
        instance = (EntityManager*)malloc(sizeof(EntityManager));
        if (instance != NULL) {
            instance->entities = NULL;
            instance->entityCount = 0;
        }
    }
    return instance;
}

bool EntityManager_CreateEntity(EntityManager* manager, const char* name) {
    GameEntity* newEntity = CreateGameEntity(name);
    if (!newEntity) {
        return false;
    }

    EntityNode* newNode = malloc(sizeof(EntityNode));
    strncpy(newNode->name, name, sizeof(newNode->name) - 1);
    newNode->name[sizeof(newNode->name) - 1] = '\0';
    newNode->entity = newEntity;
    newNode->next = manager->entities;

    manager->entities = newNode;
    manager->entityCount++;

    return true;
}

// bool EntityManager_CreateEntity(EntityManager* entityManager, const char* name) {
//     if (entityManager == NULL || name == NULL) {
//         return false;
//     }

//     GameEntity* newEntity = CreateGameEntity(name); //  (GameEntity*)malloc(sizeof(GameEntity));
//     if (newEntity == NULL) {
//         return false;
//     }

//     entityManager->entities = (GameEntity*)realloc(entityManager->entities, (entityManager->entityCount + 1) * sizeof(GameEntity));
//     if (entityManager->entities == NULL) {
//         free(newEntity);
//         return false;
//     }

//     entityManager->entities[entityManager->entityCount] = *newEntity;
//     entityManager->entityCount++;

//     free(newEntity);
//     return true;
// }

GameEntity* EntityManager_GetEntityRef(EntityManager* manager, const char* name) {
    EntityNode* node = manager->entities;
    while (node) {
        if (strcmp(node->name, name) == 0) {
            return node->entity;
        }

        node = node->next;
    }

    return NULL;
}
// GameEntity* EntityManager_GetEntityRef(EntityManager* entityManager, const char* name) {
//     if (entityManager == NULL || name == NULL) {
//         return NULL;
//     }

//     for (size_t i = 0; i < entityManager->entityCount; i++) {
//         if (strcmp(entityManager->entities[i].name, name) == 0) {
//             return &(entityManager->entities[i]);
//         }
//     }

//     return NULL;
// }

void EntityManager_RemoveEntity(EntityManager* manager, const char* name) {
    EntityNode* prev = NULL;
    EntityNode* node = manager->entities;

    while (node) {
        if (strcmp(node->name, name) == 0) {
            if (prev) {
                prev->next = node->next;
            } else {
                manager->entities = node->next;
            }

            GameEntity_Destroy(node->entity);
            free(node);
            manager->entityCount--;
            return;
        }

        prev = node;
        node = node->next;
    }
}
// void EntityManager_RemoveEntity(EntityManager* entityManager, const char* name) {
//     if (entityManager == NULL || name == NULL) {
//         return;
//     }
//     for (size_t i = 0; i < entityManager->entityCount; i++) {
//         if (strcmp(entityManager->entities[i].name, name) == 0) {
//             for (size_t j = i; j < entityManager->entityCount - 1; j++) {
//                 entityManager->entities[j] = entityManager->entities[j + 1];
//             }
//             entityManager->entityCount--;
//             entityManager->entities = (GameEntity*)realloc(entityManager->entities, entityManager->entityCount * sizeof(GameEntity));
//             return;
//         }
//     }
// }

void EntityManager_UpdateAll(EntityManager* manager) {
    EntityNode* node = manager->entities;
    while (node) {
        // GameEntity_Update(node->entity);
        node = node->next;
    }
}

void EntityManager_RenderAll(EntityManager* manager) {
    EntityNode* node = manager->entities;
    while (node) {
        GameEntity_Render(node->entity);
        node = node->next;
    }
}

void EntityManager_DeleteAll(EntityManager* manager) {
    while (manager->entities) {
        EntityNode* node = manager->entities;
        manager->entities = node->next;
        GameEntity_Destroy(node->entity);
        free(node);
    }

    manager->entityCount = 0;
}
