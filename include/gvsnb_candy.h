#ifndef GVSNB_CANDY_H
#define GVSNB_CANDY_H

#include "bn_sprite_palette_ptr.h"
#include "bn_sprite_ptr.h"
#include "bn_sprite_tiles_ptr.h"
#include "bn_vector.h"

namespace gvsnb
{

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

    [[nodiscard]] bn::fixed x() const
    {
        return _x;
    }

    [[nodiscard]] bn::fixed y() const
    {
        return _y;
    }

    [[nodiscard]] bool alive() const
    {
        return _hit_scale == 0;
    }

    void hit()
    {
        _hit_scale = 1;
    }

    [[nodiscard]] bool update(bn::fixed tempo);

private:
    bn::sprite_ptr _sprite;
    bn::fixed _x;
    bn::fixed _y;
    bn::fixed _hit_scale;
};

}

#endif
