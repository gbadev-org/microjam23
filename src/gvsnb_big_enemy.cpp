#include "gvsnb_big_enemy.h"

#include "gvsnb_enemy.h"
#include "gvsnb_utils.h"

#include "bn_sprite_tiles_items_gvsnb_kid_big.h"

namespace gvsnb
{

namespace
{
    constexpr bn::fixed y_delay = 64;
    constexpr bn::fixed initial_y = -256 - y_delay;
    constexpr bn::fixed final_y = initial_y + 192 + y_delay;
    constexpr int y_inc = 4;
}

big_enemy::big_enemy(const enemy_gfx& gfx)
{
    for(int index = 0; index < 6; ++index)
    {
        bn::sprite_ptr sprite = bn::sprite_ptr::create(
                0, 0, bn::sprite_tiles_items::gvsnb_kid_big_shape_size,
                bn::sprite_tiles_items::gvsnb_kid_big.create_tiles(index), gfx.palettes[0]);
        sprite.set_z_order(utils::big_enemy_z_order);
        sprite.set_visible(false);
        _sprites.push_back(bn::move(sprite));
    }
}

void big_enemy::setup(const enemy& enemy, const enemy_gfx& gfx)
{
    if(! _sprites[0].visible())
    {
        const bn::sprite_palette_ptr& palette = gfx.palettes[enemy.difficulty_level() - 1];
        bool horizontal_flip = enemy.horizontal_flip();

        for(bn::sprite_ptr& sprite : _sprites)
        {
            sprite.set_palette(palette);
            sprite.set_horizontal_flip(horizontal_flip);
            sprite.set_visible(true);
        }

        bn::fixed y = initial_y;
        _sprites[0].set_position(horizontal_flip ? 32 : -32, y);
        _sprites[1].set_position(horizontal_flip ? -32 : 32, y);
        y += 64;
        _sprites[2].set_position(horizontal_flip ? 32 : -32, y);
        _sprites[3].set_position(horizontal_flip ? -32 : 32, y);
        y += 64;
        _sprites[4].set_position(horizontal_flip ? 32 : -32, y);
        _sprites[5].set_position(horizontal_flip ? -32 : 32, y);
    }
}

bool big_enemy::update(bn::fixed tempo)
{
    if(_sprites[0].visible())
    {
        bn::fixed inc = y_inc * tempo;

        for(bn::sprite_ptr& sprite : _sprites)
        {
            sprite.set_y(sprite.y() + inc);
        }

        return _sprites[0].y() < final_y;
    }

    return true;
}

}
