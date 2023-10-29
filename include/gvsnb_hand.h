#ifndef GVSNB_HAND_H
#define GVSNB_HAND_H

#include "bn_sprite_ptr.h"

namespace gvsnb
{

class candy;
struct candy_gfx;

class hand
{

public:
    explicit hand(const candy_gfx& candy_gfx);

    [[nodiscard]] bool update(bn::fixed tempo, bool active);

    [[nodiscard]] candy create_candy(const candy_gfx& candy_gfx);

private:
    bn::sprite_ptr _candy_sprite;
    bn::sprite_ptr _hand_sprite;
    bn::fixed _x;
    bn::fixed _shoot_frames;
    int _candy_palette_index = 0;
    bool _fire = false;
};

}

#endif
