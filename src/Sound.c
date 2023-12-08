#include "Sound.h"
#include <stdio.h>

Sound* Sound_Create(const char* path) {
    Sound* sound = (Sound*)malloc(sizeof(Sound));
    if (sound != NULL) {
        // Load the Sound file
        sound->m_sound = Mix_LoadWAV(path);
        // Error checking
        if (sound->m_sound == NULL) {
            printf("Could not load Sound: %s\n", Mix_GetError());
            free(sound);
            sound = NULL;
        }
    }
    return sound;
}

void Sound_Destroy(Sound* Sound) {
    if (Sound != NULL) {
        // Free the Sound
        if (Sound->m_sound != NULL) {
            Mix_FreeChunk(Sound->m_sound);
            Sound->m_sound = NULL;
        }
        free(Sound);
    }
}

void Sound_Play(Sound* sound, int loops) {
    if (sound != NULL && sound->m_sound != NULL) {
        Mix_PlayChannel(-1, sound->m_sound, loops);
    }
}

// void Sound_Pause() {
//     Mix_PauseSound();
// }

// void Sound_SetVolume(Sound* Sound, int volume) {
//     if (Sound != NULL && Sound->m_sound != NULL) {
//         Mix_VolumeSound(volume);
//     }
// }