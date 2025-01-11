#ifndef AUDIO_H
#define AUDIO_H

#include <gccore.h>
#include <asndlib.h>
#include <stdint.h>
#include <stdbool.h>

typedef struct {
    uint8_t *buffer;
    uint32_t size;
    int voice;
    bool loop;
    bool autoFree;
} AudioAsset;

extern AudioAsset placeSound;

void InitializeAudioAssets();
AudioAsset CreateAudioAsset(const uint8_t *buffer, uint32_t size);
void PlayAudioAsync(AudioAsset *audioAsset, int volumePercent, float pitch);
void UpdateAudioLoop(AudioAsset *audioAsset);
void StopAudio(const AudioAsset *audioAsset);
void FreeAudio(AudioAsset *audioAsset);

#endif