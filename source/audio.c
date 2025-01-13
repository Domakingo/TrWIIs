#include <fat.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <ogc/lwp.h>

#include "headers/audio.h"
#include "headers/debug.h"
#include "headers/assets/audio/placeMarkP1_wav.h"
#include "headers/assets/audio/placeMarkP2_wav.h"

AudioAsset placeMarkSoundP1, placeMarkSoundP2;

void InitializeAudioAssets() {
    placeMarkSoundP1 = CreateAudioAsset(placeMarkP1_wav, placeMarkP1_wav_size);
    placeMarkSoundP1.autoFree = true;

    placeMarkSoundP2 = CreateAudioAsset(placeMarkP2_wav, placeMarkP2_wav_size);
    placeMarkSoundP2.autoFree = true;
}

AudioAsset CreateAudioAsset(const uint8_t *buffer, uint32_t size) {
    if (!buffer || size == 0) {
        debug_send("Invalid buffer or size in CreateAudioAsset\n");
        return (AudioAsset){NULL, 0, -1, false, false};
    }
    
    debug_send("Creating audio asset...\n");
    AudioAsset audioAsset = {
        .buffer = (uint8_t *)buffer,
        .size = size,
        .voice = -1,
        .loop = false,
        .autoFree = false
    };
    debug_send("Audio asset created with size: %u\n", size);
    return audioAsset;
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

    ASND_SetVoice(audioAsset->voice, VOICE_STEREO_16BIT, 48000 * data->pitch, 0, 
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

