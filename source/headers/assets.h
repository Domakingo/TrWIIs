#ifndef ASSETS_H
#define ASSETS_H

#include <grrlib.h>

#include "types.h"

#include "assets/gfx/img/grid_png.h"
#include "assets/gfx/img/background_png.h"
#include "assets/gfx/img/X_png.h"
#include "assets/gfx/img/O_png.h"
#include "assets/gfx/img/p1Cursor_png.h"
#include "assets/gfx/img/p2Cursor_png.h"

#include "assets/audio/placeMarkP1.h"
#include "assets/audio/placeMarkP2.h"

// Assets
// Images
extern GRRLIB_texImg *backgroundTex;
extern GRRLIB_texImg *gridTex;
extern GRRLIB_texImg *XTex;
extern GRRLIB_texImg *OTex;
extern GRRLIB_texImg *p1CursorTex;
extern GRRLIB_texImg *p2CursorTex;

// Audio
extern AudioAsset placeMarkSoundP1;
extern AudioAsset placeMarkSoundP2;

#endif