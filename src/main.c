#define SDL_MAIN_HANDLED
#ifdef __EMSCRIPTEN__
#include <emscripten.h>
#endif
#include <stdio.h>
#include "util.h"
#include "sdl_app.h"
#include "TexturedRectangle.h"
#include "GameEntity.h"
#include "DynamicText.h"
#include "ResourceManager.h"
#include "ball.h"
#include "paddle.h"
#include "Music.h"
#include "Sound.h"

#define WIDTH 1280
#define HEIGHT 720
#define PADDLE_SPEED 500

// One possibility of creating as a global our app
SDLApp *app;
Ball *ball;
Paddle *player1;
Paddle *player2;

enum State
{
    START,
    PLAY,
    SERVE,
    DONE
};

typedef struct
{
    int player1Score;
    int player2Score;
    int servingPlayer;
    int winningPlayer;
    enum State state;
} GameState;

GameState *gameState;

// Callback function for handling events
void HandleEvents()
{
    SDL_Event event;
    while (SDL_PollEvent(&event))
    {
        if (event.type == SDL_QUIT || event.key.keysym.sym == SDLK_ESCAPE || event.type == SDL_WINDOWEVENT_CLOSE)
        {
            SDLApp_StopAppLoop(app);
        }
        else if (event.type == SDL_KEYDOWN)
        {
            switch (event.key.keysym.sym)
            {
            case SDLK_ESCAPE:
                SDLApp_StopAppLoop(app);
                break;
            case SDLK_RETURN:
                if (gameState->state == START)
                {
                    gameState->state = SERVE;
                }
                else if (gameState->state == SERVE)
                {
                    gameState->state = PLAY;
                }
                else if (gameState->state == DONE)
                {
                    gameState->state = SERVE;
                    ball_reset(ball);
                    gameState->player1Score = 0;
                    gameState->player2Score = 0;
                    if (gameState->winningPlayer == 1)
                    {
                        gameState->servingPlayer = 2;
                    }
                    else
                    {
                        gameState->servingPlayer = 1;
                    }
                }
                break;
            }
        }
    }
    const Uint8 *state = SDL_GetKeyboardState(NULL);
    if (state[SDL_SCANCODE_UP])
    {
        player2->dy = -PADDLE_SPEED;
    }
    else if (state[SDL_SCANCODE_DOWN])
    {
        player2->dy = PADDLE_SPEED;
    }
    else{
        player2->dy = 0;
    } 
    if (state[SDL_SCANCODE_W])
    {
        player1->dy = -PADDLE_SPEED;
    }
    else if (state[SDL_SCANCODE_S])
    {
        player1->dy = PADDLE_SPEED;
    }else {
        player1->dy = 0;
    }
}

// Update callback
void HandleUpdate(Uint32 delta)
{
    if (gameState->state == SERVE)
    {
        ball->dy = rand_int(-50, 50) * 3;
        if (gameState->servingPlayer == 1)
        {
            ball->dx = rand_int(140, 200) * 3;
        }
        else
        {
            ball->dx = -rand_int(140, 200) * 3;
        }
    }
    else if (gameState->state == PLAY)
    {
        ResourceManager* rm = ResourceManager_GetInstance();
        GameEntity *ballEntity = EntityManager_GetEntityRef(EntityManager_GetInstance(), ball->name);
        GameEntity *player1Entity = Paddle_GetGameEntity(player1);
        GameEntity* player2Entity = Paddle_GetGameEntity(player2);
        if (ball_collides(ball, player1Entity))
        {
            ball->dx = -(ball->dx * 1.03);
            ballEntity->x = player1Entity->x + 15;

            if (ball->dy < 0)
                ball->dy = -rand_int(10, 150) * 3;
            else
                ball->dy = rand_int(10, 150) * 3;

            Sound_Play(ResourceManager_GetSound(rm, "paddle_hit"), 0);
        }
        if(ball_collides(ball, player2Entity)) {
            
            ball->dx = ball->dx * -1.03;
            ballEntity->x = player2Entity->x - 34;
            printf("Ball collided with player 2 x: %f dx %f\n", ballEntity->x, ball->dx);
            if(ball->dy < 0)
                ball->dy = -rand_int(10, 150) * 3;
            else
                ball->dy = rand_int(10, 150) * 3;

            Sound_Play(ResourceManager_GetSound(rm, "paddle_hit"), 0);
        }
        if (ballEntity->y <= 0)
        {
            ballEntity->y = 0;
            ball->dy = -ball->dy;
            Sound_Play(ResourceManager_GetSound(rm, "wall_hit"), 0);
        }
        if (ballEntity->y >= app->height - 4 * 3)
        {
            ballEntity->y = app->height - 4 * 3;
            ball->dy = -ball->dy;
            Sound_Play(ResourceManager_GetSound(rm, "wall_hit"), 0);
        }
        if (ballEntity->x < 0)
        {
            gameState->servingPlayer = 1;
            gameState->player2Score++;
            Sound_Play(ResourceManager_GetSound(rm, "score"), 0);
            if (gameState->player2Score == 3)
            {
                gameState->winningPlayer = 2;
                gameState->state = DONE;
            }
            else
            {
                gameState->state = SERVE;
                ball_reset(ball);
            }
        }

        if (ballEntity->x > WIDTH)
        {
            gameState->servingPlayer = 2;
            gameState->player1Score++;
            Sound_Play(ResourceManager_GetSound(rm, "score"), 0);
            if (gameState->player1Score == 10)
            {
                gameState->winningPlayer = 1;
                gameState->state = DONE;
            }
            else
            {
                gameState->state = SERVE;
                ball_reset(ball);
            }
        }
        ball_update(ball, delta);
    }

    Paddle_Update(player1, delta);
    Paddle_Update(player2, delta);
}

// Handle the rendering of the game entities
void HandleRendering()
{
    ResourceManager *rm = ResourceManager_GetInstance();
    DynamicText *text = ResourceManager_GetFont(rm, "peaberry");
    if (gameState->state == START)
    {
        DynamicText_SetSize(text, 24);
        DynamicText_DrawWithAlignment(text, app->renderer, "Welcome to Pong!", 0, 30, WIDTH, 1);
        DynamicText_DrawWithAlignment(text, app->renderer, "Press Enter to begin!", 0, 60, WIDTH, 1);
    }
    else if (gameState->state == SERVE)
    {
        DynamicText_SetSize(text, 24);
        char serveText[64];
        sprintf(serveText, "Player %d's serve!", gameState->servingPlayer);
        DynamicText_DrawWithAlignment(text, app->renderer, serveText, 0, 30, WIDTH, 1);
        DynamicText_DrawWithAlignment(text, app->renderer, "Press Enter to serve!", 0, 60, WIDTH, 1);
    }
    else if (gameState->state == DONE)
    {
        DynamicText_SetSize(text, 48);
        char winningText[64];
        sprintf(winningText, "Player %d wins!", gameState->winningPlayer);
        DynamicText_DrawWithAlignment(text, app->renderer, winningText, 0, 30, WIDTH, 1);
        DynamicText_SetSize(text, 24);
        DynamicText_DrawWithAlignment(text, app->renderer, "Press Enter to restart!", 0, 90, WIDTH, 1);
    }

    DynamicText_SetSize(text, 96);
    char playerScore[8];
    sprintf(playerScore, "%d", gameState->player1Score);
    DynamicText_Draw(text, app->renderer, playerScore, WIDTH / 2 - 150, HEIGHT / 3);
    sprintf(playerScore, "%d", gameState->player2Score);
    DynamicText_Draw(text, app->renderer, playerScore, WIDTH / 2 + 150, HEIGHT / 3);

    ball_render(ball);
    Paddle_Render(player1);
    Paddle_Render(player2);
}

Uint32 my_callbackfunc(Uint32 interval, void *param)
{
    printf("Fun score updated from: %s:s s\n", (const char *)param);
    return 0;
}

// Entry point of the program
int main(int argc, char *argv[])
{
    // Setup the SDLApp
    const char *title = "SDL2_Pong";
    app = SDLApp_Create(SDL_INIT_VIDEO | SDL_INIT_AUDIO | SDL_INIT_TIMER, title, 80, 80, WIDTH, HEIGHT);
    SDLApp_SetMaxFrameRate(app, 16);
    ResourceManager *rm = ResourceManager_GetInstance();
    ResourceManager_LoadFont(rm, "peaberry", "./assets/fonts/PeaberryMono.ttf");
    // ResourceManager_InitText(rm, "./assets/fonts/PeaberryMono.ttf");
    // Create entities

    player1 = Paddle_Create("left", 30, 90, 20, 80);
    player2 = Paddle_Create("right", WIDTH - 30, HEIGHT - 90, 20, 80);
    ball = ball_create(SDLApp_GetWindowWidth(app) / 2 - 6, SDLApp_GetWindowHeight(app) / 2 - 6, 40, 40);

    ResourceManager_LoadSound(rm, "paddle_hit", "./assets/sounds/paddle_hit.wav");
    ResourceManager_LoadSound(rm, "score", "./assets/sounds/score.wav");
    ResourceManager_LoadSound(rm, "wall_hit", "./assets/sounds/wall_hit.wav");
    ResourceManager_LoadMusic(rm, "bg", "./assets/sounds/DigitalWaves.mp3");
    Music_Play(ResourceManager_GetMusic(rm, "bg"), -1);

    // Setup the Game State
    gameState = (GameState *)malloc(sizeof(GameState));
    gameState->player1Score = 0;
    gameState->player2Score = 0;
    gameState->servingPlayer = 1;
    gameState->winningPlayer = 0;
    gameState->state = START;

    // Set callback functions
    SDLApp_SetEventCallback(app, HandleEvents);
    SDLApp_SetUpdateCallback(app, HandleUpdate);
    SDLApp_SetRenderCallback(app, HandleRendering);

    // Add some timers (for demonstration purpose)
    // SDLApp_AddTimer(app, 2000, my_callbackfunc, (char *)"timer called");
    // Run our application until terminated
    SDLApp_RunLoop(app);

    // Clean up our application
    SDLApp_Destroy(app);
    ResourceManager_Destroy(rm);
    ball_destroy(ball);
    Paddle_Destroy(player1);
    Paddle_Destroy(player2);
    printf("end\n");
    // EntityManager_DeleteAll(entityMgr);
    return 0;
}