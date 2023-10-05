#include "mj/mj_game_timer.h"

#include "bn_sprite_items_mj_font.h"
#include "bn_sprite_tiles_items_mj_candle.h"

namespace mj
{

namespace
{
    constexpr int animation_frames = 6;
}

game_timer::game_timer() :
    _palette(bn::sprite_items::mj_font.palette_item().create_palette())
{
    for(int index = 0; index < 3; ++index)
    {
        _tiles.push_back(bn::sprite_tiles_items::mj_candle.create_tiles(index));
    }
}

void game_timer::update(int pending_frames, int total_frames)
{
    BN_ASSERT(pending_frames >= 0, "Invalid pending frames: ", pending_frames);
    BN_ASSERT(total_frames > 0, "Invalid total frames: ", total_frames);

    int width_pixels = (pending_frames * 240) / total_frames;
    BN_ASSERT(width_pixels <= 240, "Invalid width: ", width_pixels);

    int width_sprites = (width_pixels + 16 - 1) / 16;

    if(_sprites.size() > width_sprites)
    {
        _sprites.shrink(width_sprites);
    }

    bn::fixed start_x = 8 - 120;

    if(int x_desp = width_pixels % 16)
    {
        start_x -= 16 - x_desp;
    }

    for(int index = 0; index < width_sprites; ++index)
    {
        bn::fixed x = start_x + (index * 16);
        int tiles_index = 0;

        if(index == width_sprites - 1)
        {
            if(_tiles_counter)
            {
                --_tiles_counter;
            }
            else
            {
                _tiles_counter = animation_frames * 2;
            }

            tiles_index = _tiles_counter < animation_frames ? 1 : 2;
        }

        if(_sprites.size() == index)
        {
            bn::sprite_ptr sprite = bn::sprite_ptr::create(
                        x, 65, bn::sprite_tiles_items::mj_candle_shape_size, _tiles[tiles_index], _palette);
            sprite.set_bg_priority(1);
            sprite.set_z_order(-32767);
            _sprites.push_back(bn::move(sprite));
        }
        else
        {
            bn::sprite_ptr& sprite = _sprites[index];
            sprite.set_x(x);
            sprite.set_tiles(_tiles[tiles_index]);
        }
    }
}

}
