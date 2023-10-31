#ifndef MJ_SCENE_TYPE_H
#define MJ_SCENE_TYPE_H

#include "bn_common.h"

namespace mj
{
    enum class scene_type : uint8_t
    {
        INTRO,
        TITLE,
        OPENING_A,  // old man, doorbell rings
        OPENING_B,  // front view of house, opening door
        OPENING_C,  // trick-or-treater kids
        OPENING_D,  // old man is surprised, shadow appears 
        OPENING_E,  // view of big pumpkin looming behind the kids
        OPENING_F,  // side view of pumpkin and he eats everyone
        OPENING_G,  // the vortex!
        GAME,
        GAME_ZOOM_OUT,
        CREDITS,
    };
}

#endif
