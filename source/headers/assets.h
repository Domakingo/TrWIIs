#ifndef ASSETS_H
#define ASSETS_H

#include <grrlib.h>

#include "types.h"

#include "assets/gfx/img/grid_png.h"
#include "assets/gfx/img/background_png.h"

#include "assets/gfx/img/p1/X_png.h"
#include "assets/gfx/img/p1/p1Draw_png.h"
#include "assets/gfx/img/p1/p1Move_png.h"
#include "assets/gfx/img/p1/p1Select_png.h"
#include "assets/gfx/img/p1/p1Win_png.h"

#include "assets/gfx/img/p2/O_png.h"
#include "assets/gfx/img/p2/p2Draw_png.h"
#include "assets/gfx/img/p2/p2Move_png.h"
#include "assets/gfx/img/p2/p2Select_png.h"
#include "assets/gfx/img/p2/p2Win_png.h"

#include "assets/audio/placeMarkP1.h"
#include "assets/audio/placeMarkP2.h"

// Assets
// Images
extern GRRLIB_texImg *backgroundTex;
extern GRRLIB_texImg *gridTex;
extern GRRLIB_texImg *XTex;
extern GRRLIB_texImg *OTex;
extern GRRLIB_texImg *p1CursorTex[4];
extern GRRLIB_texImg *p2CursorTex[4];

// Audio
extern AudioAsset placeMarkSoundP1;
extern AudioAsset placeMarkSoundP2;

#endif