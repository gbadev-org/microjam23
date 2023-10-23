#include "gvsnb_candy.h"

#include "bn_list.h"
#include "bn_memory.h"

#include "gvsnb_enemy.h"
#include "gvsnb_utils.h"

#include "bn_sprite_items_gvsnb_candy.h"

namespace gvsnb
{

namespace
{
    constexpr bn::fixed y_dec = 2.5;
    constexpr bn::fixed final_y = utils::y_1 - 4;

    constexpr bn::fixed initial_rotation_angle = 135;
    constexpr bn::fixed rotation_angle_inc = 8;

    constexpr bn::fixed new_hit_scale = 2;
    constexpr bn::fixed hit_scale_dec = bn::fixed(1) / 8;
}

candy_gfx::candy_gfx() :
    tiles(bn::sprite_items::gvsnb_candy.tiles_item().create_tiles())
{
    const bn::sprite_palette_item& palette_item = bn::sprite_items::gvsnb_candy.palette_item();

    // blue
    palettes.push_back(palette_item.create_palette());

    bn::array<bn::color, 16> colors;
    bn::sprite_palette_item colors_item(colors, palette_item.bpp());
    const bn::span<const bn::color>& item_colors_ref = palette_item.colors_ref();
    bn::memory::copy(item_colors_ref[0], 16, colors[0]);

    // orange
    for(int index = 2; index < 16; ++index)
    {
        bn::color& color = colors[index];
        color = bn::color(color.blue(), color.green(), color.red());
    }

    palettes.push_back(colors_item.create_palette());

    // pink
    for(int index = 2; index < 16; ++index)
    {
        bn::color& color = colors[index];
        color = bn::color(color.red(), color.blue(), color.green());
    }

    palettes.push_back(colors_item.create_palette());

    // green
    for(int index = 2; index < 16; ++index)
    {
        bn::color& color = colors[index];
        color = bn::color(color.green(), color.red(), color.blue());
    }

    palettes.push_back(colors_item.create_palette());
}

candy::candy(bn::fixed x, bn::fixed y, const bn::sprite_palette_ptr& palette, const candy_gfx& gfx) :
    _sprite(bn::sprite_ptr::create(0, 0, bn::sprite_items::gvsnb_candy.shape_size(), gfx.tiles, palette)),
    _x(x),
    _y(y),
    _hit_scale(new_hit_scale)
{
    _sprite.set_rotation_angle(initial_rotation_angle);
    _sprite.set_z_order(utils::candy_z_order);
}

bool candy::update(bn::fixed tempo, bn::ilist<enemy>& enemies, bool& enemy_hit)
{
    bn::fixed y = _y;

    if(_hit_scale == new_hit_scale)
    {
        _hit_scale = 0;
    }
    else
    {
        y -= y_dec.unsafe_multiplication(utils::scale(y)).unsafe_multiplication(tempo);
        _y = y;

        if(y <= final_y)
        {
            return false;
        }
    }

    bn::fixed x = _x;
    bn::fixed scale = 1;

    if(_hit_scale > 0)
    {
        _hit_scale -= hit_scale_dec.unsafe_multiplication(tempo);

        if(_hit_scale <= 0)
        {
            return false;
        }

        scale = _hit_scale;
    }
    else
    {
        for(enemy& enemy : enemies)
        {
            if(enemy.candy_hit(x, y))
            {
                _hit_scale = 1;
                enemy_hit = true;
                break;
            }
        }
    }

    _sprite.set_position(x.unsafe_multiplication(utils::x_mult(y)), y);
    _sprite.set_scale(bn::max(scale.unsafe_multiplication(utils::scale(y)), bn::fixed(0.01)));

    bn::fixed rotation_angle = _sprite.rotation_angle() - rotation_angle_inc.unsafe_multiplication(tempo);

    if(rotation_angle < 0)
    {
        rotation_angle += 360;
    }

    _sprite.set_rotation_angle(rotation_angle);
    return true;
}

}
