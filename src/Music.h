#ifndef MUSIC_H
#define MUSIC_H

#include <string.h>
#include <SDL2/SDL_mixer.h>

typedef struct {
    Mix_Music* m_music;
} Music;

// Constructor
Music* Music_Create(const char* path);
// Destructor
void Music_Destroy(Music* music);
// Play the music
void Music_Play(Music* music, int loops);
// Pause the music
void Music_Pause();
// Set the volume of the music
void Music_SetVolume(Music* music, int volume);

#endif
