#ifndef Sound_H
#define Sound_H

#include <SDL2/SDL_mixer.h>

typedef struct {
    Mix_Chunk* m_sound;
} Sound;

// Constructor
Sound* Sound_Create(const char* path);

// Destructor
void Sound_Destroy(Sound* music);

// Play the music
void Sound_Play(Sound* music, int loops);

// Pause the music
// void Sound_Pause();

// Set the volume of the music
// void Music_SetVolume(Music* music, int volume);

#endif
