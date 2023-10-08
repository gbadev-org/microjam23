#ifndef MJ_CREDITS_TYPE_H
#define MJ_CREDITS_TYPE_H

#include "bn_common.h"

namespace mj
{
    /**
     * @brief Credits categories.
     */
    enum class credits_type : uint8_t
    {
        CODE,
        GRAPHICS,
        MUSIC,
        SOUND_EFFECTS,
    };
}

#endif
