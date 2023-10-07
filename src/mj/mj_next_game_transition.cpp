#include "mj/mj_next_game_transition.h"

#include "bn_sprite_palette_ptr.h"
#include "bn_sprite_affine_mat_ptr.h"

#include "bn_sprite_items_mj_coffin.h"
#include "bn_sprite_items_mj_big_digits.h"
#include "bn_regular_bg_items_mj_coffin_light.h"

namespace mj
{

namespace
{
    constexpr bn::fixed coffin_y = 48;
    constexpr bn::fixed coffin_light_y = coffin_y;

    constexpr int stage_0_frames = 8;
    constexpr bn::fixed stage_0_initial_scale = 0.01;
    constexpr bn::fixed stage_0_scale_inc = (1 - stage_0_initial_scale) / stage_0_frames;

    constexpr int stage_1_frames = 24;

    constexpr int stage_2_frames = 48;

    constexpr int big_digit_frames = 16;
    constexpr bn::fixed big_digit_y = -6;
    constexpr bn::fixed big_digit_initial_scale = 0.01;
    constexpr bn::fixed big_digit_scale_inc = (1 - big_digit_initial_scale) / big_digit_frames;

    constexpr int stage_3_frames = 8;
    constexpr bn::fixed stage_3_scale_dec = bn::fixed(1) / stage_3_frames;
}

next_game_transition::next_game_transition(int completed_games) :
    _completed_games(completed_games)
{
    bn::sprite_affine_mat_ptr affine_mat = bn::sprite_affine_mat_ptr::create();
    affine_mat.set_scale(stage_0_initial_scale);

    bn::sprite_ptr coffin_sprite = bn::sprite_items::mj_coffin.create_sprite(0, coffin_y);
    coffin_sprite.set_bg_priority(0);
    coffin_sprite.set_z_order(-1);
    coffin_sprite.set_affine_mat(affine_mat);
    _coffin_sprites.push_back(bn::move(coffin_sprite));

    coffin_sprite = bn::sprite_items::mj_coffin.create_sprite(0, coffin_y, 1);
    coffin_sprite.set_bg_priority(0);
    coffin_sprite.set_z_order(-1);
    coffin_sprite.set_affine_mat(bn::move(affine_mat));
    _coffin_sprites.push_back(bn::move(coffin_sprite));
}

bool next_game_transition::update()
{
    switch(_stage)
    {

    case 0:
        _update_stage_0();
        return true;

    case 1:
        _update_stage_1();
        return true;

    case 2:
        _update_stage_2();
        return true;

    case 3:
        _update_stage_3();
        return true;

    default:
        return false;
    }
}

void next_game_transition::_fill_window_boundaries()
{
    constexpr int scanlines = coffin_light_y.right_shift_integer() + 80;

    bn::fixed amplitude = 1 - (bn::fixed(_counter) / stage_2_frames);
    bn::fixed min_radius = 50 * amplitude;
    bn::fixed max_radius = 120 * amplitude;
    bn::fixed radius = max_radius;
    bn::fixed radius_dec = (max_radius - min_radius) / scanlines;

    for(int index = 0; index < scanlines; ++index)
    {
        _window_boundaries[index].first = -radius;
        _window_boundaries[index].second = radius;
        radius -= radius_dec;
    }

    _light_window_hbe->reload_deltas_ref();
}

void next_game_transition::_update_stage_0()
{
    bn::sprite_affine_mat_ptr affine_mat = *_coffin_sprites[0].affine_mat();
    bn::fixed scale = affine_mat.horizontal_scale() + stage_0_scale_inc;

    if(scale < 1)
    {
        affine_mat.set_scale(scale);
    }
    else
    {
        ++_stage;

        scale = 1;

        for(bn::sprite_ptr& coffin_sprite : _coffin_sprites)
        {
            coffin_sprite.remove_affine_mat();
        }
    }

    bn::fixed x = scale * 32;
    _coffin_sprites[0].set_x(-x);
    _coffin_sprites[1].set_x(x);
}

void next_game_transition::_update_stage_1()
{
    ++_counter;

    int tiles_index = bn::min((_counter / 2) * 2, 18);
    _coffin_sprites[0].set_tiles(bn::sprite_items::mj_coffin.tiles_item(), tiles_index);
    _coffin_sprites[1].set_tiles(bn::sprite_items::mj_coffin.tiles_item(), tiles_index + 1);

    if(_counter == stage_1_frames)
    {
        ++_stage;
        _counter = 0;

        bn::regular_bg_ptr light_bg = bn::regular_bg_items::mj_coffin_light.create_bg(0, coffin_light_y);
        light_bg.set_priority(0);
        light_bg.set_blending_enabled(true);
        bn::window::outside().set_show_bg(light_bg, false);
        _light_bg = bn::move(light_bg);

        bn::rect_window internal_window = bn::rect_window::internal();
        internal_window.set_top(-80);
        internal_window.set_bottom(80);

        _light_window_hbe = bn::rect_window_boundaries_hbe_ptr::create_horizontal(internal_window, _window_boundaries);
        _fill_window_boundaries();

        int game_stage = bn::min(_completed_games + 1, 999);
        int first_digit = game_stage / 100;
        game_stage %= 100;

        if(first_digit)
        {
            _digit_sprites.push_back(bn::sprite_items::mj_big_digits.create_sprite(0, coffin_y, first_digit));
        }

        int second_digit = game_stage / 10;
        game_stage %= 10;

        if(first_digit || second_digit)
        {
            _digit_sprites.push_back(bn::sprite_items::mj_big_digits.create_sprite(0, coffin_y, second_digit));
        }

        int third_digit = game_stage;
        _digit_sprites.push_back(bn::sprite_items::mj_big_digits.create_sprite(0, coffin_y, third_digit));

        bn::sprite_affine_mat_ptr affine_mat = bn::sprite_affine_mat_ptr::create();
        affine_mat.set_scale(big_digit_initial_scale);

        for(bn::sprite_ptr& digit_sprite : _digit_sprites)
        {
            digit_sprite.set_bg_priority(0);
            digit_sprite.set_affine_mat(affine_mat);
        }
    }
}

void next_game_transition::_update_stage_2()
{
    ++_counter;
    _fill_window_boundaries();

    {
        bn::sprite_affine_mat_ptr affine_mat = *_digit_sprites[0].affine_mat();
        bn::fixed scale = bn::min(affine_mat.horizontal_scale() + big_digit_scale_inc, bn::fixed(1));
        affine_mat.set_scale(scale);

        bn::fixed x = -(_digit_sprites.size() - 1) * 24 * scale;
        bn::fixed y = coffin_y + ((big_digit_y - coffin_y).unsafe_multiplication(scale));

        for(bn::sprite_ptr& digit_sprite : _digit_sprites)
        {
            digit_sprite.set_position(x, y);
            x += 48 * scale;
        }

        bn::sprite_palette_ptr palette = _digit_sprites[0].palette();
        palette.set_fade(bn::color(31, 31, 0), 1 - scale);

        bn::fixed desp_x;
        bn::fixed desp_y;

        if(scale < 1)
        {
            switch(_counter % 4)
            {

            case 0:
                desp_x = -1;
                desp_y = 1;
                break;

            case 1:
                desp_x = 1;
                break;

            case 2:
                desp_y = -1;
                break;

            default:
                desp_x = 1;
                break;
            }
        }

        _coffin_sprites[0].set_position(-32 + desp_x, coffin_y + desp_y);
        _coffin_sprites[1].set_position(32 + desp_x, coffin_y + desp_y);
    }

    if(_counter == stage_2_frames)
    {
        ++_stage;
        _counter = 0;

        bn::sprite_affine_mat_ptr affine_mat = bn::sprite_affine_mat_ptr::create();
        _coffin_sprites[0].set_affine_mat(affine_mat);
        _coffin_sprites[1].set_affine_mat(bn::move(affine_mat));

        _light_bg.reset();
        _light_window_hbe.reset();
    }
}

void next_game_transition::_update_stage_3()
{
    bn::sprite_affine_mat_ptr affine_mat = *_coffin_sprites[0].affine_mat();
    bn::fixed scale = affine_mat.vertical_scale() - stage_3_scale_dec;

    if(scale > 0)
    {
        affine_mat.set_vertical_scale(scale);

        affine_mat = *_digit_sprites[0].affine_mat();
        affine_mat.set_scale(scale);
    }
    else
    {
        ++_stage;

        _coffin_sprites.clear();
        _digit_sprites.clear();
    }
}

}
