#ifndef MJ_GAME_RESULT_H
#define MJ_GAME_RESULT_H

#include "bn_common.h"

namespace mj
{

/**
 * @brief The result of playing a game for a frame.
 */
struct game_result
{
    bool exit = false; //!< Indicates if the game should finish before there are no more pending frames or not.
    bool remove_title = false; //!< Indicates if the game title must be removed immediately or not.
};

}

#endif
