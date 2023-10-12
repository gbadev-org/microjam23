#ifndef MJ_DIFFICULTY_LEVEL_H
#define MJ_DIFFICULTY_LEVEL_H

#include "bn_common.h"

namespace mj
{
    /**
     * @brief Available difficulty levels.
     */
    enum class difficulty_level : uint8_t
    {
        EASY,
        NORMAL,
        HARD
    };
}

#endif
