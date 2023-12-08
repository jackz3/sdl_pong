#include "GameEntity.h"
#include "Collider.h"

GameEntity* CreateGameEntity() {
    GameEntity* entity = (GameEntity*)malloc(sizeof(GameEntity));
    if (entity != NULL) {
        // entity->rect = (SDL_Rect){0, 0, 0, 0};
        entity->m_sprite = NULL;
        entity->m_colliders = NULL;
        entity->m_colliderCount = 0;
    }
    return entity;
}

// GameEntity* CreateGameEntityWithRenderer(SDL_Renderer* renderer) {
//     GameEntity* entity = CreateGameEntity();
//     if (entity != NULL) {
//         entity->m_renderer = renderer;
//     }
//     return entity;
// }

void GameEntity_Destroy(GameEntity* entity) {
    if (entity != NULL) {
        if (entity->m_sprite != NULL) {
            TexturedRectangle_Destroy(entity->m_sprite);
            entity->m_sprite = NULL;
        }
        
        if (entity->m_colliders != NULL) {
            for (size_t i = 0; i < entity->m_colliderCount; i++) {
                BoxCollider2D_Destroy(entity->m_colliders[i]);
            }
            free(entity->m_colliders);
            entity->m_colliders = NULL;
            entity->m_colliderCount = 0;
        }
        
        free(entity);
    }
}

void UpdateGameEntity(GameEntity* entity) {
    // Update the position of our collider,
    // to be the same as the position of our Sprite component
    //if (entity->m_sprite != NULL) {
    //    int x = GetPositionX(entity->m_sprite);
    //    int y = GetPositionY(entity->m_sprite);
    //    int w = GetWidth(entity->m_sprite);
    //    int h = GetHeight(entity->m_sprite);
    //}
}

void GameEntity_Render(GameEntity* entity) {
    if (entity->m_sprite != NULL) {
        TexturedRectangle_Render(entity->m_sprite, entity->x, entity->y, entity->w, entity->h);
    }

    // for (size_t i = 0; i < entity->m_colliderCount; i++) {
    //     if (entity->m_colliders[i] != NULL) {
    //         SDL_SetRenderDrawColor(entity->m_renderer, 255, 0, 255, SDL_ALPHA_OPAQUE);
    //         SDL_RenderDrawRect(entity->m_renderer, entity->m_colliders[i] ->colliderRectangle);
    //     }
    // }
}
// void GameEntity_Render2(GameEntity* entity) {
//     if (entity->m_sprite != NULL) {
//         TexturedRectangle_Render(entity->m_sprite);
//     }
// }

void GameEntity_AddTexturedRectangleComponent(GameEntity* entity, SDL_Renderer* renderer, const char* spritepath, SDL_Rect* srcRect) {
    if (entity->m_sprite != NULL) {
        TexturedRectangle_Destroy(entity->m_sprite);
    }
    entity->m_sprite = TexturedRectangle_Create(renderer, spritepath, srcRect);
}

// void GameEntity_AddTexturedRectangleComponentWithColorKey(GameEntity* entity, const char* spritepath, int redcolorkey, int greencolorkey, int bluecolorkey) {
//     if (entity->m_sprite != NULL) {
//         TexturedRectangle_Destroy(entity->m_sprite);
//     }
//     entity->m_sprite = TexturedRectangle_CreateWithColorKey(spritepath, redcolorkey, greencolorkey, bluecolorkey);
// }

void GameEntity_AddBoxCollider2D(GameEntity* entity) {
    entity->m_colliderCount++;
    entity->m_colliders = (BoxCollider2D**)realloc(entity->m_colliders, sizeof(BoxCollider2D*) * entity->m_colliderCount);
    entity->m_colliders[entity->m_colliderCount - 1] = CreateBoxCollider2D();
}

TexturedRectangle* GameEntity_GetTexturedRectangle(GameEntity* entity) {
    return entity->m_sprite;
}

BoxCollider2D* GameEntity_GetBoxCollider2D(GameEntity* entity, size_t index) {
    if (index < entity->m_colliderCount) {
        return entity->m_colliders[index];
    }
    return NULL;
}

void GameEntity_SetPosition(GameEntity* entity, float x, float y) {
    entity->x = x;
    entity->y = y;

    for (size_t i = 0; i < entity->m_colliderCount; i++) {
        if (entity->m_colliders[i] != NULL) {
            BoxCollider2D_SetAbsolutePosition(entity->m_colliders[i], x, y);
        }
    }
}

void GameEntity_SetDimensions(GameEntity* entity, int w, int h) {
    entity->w = w;
    entity->h = h;

    for (size_t i = 0; i < entity->m_colliderCount; i++) {
        if (entity->m_colliders[i] != NULL) {
            BoxCollider2D_SetDimensions(entity->m_colliders[i], w, h);
        }
    }
}