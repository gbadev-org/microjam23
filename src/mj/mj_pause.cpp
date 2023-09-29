#include "mj/mj_pause.h"

#include "bn_bg_palettes.h"
#include "bn_keypad.h"
#include "bn_sprite_builder.h"
#include "bn_sprite_palettes.h"

#include "mj/mj_core.h"

#include "bn_sprite_items_mj_font.h"
#include "bn_sprite_tiles_items_mj_pause_dialog.h"
#include "bn_sprite_tiles_items_mj_pause_dialog_lateral.h"

namespace mj
{

namespace
{
    constexpr bn::fixed continue_x = -51;
    constexpr bn::fixed exit_x = 32;
}

pause::pause(core& core)
{
    {
        bn::sprite_item sprite_item(
                    bn::sprite_tiles_items::mj_pause_dialog_shape_size, bn::sprite_tiles_items::mj_pause_dialog,
                    bn::sprite_items::mj_font.palette_item());
        bn::sprite_builder sprite_builder(sprite_item);
        sprite_builder.set_bg_priority(0);
        sprite_builder.set_z_order(-1);

        for(int index = 0; index < 4; ++index)
        {
            sprite_builder.set_x(-47 + (index * 31));
            _sprites.push_back(sprite_builder.build());
        }
    }

    {
        bn::sprite_item sprite_item(
                    bn::sprite_tiles_items::mj_pause_dialog_lateral_shape_size,
                    bn::sprite_tiles_items::mj_pause_dialog_lateral, bn::sprite_items::mj_font.palette_item());
        bn::sprite_builder sprite_builder(sprite_item);
        sprite_builder.set_bg_priority(0);
        sprite_builder.set_z_order(-1);
        sprite_builder.set_horizontal_flip(true);
        sprite_builder.set_x(_sprites.back().x() + 20);
        _sprites.push_back(sprite_builder.build());

        sprite_builder.set_horizontal_flip(false);
        sprite_builder.set_x(_sprites[0].x() - 20);
        _sprites.push_back(sprite_builder.release_build());
    }

    bn::sprite_text_generator& small_text_generator = core.small_text_generator();
    small_text_generator.set_bg_priority(0);
    small_text_generator.set_z_order(-1);

    small_text_generator.set_center_alignment();
    small_text_generator.generate(0, -8, "PAUSE", _sprites);
    small_text_generator.set_left_alignment();

    bn::fixed y = 7;
    small_text_generator.generate(continue_x, y, "CONTINUE", _sprites);
    small_text_generator.generate(exit_x, y, "EXIT", _sprites);

    small_text_generator.set_one_sprite_per_character(true);
    small_text_generator.generate(0, y, ">", _sprites);

    for(bn::sprite_ptr& sprite : _sprites)
    {
        sprite.set_visible(false);
    }

    small_text_generator.set_one_sprite_per_character(false);
    small_text_generator.set_bg_priority(3);
    small_text_generator.set_z_order(0);
}

pause::~pause()
{
    bn::bg_palettes::set_grayscale_intensity(0);
    bn::sprite_palettes::set_grayscale_intensity(0);
}

bool pause::update(bool& exit)
{
    exit = false;

    bool old_paused = _sprites[0].visible();
    bool new_paused = old_paused;

    if(bn::keypad::start_pressed())
    {
        _exit_selected = false;
        new_paused = ! new_paused;
    }

    if(old_paused && new_paused)
    {
        if(bn::keypad::a_pressed())
        {
            if(_exit_selected)
            {
                exit = true;
            }
            else
            {
                new_paused = false;
            }
        }
        else if(bn::keypad::b_pressed())
        {
            new_paused = false;
        }
        else if(bn::keypad::left_pressed() || bn::keypad::right_pressed())
        {
            _exit_selected = ! _exit_selected;
        }
    }

    if(old_paused != new_paused)
    {
        bn::fixed grayscale_intensity = new_paused ? 1 : 0;
        bn::bg_palettes::set_grayscale_intensity(grayscale_intensity);
        bn::sprite_palettes::set_grayscale_intensity(grayscale_intensity);

        for(bn::sprite_ptr& sprite : _sprites)
        {
            sprite.set_visible(new_paused);
        }
    }

    if(new_paused)
    {
        bn::fixed cursor_x = _exit_selected ? exit_x : continue_x;
        _sprites.back().set_x(cursor_x - 5);
    }

    return new_paused;
}

}
