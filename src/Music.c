#include "Music.h"
#include <stdio.h>

Music* Music_Create(const char* path) {
    Music* music = (Music*)malloc(sizeof(Music));
    if (music != NULL) {
        // Load the music file
        music->m_music = Mix_LoadMUS(path);
        // Error checking
        if (music->m_music == NULL) {
            printf("Could not load music: %s\n", Mix_GetError());
            free(music);
            music = NULL;
        }
    }
    return music;
}

void Music_Destroy(Music* music) {
    if (music != NULL) {
        // Free the music
        if (music->m_music != NULL) {
            Mix_FreeMusic(music->m_music);
            music->m_music = NULL;
        }
        free(music);
    }
}

void Music_Play(Music* music, int loops) {
    if (music != NULL && music->m_music != NULL) {
        Mix_PlayMusic(music->m_music, loops);
    }
}

void Music_Pause() {
    Mix_PauseMusic();
}

void Music_SetVolume(Music* music, int volume) {
    if (music != NULL && music->m_music != NULL) {
        Mix_VolumeMusic(volume);
    }
}