#include <fat.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <ogc/lwp.h>
#include <string.h>

#include "headers/audio.h"
#include "headers/assets.h"
#include "headers/debug.h"

void InitializeAudioAssets() {
    if (!fatInitDefault()) {
        debug_send("Failed to initialize FAT filesystem\n");
        return;
    }

    placeMarkSoundP1 = CreateAudioAssetFromRaw(placeMarkP1, placeMarkP1_size);
    placeMarkSoundP1.autoFree = true;

    placeMarkSoundP2 = CreateAudioAssetFromRaw(placeMarkP2, placeMarkP2_size);
    placeMarkSoundP2.autoFree = true;
}


AudioAsset CreateAudioAssetFromRaw(const unsigned char *rawData, int size) {
    uint8_t *buffer = (uint8_t *)malloc(size);

    memcpy(buffer, rawData, size);

    debug_send("Audio asset created with size: %d\n", size);
    return (AudioAsset){buffer, size, -1, false, false};
}

typedef struct {
    AudioAsset *audioAsset;
    int volume;
    float pitch;
} AudioTaskData;

static void* AudioTask(void* arg) {
    AudioTaskData* data = (AudioTaskData*)arg;
    AudioAsset *audioAsset = data->audioAsset;
    
    audioAsset->voice = ASND_GetFirstUnusedVoice();
    if (audioAsset->voice < 0) {
        debug_send("No available voice for audio playback\n");
        free(data);
        return NULL;
    }

    ASND_SetVoice(audioAsset->voice, VOICE_MONO_16BIT, 48000 * data->pitch, 0, 
                  audioAsset->buffer, audioAsset->size, data->volume, data->volume, NULL);
    debug_send("Audio playback started with volume: %d on voice: %d\n", 
                data->volume, audioAsset->voice);

    free(data);
    return NULL;
}

void PlayAudioAsync(AudioAsset *audioAsset, int volumePercent, float pitch) {
    if (!audioAsset || !audioAsset->buffer) {
        debug_send("Invalid audio asset or buffer is NULL\n");
        return;
    }
    if (volumePercent < 0 || volumePercent > 100) {
        debug_send("Invalid volume percent: %d\n", volumePercent);
        return;
    }
    if (pitch <= 0.0f) {
        debug_send("Invalid pitch: %f\n", pitch);
        return;
    }

    debug_send("Playing audio asynchronously...\n");
    int volume = (volumePercent * 255) / 100;

    AudioTaskData* data = (AudioTaskData*)malloc(sizeof(AudioTaskData));
    if (!data) {
        debug_send("Error: Could not allocate memory for AudioTaskData\n");
        return;
    }
    data->audioAsset = audioAsset;
    data->volume = volume;
    data->pitch = pitch;

    lwp_t thread;
    if (LWP_CreateThread(&thread, AudioTask, data, NULL, 0, 80) != 0) {
        debug_send("Error: Could not create audio thread\n");
        free(data);
    }
}

void UpdateAudioLoop(AudioAsset *audioAsset) {
    if (!audioAsset || audioAsset->voice < 0) {
        return;
    }

    if (ASND_StatusVoice(audioAsset->voice) == SND_UNUSED) {
        if (audioAsset->loop) {
            debug_send("Restarting looped audio...\n");
            PlayAudioAsync(audioAsset, 100, 1.0f);
        } else if (audioAsset->autoFree) {
            debug_send("Freeing audio asset after playback\n");
            FreeAudio(audioAsset);
        }
    }
}

void StopAudio(const AudioAsset *audioAsset) {
    if (audioAsset->voice >= 0) {
        debug_send("Stopping audio on voice: %d\n", audioAsset->voice);
        ASND_StopVoice(audioAsset->voice);
    }
}

void FreeAudio(AudioAsset *audioAsset) {
    if (audioAsset->buffer) {
        debug_send("Freeing audio buffer\n");
        free(audioAsset->buffer);
        audioAsset->buffer = NULL;
    }
    audioAsset->size = 0;
    audioAsset->voice = -1;
    audioAsset->loop = false;
    audioAsset->autoFree = false;
    debug_send("Audio asset freed\n");
}

