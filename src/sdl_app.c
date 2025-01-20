
#include "sdl_app.h"
#include <stdio.h>
#include <time.h>

SDLApp *SDLApp_Create(Uint32 subsystemFlags, const char *title, int x, int y, int w, int h)
{
    SDLApp *app = (SDLApp *)malloc(sizeof(SDLApp));
    app->window = NULL;
    app->renderer = NULL;
    app->maxFrameRate = 60;
    app->gameIsRunning = 1;
    app->mouseX = 0;
    app->mouseY = 0;
    app->width = w;
    app->height = h;
    app->eventCallback = NULL;
    app->updateCallback = NULL;
    app->renderCallback = NULL;
    app->timers = NULL;
    app->timerCount = 0;

    if (SDL_Init(subsystemFlags) < 0)
    {
        printf("SDL could not be initialized: %s\n", SDL_GetError());
        free(app);
        return NULL;
    }

    printf("SDL video system is ready to go\n");

    app->window = SDL_CreateWindow(title, x, y, w, h, SDL_WINDOW_SHOWN);
    app->renderer = SDL_CreateRenderer(app->window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);

    srand(time(0));
    return app;
}

void SDLApp_Destroy(SDLApp *app)
{
    if (app == NULL)
    {
        return;
    }

    SDL_DestroyRenderer(app->renderer);
    SDL_DestroyWindow(app->window);
    SDL_Quit();

    free(app->timers);
    free(app);
}

void SDLApp_SetEventCallback(SDLApp *app, void (*func)(void))
{
    app->eventCallback = func;
}

void SDLApp_SetUpdateCallback(SDLApp *app, void (*func)(Uint32 delta))
{
    app->updateCallback = func;
}

void SDLApp_SetRenderCallback(SDLApp *app, void (*func)(void))
{
    app->renderCallback = func;
}

static void mainloop(void *app_) /* this will run often, possibly at the monitor's refresh rate */
{
    SDLApp *app = (SDLApp *)app_;
    if (!app->gameIsRunning)
    {
#ifdef __EMSCRIPTEN__
        emscripten_cancel_main_loop(); /* this should "kill" the app. */
#endif
        return;
    }

    Uint32 start = SDL_GetTicks();
    static Uint32 delta = 0;

    SDL_GetMouseState(&app->mouseX, &app->mouseY);
    if (app->eventCallback != NULL)
    {
        // printf("Event callback called\n");
        app->eventCallback();
    }
    if (app->updateCallback != NULL)
    {
        Uint32 elap = SDL_GetTicks() - start;
        app->updateCallback(delta + elap);
        delta = elap;
    }
    SDL_SetRenderDrawColor(app->renderer, 40, 45, 52, SDL_ALPHA_OPAQUE);
    SDL_RenderClear(app->renderer);
    if (app->renderCallback != NULL)
    {
        // printf("Render callback called\n");
        app->renderCallback();
    }
    SDL_RenderPresent(app->renderer);

    Uint32 elapsedTime = SDL_GetTicks() - start;
    delta = elapsedTime - delta;
    if (elapsedTime < app->maxFrameRate)
    {
        Uint32 waitTime = app->maxFrameRate - elapsedTime;
        delta += waitTime;
        SDL_Delay(waitTime);
    }
}

void SDLApp_RunLoop(SDLApp *app)
{
#ifdef __EMSCRIPTEN__
    SDLApp_Destroy(app);
    
    emscripten_set_main_loop_arg(mainloop, app, 0, 1);
#else
    while (app->gameIsRunning)
    {
        mainloop(app);
    }
#endif
}

void SDLApp_SetMaxFrameRate(SDLApp *app, int frameRate)
{
    app->maxFrameRate = frameRate;
}

SDL_Renderer *SDLApp_GetRenderer(SDLApp *app)
{
    return app->renderer;
}

int SDLApp_GetMouseX(SDLApp *app)
{
    return app->mouseX;
}

int SDLApp_GetMouseY(SDLApp *app)
{
    return app->mouseY;
}

int SDLApp_GetWindowWidth(SDLApp *app)
{
    return app->width;
}

int SDLApp_GetWindowHeight(SDLApp *app)
{
    return app->height;
}

void SDLApp_StopAppLoop(SDLApp *app)
{
    app->gameIsRunning = 0;
}

SDL_TimerID SDLApp_AddTimer(SDLApp *app, Uint32 delay, SDL_TimerCallback callback, void *param)
{
    if (app->timers == NULL)
    {
        app->timers = (SDL_TimerID *)malloc(sizeof(SDL_TimerID));
    }
    else
    {
        app->timers = (SDL_TimerID *)realloc(app->timers, sizeof(SDL_TimerID) * (app->timerCount + 1));
    }

    SDL_TimerID timerID = SDL_AddTimer(delay, callback, param);
    app->timers[app->timerCount++] = timerID;

    return timerID;
}

SDL_TimerID SDLApp_AddRecurringTimer(SDLApp *app, Uint32 interval, SDL_TimerCallback callback, void *param)
{
    if (app->timers == NULL)
    {
        app->timers = (SDL_TimerID *)malloc(sizeof(SDL_TimerID));
    }
    else
    {
        app->timers = (SDL_TimerID *)realloc(app->timers, sizeof(SDL_TimerID) * (app->timerCount + 1));
    }

    SDL_TimerID timerID = SDL_AddTimer(interval, callback, param);
    app->timers[app->timerCount++] = timerID;

    return timerID;
}

void SDLApp_RemoveTimer(SDLApp *app, SDL_TimerID id)
{
    for (int i = 0; i < app->timerCount; i++)
    {
        if (app->timers[i] == id)
        {
            SDL_RemoveTimer(id);
            for (int j = i; j < app->timerCount - 1; j++)
            {
                app->timers[j] = app->timers[j + 1];
            }
            app->timerCount--;
            app->timers = (SDL_TimerID *)realloc(app->timers, sizeof(SDL_TimerID) * app->timerCount);
            break;
        }
    }
}