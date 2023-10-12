#include "mj/mj_game_over_scene.h"

#include "bn_blending.h"
#include "bn_fixed_point.h"
#include "bn_keypad.h"
#include "bn_string.h"

#include "mj/mj_core.h"
#include "mj/mj_scene_type.h"

#include "bn_regular_bg_items_mj_game_over_backdrop.h"
#include "bn_sound_items.h"
#include "bn_sprite_items_mj_font.h"
#include "bn_sprite_palette_items_mj_big_font_highlight.h"

namespace mj
{

game_over_scene::game_over_scene(int completed_games, core& core) :
    _backdrop(bn::regular_bg_items::mj_game_over_backdrop.create_bg(0, 0)),
    _old_blending_intensity(bn::blending::intensity_alpha()),
    _affine_mat(bn::sprite_affine_mat_ptr::create())
{
    sram_data& sram_data = core.sram_data();
    sram_data.add_high_score(completed_games);
    sram_data.write();

    _backdrop.set_priority(0);
    _backdrop.set_blending_enabled(true);
    bn::blending::set_intensity_alpha(0);

    const bn::sprite_palette_item& regular_palette = bn::sprite_items::mj_font.palette_item();
    const bn::sprite_palette_item& highlight_palette = bn::sprite_palette_items::mj_big_font_highlight;
    bn::sprite_text_generator& text_generator = core.big_text_generator();
    text_generator.set_bg_priority(0);
    text_generator.set_center_alignment();

    text_generator.generate(0, -60, "HIGH SCORE", _text_sprites);

    bn::span<const int> high_scores = sram_data.high_scores();
    text_generator.set_palette_item(completed_games == high_scores[0] ? highlight_palette : regular_palette);
    text_generator.generate(0, -20, "1ST  " + bn::to_string<16>(high_scores[0]), _text_sprites);
    text_generator.set_palette_item(completed_games == high_scores[1] ? highlight_palette : regular_palette);
    text_generator.generate(0, 0,   "2ND  " + bn::to_string<16>(high_scores[1]), _text_sprites);
    text_generator.set_palette_item(completed_games == high_scores[2] ? highlight_palette : regular_palette);
    text_generator.generate(0, 20,  "3RD  " + bn::to_string<16>(high_scores[2]), _text_sprites);

    text_generator.set_palette_item(regular_palette);
    text_generator.generate(0, 60, "(A) RETRY    (B) EXIT", _text_sprites);

    text_generator.set_bg_priority(3);
    text_generator.set_left_alignment();

    _affine_mat.set_vertical_scale(0.01);

    for(bn::sprite_ptr& text_sprite : _text_sprites)
    {
        text_sprite.set_affine_mat(_affine_mat);
    }
}

game_over_scene::~game_over_scene()
{
    bn::blending::set_intensity_alpha(_old_blending_intensity);
}

bn::optional<scene_type> game_over_scene::update()
{
    bn::optional<scene_type> result;
    update_gfx();

    if(_fade_in)
    {
        bn::fixed vertical_scale = _affine_mat.vertical_scale() + (bn::fixed(1) / 16);

        if(vertical_scale >= 1)
        {
            vertical_scale = 1;
            _fade_in = false;
        }

        _affine_mat.set_vertical_scale(vertical_scale);
    }
    else if(_next_scene)
    {
        bn::fixed vertical_scale = _affine_mat.vertical_scale() - (bn::fixed(1) / 16);

        if(vertical_scale > 0)
        {
            _affine_mat.set_vertical_scale(vertical_scale);
        }
        else
        {
            _text_sprites.clear();
            result = _next_scene;
        }
    }
    else if(bn::keypad::a_pressed())
    {
        _next_scene = scene_type::GAME;
        bn::sound_items::mj_pause_end.play();
    }
    else if(bn::keypad::b_pressed())
    {
        _next_scene = scene_type::TITLE;
        bn::sound_items::mj_pause_exit.play();
    }

    return result;
}

void game_over_scene::update_gfx()
{
    ++_update_gfx_counter;

    if(_update_gfx_counter % 2)
    {
        _backdrop.set_position(_backdrop.position() + bn::fixed_point(1, -1));
    }
}

}
