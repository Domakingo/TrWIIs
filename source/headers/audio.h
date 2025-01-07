#ifndef AUDIO_H
#define AUDIO_H

#include <gccore.h>
#include <asndlib.h>
#include <stdint.h>
#include <stdbool.h>

// Audio assets
typedef struct {
    uint8_t *buffer;
    uint32_t size;
    int voice;
    bool loop;
    bool autoFree;
} AudioAsset;

extern AudioAsset placeSound;

// Function declarations
void FreeAudio(AudioAsset *audioAsset);
void InitializeAudioAssets();
AudioAsset CreateAudioAsset(const uint8_t *buffer, uint32_t size);
void PlayAudioAsync(AudioAsset *audioAsset, int volumePercent, float pitch);
void UpdateAudioLoop(AudioAsset *audioAsset);
void StopAudio(const AudioAsset *audioAsset);

#endif
