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
} AudioAsset;

extern AudioAsset placeSound;

// Function declarations
void FreeAudio(AudioAsset *audioAsset);
void InitializeAudioAssets();
AudioAsset CreateAudioAsset(const uint8_t *buffer, uint32_t size);
void PlayAudio(AudioAsset *audioAsset, int volumePercent, bool loop, float pitch, bool autoFree);
void StopAudio(const AudioAsset *audioAsset);

#endif