#ifndef MJ_GAME_DATA_H
#define MJ_GAME_DATA_H

#include "bn_random.h"
#include "bn_sprite_text_generator.h"

namespace mj
{

/**
 * @brief Shared input data between all games.
 */
struct game_data
{
    const bn::sprite_text_generator& text_generator; //!< 8x16 text generator used by the parent menu.
    const bn::sprite_text_generator& small_text_generator; //!< 8x8 text generator used by the parent menu.
    const bn::sprite_text_generator& big_text_generator; //!< 16x16 text generator used by the parent menu.
    bn::random& random; //!< Random number generator used by all games. It is updated at least once per frame.
    int pending_frames; //!< Indicates how many frames are left before the current game ends.
};

}

#endif
