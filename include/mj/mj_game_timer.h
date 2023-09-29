#ifndef MJ_GAME_TIMER_H
#define MJ_GAME_TIMER_H

#include "bn_sprite_ptr.h"
#include "bn_vector.h"

namespace mj
{

class core;

class game_timer
{

public:
    void update(int pending_frames, core& core);

private:
    bn::vector<bn::sprite_ptr, 8> _sprites;
};

}

#endif
