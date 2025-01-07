#include <fat.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "headers/audio.h"
#include "headers/debug.h"

// Audio assets
#include "headers/assets/audio/placeMark_wav.h"

// Declare the AudioAsset
AudioAsset placeSound;

void InitializeAudioAssets() {
    // Initialize the AudioAsset
    placeSound = CreateAudioAsset(placeMark_wav, placeMark_wav_size);
}

AudioAsset CreateAudioAsset(const uint8_t *buffer, uint32_t size) {
    if (!buffer || size == 0) {
        debug_send("Invalid buffer or size in CreateAudioAsset\n");
        AudioAsset audioAsset = { NULL, 0, -1, false, false };
        return audioAsset;
    }
    
    debug_send("Creating audio asset...\n");
    AudioAsset audioAsset;
    audioAsset.buffer = (uint8_t *)buffer;
    audioAsset.size = size;
    audioAsset.voice = -1; // Initialize voice to -1 indicating no voice assigned yet
    audioAsset.loop = false;
    audioAsset.autoFree = false;
    debug_send("Audio asset created with size: %u\n", size);
    return audioAsset;
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
    audioAsset->voice = ASND_GetFirstUnusedVoice();
    if (audioAsset->voice < 0) {
        debug_send("No available voice for audio playback\n");
        return;
    }

    ASND_SetVoice(audioAsset->voice, VOICE_STEREO_8BIT, 48000 * pitch, 0, audioAsset->buffer, audioAsset->size, volume, volume, NULL);
    debug_send("Audio playback started with volume: %d on voice: %d\n", volume, audioAsset->voice);
}

void UpdateAudioLoop(AudioAsset *audioAsset) {
    if (!audioAsset || audioAsset->voice < 0) {
        return;
    }

    int status = ASND_StatusVoice(audioAsset->voice);
    if (status == SND_UNUSED) {
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