#include "mj/mj_game_scene.h"

#include "bn_bg_palettes.h"
#include "bn_colors.h"
#include "bn_keypad.h"
#include "bn_sprite_affine_mat_ptr.h"
#include "bn_sprite_builder.h"
#include "bn_sprite_palettes.h"

#include "mj/mj_core.h"
#include "mj/mj_game_list.h"
#include "mj/mj_scene_type.h"

#include "bn_regular_bg_items_mj_big_pumpkin_1.h"
#include "bn_regular_bg_items_mj_big_pumpkin_2.h"
#include "bn_regular_bg_items_mj_big_pumpkin_3.h"
#include "bn_regular_bg_items_mj_big_pumpkin_4.h"
#include "bn_regular_bg_items_mj_big_pumpkin_5.h"
#include "bn_regular_bg_items_mj_big_pumpkin_6.h"
#include "bn_regular_bg_items_mj_big_pumpkin_7.h"
#include "bn_regular_bg_items_mj_big_pumpkin_8.h"
#include "bn_sprite_items_mj_font.h"
#include "bn_sprite_tiles_items_mj_pause_dialog.h"
#include "bn_sprite_tiles_items_mj_pause_dialog_lateral.h"

namespace mj
{

namespace
{
    constexpr bn::fixed continue_x = -51;
    constexpr bn::fixed exit_x = 32;
    constexpr int exit_frames = 64;
}

game_scene::game_scene(core& core) :
    _core(core),
    _data({ core.text_generator(), core.small_text_generator(), core.random(), 0 })
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
            _pause_sprites.push_back(sprite_builder.build());
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
        sprite_builder.set_x(_pause_sprites.back().x() + 20);
        _pause_sprites.push_back(sprite_builder.build());

        sprite_builder.set_horizontal_flip(false);
        sprite_builder.set_x(_pause_sprites[0].x() - 20);
        _pause_sprites.push_back(sprite_builder.release_build());
    }

    bn::sprite_text_generator& small_text_generator = _core.small_text_generator();
    small_text_generator.set_bg_priority(0);
    small_text_generator.set_z_order(-1);

    small_text_generator.set_center_alignment();
    small_text_generator.generate(0, -8, "PAUSE", _pause_sprites);
    small_text_generator.set_left_alignment();

    bn::fixed y = 7;
    small_text_generator.generate(continue_x, y, "CONTINUE", _pause_sprites);
    small_text_generator.generate(exit_x, y, "EXIT", _pause_sprites);

    small_text_generator.set_one_sprite_per_character(true);
    small_text_generator.generate(0, y, ">", _pause_sprites);

    for(bn::sprite_ptr& pause_sprite : _pause_sprites)
    {
        pause_sprite.set_visible(false);
    }

    small_text_generator.set_one_sprite_per_character(false);
    small_text_generator.set_bg_priority(3);
    small_text_generator.set_z_order(0);
}

game_scene::~game_scene()
{
    bn::bg_palettes::set_grayscale_intensity(0);
    bn::sprite_palettes::set_grayscale_intensity(0);

    bn::bg_palettes::set_fade_intensity(0);
    bn::sprite_palettes::set_fade_intensity(0);
}

bn::optional<scene_type> game_scene::update()
{
    bn::optional<scene_type> result;

    if(_exit_frames)
    {
        --_exit_frames;

        if(_exit_frames)
        {
            bn::fixed fade_intensity = 1 - (bn::fixed(_exit_frames) / exit_frames);
            bn::bg_palettes::set_fade(bn::colors::black, fade_intensity);
            bn::sprite_palettes::set_fade(bn::colors::black, fade_intensity);
        }
        else
        {
            result = scene_type::GAME;
        }
    }
    else if(! _update_pause())
    {
        if(_playing)
        {
            _data.pending_frames = _pending_frames;
            --_pending_frames;

            _update_play();
        }
        else
        {
            _data.pending_frames = 0;

            if(_update_fade())
            {
                _exit_frames = exit_frames;
            }
        }

        _update_title();
        _update_timer();
    }

    return result;
}

void game_scene::_print_info()
{
    bn::sprite_text_generator& text_generator = _core.text_generator();
    text_generator.set_bg_priority(0);
    text_generator.set_right_alignment();

    bn::fixed x = 120 - 10;
    bn::fixed y = 16 - 80;
    text_generator.generate(x, y, "Lives: " + bn::to_string<16>(_lives), _info_sprites);
    y += 16;
    text_generator.generate(x, y, "Stage: " + bn::to_string<16>(_completed_games + 1), _info_sprites);

    text_generator.set_bg_priority(3);
    text_generator.set_left_alignment();
}

void game_scene::_print_title()
{
    bn::string<16> title = _game->title();
    BN_BASIC_ASSERT(! title.empty(), "Game title is empty");

    _title_counter = 0;
    _title_sprites.clear();
    _title_sprites_x.clear();

    bn::sprite_text_generator& text_generator = _core.text_generator();
    text_generator.set_bg_priority(0);
    text_generator.set_center_alignment();
    text_generator.set_one_sprite_per_character(true);

    text_generator.generate(0, -16, _game->title(), _title_sprites);

    bn::sprite_affine_mat_ptr affine_mat = bn::sprite_affine_mat_ptr::create();
    affine_mat.set_scale(2);

    for(bn::sprite_ptr& title_sprite : _title_sprites)
    {
        title_sprite.set_affine_mat(affine_mat);
        _title_sprites_x.push_back(title_sprite.x());
    }

    text_generator.set_bg_priority(3);
    text_generator.set_left_alignment();
    text_generator.set_one_sprite_per_character(false);
}

bool game_scene::_update_pause()
{
    bool old_paused = _pause_sprites[0].visible();
    bool new_paused = old_paused;

    if(bn::keypad::start_pressed())
    {
        _exit_from_pause = false;
        new_paused = ! new_paused;
    }

    if(old_paused && new_paused)
    {
        if(bn::keypad::a_pressed())
        {
            if(_exit_from_pause)
            {
                _exit_frames = exit_frames;
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
            _exit_from_pause = ! _exit_from_pause;
        }
    }

    if(old_paused != new_paused)
    {
        bn::fixed grayscale_intensity = new_paused ? 1 : 0;
        bn::bg_palettes::set_grayscale_intensity(grayscale_intensity);
        bn::sprite_palettes::set_grayscale_intensity(grayscale_intensity);

        for(bn::sprite_ptr& pause_sprite : _pause_sprites)
        {
            pause_sprite.set_visible(new_paused);
        }
    }

    if(new_paused)
    {
        bn::fixed cursor_x = _exit_from_pause ? exit_x : continue_x;
        _pause_sprites.back().set_x(cursor_x - 5);
    }

    return new_paused;
}

void game_scene::_update_title()
{
    if(! _title_sprites.empty())
    {
        if(_title_counter)
        {
            --_title_counter;

            if(! _title_counter)
            {
                _title_sprites.clear();
            }
        }
        else
        {
            bn::sprite_affine_mat_ptr affine_mat = *_title_sprites[0].affine_mat();
            bn::fixed scale = affine_mat.horizontal_scale() - (bn::fixed(1) / 12);

            if(scale > 1)
            {
                affine_mat.set_scale(scale);

                for(int index = 0, limit = _title_sprites.size(); index < limit; ++index)
                {
                    bn::sprite_ptr& title_sprite = _title_sprites[index];
                    title_sprite.set_x(_title_sprites_x[index].unsafe_multiplication(scale));
                }
            }
            else
            {
                affine_mat.set_scale(1);
                _title_counter = 50;

                for(int index = 0, limit = _title_sprites.size(); index < limit; ++index)
                {
                    bn::sprite_ptr& title_sprite = _title_sprites[index];
                    title_sprite.set_x(_title_sprites_x[index]);
                    title_sprite.remove_affine_mat();
                }
            }
        }
    }
}

void game_scene::_update_timer()
{
    bn::sprite_text_generator& text_generator = _core.text_generator();
    text_generator.set_bg_priority(1);
    text_generator.set_z_order(-32767);

    int time = _pending_frames / 6;
    bn::string<16> time_text = bn::to_string<16>(time / 10) + '.' + bn::to_string<16>(time % 10);

    _timer_sprites.clear();
    text_generator.generate(10 - 120, 80 - 16, "Time: " + time_text, _timer_sprites);

    text_generator.set_bg_priority(3);
    text_generator.set_z_order(0);
}

void game_scene::_update_play()
{
    game_result game_result = _game->play(_data);
    _playing = ! game_result.exit && _pending_frames;

    if(! _playing || game_result.remove_title)
    {
        _title_sprites.clear();
    }

    if(! _playing)
    {
        if(_game->victory())
        {
            ++_completed_games;
        }
        else
        {
            --_lives;
        }
    }
}

bool game_scene::_update_fade()
{
    bool exit = false;

    if(_big_pumpkin_counter)
    {
        --_big_pumpkin_counter;
    }
    else
    {
        if(_big_pumpkin_inc)
        {
            if(_big_pumpkin_stage < 9)
            {
                ++_big_pumpkin_stage;
            }
            else
            {
                --_big_pumpkin_stage;
                _big_pumpkin_inc = false;
            }
        }
        else
        {
            if(_big_pumpkin_stage > 1)
            {
                --_big_pumpkin_stage;
            }
            else
            {
                if(_lives)
                {
                    ++_big_pumpkin_stage;
                    _big_pumpkin_inc = true;
                }
                else
                {
                    exit = true;
                }
            }
        }

        const bn::regular_bg_item* bg_item = nullptr;
        _big_pumpkin.reset();
        _big_pumpkin_counter = 3;

        switch(_big_pumpkin_stage)
        {

        case 1:
            bg_item = &bn::regular_bg_items::mj_big_pumpkin_1;
            _big_pumpkin_counter = 60;
            _game.reset();
            _print_info();
            break;

        case 2:
            bg_item = &bn::regular_bg_items::mj_big_pumpkin_2;

            if(_big_pumpkin_inc)
            {
                bn::span<game_list::function_type> game_list_entries = game_list::get();
                int game_list_entries_count = game_list_entries.size();
                BN_BASIC_ASSERT(game_list_entries_count, "No game list entries found");

                game_list::function_type game_list_entry =
                        game_list_entries[_core.random().get_int(game_list_entries_count)];

                game* game_ptr = game_list_entry(_completed_games, _data);
                BN_BASIC_ASSERT(game_ptr, "Game create failed");

                int total_frames = game_ptr->total_frames();
                BN_ASSERT(total_frames >= game::minimum_frames && total_frames <= game::maximum_frames,
                          "Invalid game total frames: ", total_frames);

                _pending_frames = total_frames;
                _info_sprites.clear();
                _game.reset(game_ptr);
            }
            break;

        case 3:
            bg_item = &bn::regular_bg_items::mj_big_pumpkin_3;
            break;

        case 4:
            bg_item = &bn::regular_bg_items::mj_big_pumpkin_4;
            break;

        case 5:
            bg_item = &bn::regular_bg_items::mj_big_pumpkin_5;

            if(_big_pumpkin_inc)
            {
                _print_title();
            }
            break;

        case 6:
            bg_item = &bn::regular_bg_items::mj_big_pumpkin_6;
            break;

        case 7:
            bg_item = &bn::regular_bg_items::mj_big_pumpkin_7;
            break;

        case 8:
            bg_item = &bn::regular_bg_items::mj_big_pumpkin_8;
            break;

        default:
            if(_big_pumpkin_inc)
            {
                _playing = true;
            }
            else
            {
                _big_pumpkin_counter = 60;
            }
            break;
        }

        if(bg_item)
        {
            bn::regular_bg_ptr big_pumpkin = bg_item->create_bg(0, (256 - 160) / 2);
            big_pumpkin.set_priority(0);
            _big_pumpkin = bn::move(big_pumpkin);
        }
    }

    if(! _playing)
    {
        if(game* game = _game.get())
        {
            if(_big_pumpkin_inc)
            {
                game->fade_in(_data);
            }
            else
            {
                game->fade_out(_data);

                if(_pending_frames)
                {
                    --_pending_frames;
                }
            }
        }
    }

    return exit;
}

}
