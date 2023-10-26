#include "fixxg2_game.h"

#include "bn_fixed_point.h"
#include "bn_keypad.h"
#include "bn_math.h"

#include "mj/mj_game_list.h"

#include "bn_regular_bg_items_fixxg2_bg.h"
#include "bn_affine_bg_items_fixxg2_zombie.h"
#include "bn_affine_bg_items_fixxg2_zombie_dead.h"
#include "bn_sprite_items_fixxg2_crosshair.h"
#include "bn_sound_items.h"

namespace
{
    constexpr bn::string_view code_credits[] = { "fixx" };
    constexpr bn::string_view graphics_credits[] = { "fixx" };
    constexpr bn::string_view sfx_credits[] = { "MATRIXXX_" };


    bn::fixed_point calc_zombie_pos(int elapsed_frames)
    {
        bn::fixed x = bn::degrees_cos(elapsed_frames)*60;
        bn::fixed y = bn::degrees_sin(elapsed_frames*3)*20 +40;

        return bn::fixed_point(x,y);
    }

    bn::fixed_point calc_crosshair_pos(bn::fixed elapsed_frames)
    {
        bn::fixed x = bn::degrees_cos(elapsed_frames*1.5+120)*60;
        bn::fixed y = bn::degrees_sin(elapsed_frames*6)*50;

        return bn::fixed_point(x,y);
    }

    

}

// MJ_GAME_LIST_ADD(fixxg2::game)
MJ_GAME_LIST_ADD_CODE_CREDITS(code_credits)
MJ_GAME_LIST_ADD_GRAPHICS_CREDITS(graphics_credits)
// MJ_GAME_LIST_ADD_MUSIC_CREDITS(music_credits)
MJ_GAME_LIST_ADD_SFX_CREDITS(sfx_credits)

namespace fixxg2
{

game::game(int completed_games, const mj::game_data& data) :
    _bg(bn::regular_bg_items::fixxg2_bg.create_bg((256 - 240) / 2, (256 - 160) / 2)),
    _zombie(bn::affine_bg_items::fixxg2_zombie.create_bg(calc_zombie_pos(0))),
    _crosshair(bn::sprite_items::fixxg2_crosshair.create_sprite(calc_crosshair_pos(0))),
    _total_frames(play_jingle(mj::game_jingle_type::TOTSNUK04A, completed_games, data))
{
    _zombie.set_priority(2);
    _zombie.set_wrapping_enabled(false);
    _crosshair.set_bg_priority(1);
    
    switch (recommended_difficulty_level(completed_games, data)) {
        default:
        case mj::difficulty_level::EASY:
            _crosshair_speed_multiplier=1;
            break;
        case mj::difficulty_level::NORMAL:
            _crosshair_speed_multiplier=1.5;
            break;
        case mj::difficulty_level::HARD:
            _crosshair_speed_multiplier=2;
            break;
    }
}

void game::fade_in([[maybe_unused]] const mj::game_data& data)
{
}

bool game::does_crosshair_hit_zombie()
    {
        bn::fixed max_dist = 36;
        bn::fixed_point to_zombie = _zombie.position() - _crosshair.position();
        return (to_zombie.x() * to_zombie.x() + to_zombie.y() * to_zombie.y()) <= max_dist * max_dist;
    }

mj::game_result game::play(const mj::game_data& data)
{
    mj::game_result result;
    result.exit = data.pending_frames == 0;

    //Zombie moving
    if(_victory)
    {
        _zombie.set_y(_zombie.y()+1);
    }
    else
    {
        _zombie.set_position(calc_zombie_pos(_elapsed_frames));
        _crosshair.set_position(calc_crosshair_pos(_crosshair_speed_multiplier * _elapsed_frames));
    }
    
    /*if(does_crosshair_hit_zombie())
    {
        _crosshair.set_scale(2);
    }
    else
    {
        _crosshair.set_scale(1);
    }*/

    if(! _victory && ! _defeat)
    {
        if(bn::keypad::a_pressed() || bn::keypad::b_pressed())
        {
            bn::sound_items::fixxg2_shotgun_shot.play(0.8);
            if(does_crosshair_hit_zombie())
            {
                _zombie.set_item(bn::affine_bg_items::fixxg2_zombie_dead);
                _victory = true;
            }
            else
            {
                _defeat=true;
            }
            _crosshair.set_visible(false);
            result.remove_title = true;
            
        }
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

    _elapsed_frames++;
    return result;
}

void game::fade_out([[maybe_unused]] const mj::game_data& data)
{
}

}
