#include "headers/assets.h"

// Assets
// Images
GRRLIB_texImg *backgroundTex = NULL;
GRRLIB_texImg *gridTex = NULL;
GRRLIB_texImg *XTex = NULL;
GRRLIB_texImg *OTex = NULL;

GRRLIB_texImg *p1CursorTex[4] = {NULL, NULL, NULL, NULL};
GRRLIB_texImg *p2CursorTex[4] = {NULL, NULL, NULL, NULL};

// Audio
AudioAsset placeMarkSoundP1 = (AudioAsset){NULL, 0, -1, false, false};
AudioAsset placeMarkSoundP2 = (AudioAsset){NULL, 0, -1, false, false};