#include "wwtf_wwtf_game.h"

#include "bn_keypad.h"

#include "mj/mj_game_list.h"

#include "bn_regular_bg_items_wwtf_background.h"
#include "bn_sound_items.h"

namespace
{
constexpr bn::string_view code_credits[] = { "RiPpEr253" };
constexpr bn::string_view graphics_credits[] = { "RiPpEr253" };
constexpr bn::string_view sfx_credits[] = { "RiPpEr253" };
}

MJ_GAME_LIST_ADD(wwtf::wwtf_game)
MJ_GAME_LIST_ADD_CODE_CREDITS(code_credits)
MJ_GAME_LIST_ADD_GRAPHICS_CREDITS(graphics_credits)
// MJ_GAME_LIST_ADD_MUSIC_CREDITS(music_credits)
MJ_GAME_LIST_ADD_SFX_CREDITS(sfx_credits)

namespace wwtf
{

wwtf_game::wwtf_game(int completed_games, const mj::game_data& data) :
    mj::game("wwtf"),
    _bg(bn::regular_bg_items::wwtf_background.create_bg((256 - 240) / 2, (256 - 160) / 2)),
    _total_frames(play_jingle(mj::game_jingle_type::TOTSNUK11, completed_games, data))
{
    /*constexpr int frames_diff = maximum_frames - minimum_frames;
    constexpr int maximum_speed_completed_games = 30;

    completed_games = bn::min(completed_games, maximum_speed_completed_games);

    int frames_reduction = (frames_diff * completed_games) / maximum_speed_completed_games;
    _total_frames = maximum_frames - frames_reduction;
    _total_frames -= data.random.get_int(60);
    _total_frames = bn::clamp(_total_frames, minimum_frames, maximum_frames);*/

    _total_frames = (_total_frames * bn::fixed(0.6)).right_shift_integer();

    //set sprite starting positions and attributes
    _cloudsprite1.set_horizontal_flip(true);
    _cloudsprite3.set_horizontal_flip(true);

    //grab a random number for the game length adjustment
    _game_time_offset = data.random.get_int(60);
    _game_time_offset -= 40; // split random number almost down the middle for about half a second either side

    //setup text generator and text sprites
    bn::sprite_text_generator _local_text_generator = data.big_text_generator;
    _local_text_generator.set_center_alignment();
    _local_text_generator.set_one_sprite_per_character(false);
    _local_text_generator.generate(0, 0, _you_win_label, _win_text_sprites);
    _local_text_generator.generate(0, 0, _you_lose_label, _lose_text_sprites);
    _local_text_generator.generate(0, 0, _you_lose_label_moon, _lose_text_sprites_moon);

    for(bn::sprite_ptr& text_sprite : _win_text_sprites)
    {
        text_sprite.set_visible(false);
    }

    for(bn::sprite_ptr& text_sprite : _lose_text_sprites)
    {
        text_sprite.set_visible(false);
    }

    for(bn::sprite_ptr& text_sprite : _lose_text_sprites_moon)
    {
        text_sprite.set_visible(false);
    }
}

void wwtf_game::fade_in([[maybe_unused]] const mj::game_data& data)
{
}

mj::game_result wwtf_game::play(const mj::game_data& data)
{
    mj::game_result result;
    result.exit = data.pending_frames == 0;

    _elapsed_frames++;

    if(!_animationaction.done())
    {
        _animationaction.update();
    }

    if(! _victory && ! _defeat)
    {
        if(bn::keypad::a_pressed() && _victoryallowed)
        {
            result.remove_title = true;
            _victory = true;

            _animationaction = bn::create_sprite_animate_action_once(_werewolftransformation, 1,
                bn::sprite_items::wwtf_werewolf_transformation.tiles_item(), 4, 5, 6, 7, 8, 9, 10, 11, 12, 13 ,14 ,15 ,16 ,17 ,18 ,19);

            _strugglesoundhandle.stop();

            bn::sound_items::wwtf_awoo.play(0.5);

            for(bn::sprite_ptr& text_sprite : _win_text_sprites)
            {
                text_sprite.set_visible(true);
            }
        }
        else if(! bn::keypad::start_pressed() && bn::keypad::any_pressed())
        {
            // figure out reason for losing and display appropriate text
            if(bn::keypad::a_pressed())
            {
                for(bn::sprite_ptr& text_sprite : _lose_text_sprites_moon)
                {
                    text_sprite.set_visible(true);
                }
            }
            else
            {
                for(bn::sprite_ptr& text_sprite : _lose_text_sprites)
                {
                    text_sprite.set_visible(true);
                }
            }

            result.remove_title = true;
            _defeat = true;
        }

        // check to see if we should start moving clouds
        // 75 frames + random number of frames(-30..30)
        if(_elapsed_frames > (75 + _game_time_offset))
        {
            _cloudsmoving = true;
            _victoryallowed = true;
        }

        // if clouds have fully revealed the moon, stop moving
        if(_cloudsprite1.x() < -60)
        {
            _cloudsmoving = false;
        }

        // perform cloud movement
        if(_cloudsmoving)
        {
            _cloudsprite1.set_position(_cloudsprite1.x() - 1, _cloudsprite1.y());
            _cloudsprite2.set_position(_cloudsprite2.x() - 2, _cloudsprite2.y());
            _cloudsprite3.set_position(_cloudsprite3.x() + 1, _cloudsprite3.y());
            _cloudsprite4.set_position(_cloudsprite4.x() + 2, _cloudsprite4.y());
        }
        // animate the cloud sprites somehow
        //_cloudsprite1.set_position(_cloudsprite1.x(), (_cloudsprite1.y() + (bn::degrees_lut_cos(_elapsed_frames/2) /100)));

    }
    else
    {
        if(_show_result_frames)
        {
            --_show_result_frames;
        }
        else
        {
            result.exit = true;
        }
    }

    return result;
}

void wwtf_game::fade_out([[maybe_unused]] const mj::game_data& data)
{
}

}
