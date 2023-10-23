#ifndef GVSNB_CANDY_H
#define GVSNB_CANDY_H

#include "bn_list_fwd.h"
#include "bn_sprite_palette_ptr.h"
#include "bn_sprite_ptr.h"
#include "bn_sprite_tiles_ptr.h"
#include "bn_vector.h"

namespace gvsnb
{

class enemy;

struct candy_gfx
{
    bn::sprite_tiles_ptr tiles;
    bn::vector<bn::sprite_palette_ptr, 4> palettes;

    candy_gfx();
};


class candy
{

public:
    candy(bn::fixed x, bn::fixed y, const bn::sprite_palette_ptr& palette, const candy_gfx& gfx);

    [[nodiscard]] bool update(bn::fixed tempo, bn::ilist<enemy>& enemies, bool& enemy_hit);

private:
    bn::sprite_ptr _sprite;
    bn::fixed _x;
    bn::fixed _y;
    bn::fixed _hit_scale;
};

}

#endif
