#ifndef SDLAPP_H
#define SDLAPP_H

#include <SDL2/SDL.h>

typedef struct {
    SDL_Window* window;
    SDL_Renderer* renderer;
    int maxFrameRate;
    int gameIsRunning;
    int mouseX;
    int mouseY;
    int width;
    int height;
    void (*eventCallback)(void);
    void (*updateCallback)(Uint32 delta);
    void (*renderCallback)(void);
    SDL_TimerID* timers;
    int timerCount;
} SDLApp;

SDLApp* SDLApp_Create(Uint32 subsystemFlags, const char* title, int x, int y, int w, int h);
void SDLApp_Destroy(SDLApp* app);
void SDLApp_SetEventCallback(SDLApp* app, void (*func)(void));
void SDLApp_SetUpdateCallback(SDLApp* app, void (*func)(Uint32 delta));
void SDLApp_SetRenderCallback(SDLApp* app, void (*func)(void));
void SDLApp_RunLoop(SDLApp* app);
void SDLApp_SetMaxFrameRate(SDLApp* app, int frameRate);
SDL_Renderer* SDLApp_GetRenderer(SDLApp* app);
int SDLApp_GetMouseX(SDLApp* app);
int SDLApp_GetMouseY(SDLApp* app);
int SDLApp_GetWindowWidth(SDLApp* app);
int SDLApp_GetWindowHeight(SDLApp* app);
void SDLApp_StopAppLoop(SDLApp* app);
SDL_TimerID SDLApp_AddTimer(SDLApp* app, Uint32 delay, SDL_TimerCallback callback, void* param);
SDL_TimerID SDLApp_AddRecurringTimer(SDLApp* app, Uint32 interval, SDL_TimerCallback callback, void* param);
void SDLApp_RemoveTimer(SDLApp* app, SDL_TimerID id);

#endif