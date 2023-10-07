#ifndef MJ_GAME_TIMER_H
#define MJ_GAME_TIMER_H

#include "bn_sprite_ptr.h"
#include "bn_sprite_tiles_ptr.h"
#include "bn_sprite_palette_ptr.h"
#include "bn_vector.h"

namespace mj
{

class game_timer
{

public:
    game_timer();

    void update(int pending_frames, int total_frames);

private:
    bn::vector<bn::sprite_ptr, 16> _sprites;
    bn::optional<bn::sprite_ptr> _fire_sprite;
    bn::vector<bn::sprite_tiles_ptr, 2> _tiles;
    bn::sprite_palette_ptr _palette;
    int _fire_counter = 0;
};

}

#endif
