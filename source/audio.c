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
    debug_send("Creating audio asset...\n");
    AudioAsset audioAsset;
    audioAsset.buffer = (uint8_t *)buffer;
    audioAsset.size = size;
    audioAsset.voice = -1; // Initialize voice to -1 indicating no voice assigned yet
    debug_send("Audio asset created with size: %u\n", size);
    return audioAsset;
}

void PlayAudio(AudioAsset *audioAsset, int volumePercent, bool loop, float pitch, bool autoFree) {
    if (audioAsset->buffer == NULL) {
        debug_send("Audio buffer is NULL\n");
        return;
    }

    debug_send("Playing audio...\n");
    int volume = (volumePercent * 255) / 100;
    audioAsset->voice = ASND_GetFirstUnusedVoice();
    debug_send("Assigned voice: %d\n", audioAsset->voice);

    if (audioAsset->voice < 0) {
        debug_send("No available voice for audio playback\n");
        return;
    }

    ASND_SetVoice(audioAsset->voice, VOICE_STEREO_8BIT, 48000 * pitch, 0, audioAsset->buffer, audioAsset->size, volume, volume, NULL);
    debug_send("Audio playback started with volume: %d\n", volume);

    // Check the status of the voice
    int status = ASND_StatusVoice(audioAsset->voice);
    debug_send("Voice status after setting: %d\n", status);

    // Manual loop handling
    if (loop) {
        while (ASND_StatusVoice(audioAsset->voice) != SND_UNUSED) {
            usleep(1000); // Sleep for a short time to prevent busy waiting
        }
        debug_send("Looping audio...\n");
        PlayAudio(audioAsset, volumePercent, loop, pitch, autoFree);
    } else if (autoFree) {
        while (ASND_StatusVoice(audioAsset->voice) != SND_UNUSED) {
            usleep(1000); // Sleep for a short time to prevent busy waiting
        }
        debug_send("Freeing audio asset after playback\n");
        FreeAudio(audioAsset);
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
    debug_send("Audio asset freed\n");
}