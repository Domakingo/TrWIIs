#ifndef AUDIO_H
#define AUDIO_H

#include <gccore.h>
#include <asndlib.h>
#include <stdint.h>
#include <stdbool.h>

#include "types.h"

void InitializeAudioAssets();
AudioAsset CreateAudioAssetFromRaw(const unsigned char *rawData, int size);
void PlayAudioAsync(AudioAsset *audioAsset, int volumePercent, float pitch);
void UpdateAudioLoop(AudioAsset *audioAsset);
void StopAudio(const AudioAsset *audioAsset);
void FreeAudio(AudioAsset *audioAsset);

#endif