#include "mj/mj_game_backdrop.h"

#include "bn_blending.h"
#include "bn_sprite_tiles_ptr.h"
#include "bn_sprite_palette_ptr.h"

#include "bn_sprite_items_mj_bat.h"

#include "mj/mj_core.h"

namespace mj
{

namespace
{
    constexpr bn::fixed sprite_xs[8] = { -42, 86, -86, 32, -48, 16, -64, 48 };
    constexpr bn::fixed sprite_y = 128;

    void _fill_transparent_colors(bn::fixed fade_intensity, bn::color* transparent_colors)
    {
        constexpr int color_step = 10;
        constexpr int its = 160 / (color_step * 2);

        bn::fixed red_inc = bn::fixed(color_step) / color_step;
        bn::fixed green_inc = bn::fixed(0) / color_step;
        bn::fixed blue_inc = bn::fixed(color_step / 2) / color_step;
        red_inc = red_inc.unsafe_multiplication(1 - fade_intensity);
        green_inc = green_inc.unsafe_multiplication(1 - fade_intensity);
        blue_inc = blue_inc.unsafe_multiplication(1 - fade_intensity);

        bn::fixed red;
        bn::fixed green;
        bn::fixed blue;

        for(int it = 0; it < its; ++it)
        {
            for(int index = 0; index < color_step; ++index)
            {
                bn::color color(red.right_shift_integer(), green.right_shift_integer(), blue.right_shift_integer());
                *transparent_colors = color;
                ++transparent_colors;

                red += red_inc;
                green += green_inc;
                blue += blue_inc;
            }

            for(int index = 0; index < color_step; ++index)
            {
                bn::color color(red.right_shift_integer(), green.right_shift_integer(), blue.right_shift_integer());
                *transparent_colors = color;
                ++transparent_colors;

                red -= red_inc;
                green -= green_inc;
                blue -= blue_inc;
            }
        }
    };

    void _enable_blending()
    {
        bn::blending::set_transparency_alpha(0.75);
        bn::blending::set_intensity_alpha(0.75);
    }

    void _disable_blending()
    {
        bn::blending::set_transparency_alpha(1);
        bn::blending::set_intensity_alpha(0);
    }
}

game_backdrop::game_backdrop() :
    _transparent_color_hbe(bn::bg_palettes_transparent_color_hbe_ptr::create(_transparent_colors))
{
    _enable_blending();
}

game_backdrop::~game_backdrop()
{
    _disable_blending();
}

void game_backdrop::fade_out()
{
    _fade_intensity = 0;
    _fade_inc = true;
    _fade_dec = false;
}

void game_backdrop::fade_in()
{
    _counter = 0;
    _sprites.clear();
    _transparent_color_hbe.set_visible(true);

    _fade_intensity = 1;
    _fade_inc = false;
    _fade_dec = true;

    _enable_blending();
}

void game_backdrop::update(core& core)
{
    if(_counter < 0)
    {
        return;
    }

    if(_fade_inc)
    {
        _fade_intensity += bn::fixed(1) / 8;

        if(_fade_intensity >= 1)
        {
            _counter = -1;
            _sprites.clear();
            _transparent_color_hbe.set_visible(false);

            _fade_intensity = 0;
            _fade_inc = false;
            _fade_dec = false;

            _disable_blending();
            return;
        }
    }

    if(_fade_dec)
    {
        _fade_intensity -= bn::fixed(1) / 16;

        if(_fade_intensity <= 0)
        {
            _fade_intensity = 0;
            _fade_dec = false;
        }
    }

    bn::fixed y = sprite_y;

    while(! _sprites.full())
    {
        int sprite_index = _sprites.size();
        bn::fixed x = sprite_xs[sprite_index];
        y -= 32;

        if(y <= -sprite_y)
        {
            y += sprite_y * 2;
        }

        bn::sprite_ptr sprite = bn::sprite_items::mj_bat.create_sprite(x, y);
        sprite.set_bg_priority(2);
        sprite.set_z_order(-32766);
        sprite.set_blending_enabled(true);
        sprite.set_horizontal_flip(sprite_index % 2 == 1);
        sprite.set_vertical_flip(core.random().get() % 128 == 0);
        _sprites.push_back(bn::move(sprite));
    }

    for(bn::sprite_ptr& sprite : _sprites)
    {
        y = sprite.y();

        if(y <= -sprite_y)
        {
            sprite.set_y(sprite_y);
            sprite.set_vertical_flip(core.random().get() % 128 == 0);
        }
        else
        {
            sprite.set_y(y - 1);
        }
    }

    bn::sprite_tiles_ptr tiles = _sprites[0].tiles();
    tiles.set_tiles_ref(bn::sprite_items::mj_bat.tiles_item(), _counter / 3);
    _counter = (_counter + 1) % 18;

    bn::sprite_palette_ptr palette = _sprites[0].palette();
    palette.set_fade(bn::color(), _fade_intensity);
    _fill_transparent_colors(_fade_intensity, _transparent_colors);
    _transparent_color_hbe.reload_colors_ref();
}

}
