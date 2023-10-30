#include "fixxg1_game.h"

#include "bn_keypad.h"

#include "mj/mj_game_list.h"

#include "bn_math.h"
#include "bn_regular_bg_items_fixxg1_bg.h"
#include "bn_sprite_items_fixxg1_club.h"
#include "bn_sprite_items_fixxg1_ball.h"


namespace
{
    constexpr bn::string_view code_credits[] = { "fixx" };
    constexpr bn::string_view graphics_credits[] = { "fixx" };

    bn::fixed_point rotate_point(bn::fixed angle, bn::fixed_point p)
    {
        bn::fixed s= degrees_cos(angle);
        bn::fixed c= degrees_sin(angle);

        //translate point back to origin
        //p.set_x(p.x()-pivot.x());
        //p.set_y(p.y()-pivot.y());

        // rotate point
        bn::fixed xnew = p.y() * c - p.x() * s;
        bn::fixed ynew = p.y() * s + p.x() * c;

        // translate point back
        //return bn::fixed_point(xnew + pivot.x(), ynew + pivot.y());
        return bn::fixed_point(xnew,ynew);
    }

    void rotate_sprite_around(bn::sprite_ptr sprite,bn::fixed_point position,bn::fixed_point rotation_offset,bn::fixed rotation)
    {
        
        sprite.set_position(position - rotate_point(rotation,rotation_offset));
        sprite.set_rotation_angle(rotation);
    }
}

// MJ_GAME_LIST_ADD(fixxg1::game)
MJ_GAME_LIST_ADD_CODE_CREDITS(code_credits)
MJ_GAME_LIST_ADD_GRAPHICS_CREDITS(graphics_credits)
// MJ_GAME_LIST_ADD_MUSIC_CREDITS(music_credits)
// MJ_GAME_LIST_ADD_SFX_CREDITS(sfx_credits)

namespace fixxg1
{

game::game(int completed_games, const mj::game_data& data) :
    _bg(bn::regular_bg_items::fixxg1_bg.create_bg((256 - 240) / 2, (256 - 160) / 2)),
    _spr_club(bn::sprite_items::fixxg1_club.create_sprite(0,0)),
    _spr_ball(bn::sprite_items::fixxg1_ball.create_sprite(0,-160-16)),
    _total_frames(play_jingle(mj::game_jingle_type::TOTSNUK01, completed_games, data))
{
    switch (recommended_difficulty_level(completed_games, data)) {
        default:
        case mj::difficulty_level::EASY:
            _ball_velocity=bn::fixed_point(0,0);
            _ball_fall_delay=30;
            break;
        case mj::difficulty_level::NORMAL:
            _ball_velocity=bn::fixed_point(0,2);
            _ball_fall_delay=20;
            break;
        case mj::difficulty_level::HARD:
            _ball_velocity=bn::fixed_point(0,3);
            _ball_fall_delay=15;
            break;
    }
    rotate_sprite_around(_spr_club,_club_position,_club_rotation_offset,_club_rotation);
}

void game::fade_in([[maybe_unused]] const mj::game_data& data)
{
}

mj::game_result game::play(const mj::game_data& data)
{
    mj::game_result result;
    result.exit = data.pending_frames == 0;

    if(_ball_falling)
    {
        _ball_velocity.set_y(_ball_velocity.y()+_ball_gravity);
        _spr_ball.set_position(_spr_ball.position()+_ball_velocity);
        if(!_victory && _spr_ball.position().y() > 160/2)
        {
            _defeat=true;
            result.remove_title = true;
        }
        if(_spr_ball.position().x()<-(240/2)-16)
        {
            result.exit = true;
        }
    }
    else
    {
        _ball_fall_delay--;
        if(_ball_fall_delay<=0)
            _ball_falling=true;
    }

    

    if(_swinging)
    {
        if(_swing_frames>0)
        {
            _swing_frames--;
            _club_rotation +=10;
            if(_club_rotation > bn::fixed(360))
                _club_rotation-=360;
        }
    }
    else
    {

        
        //bn::fixed ballY=_spr_ball.position().y();
        if(bn::keypad::a_pressed() || bn::keypad::b_pressed())
        {
            _swinging=true;
            bn::fixed ballY=_spr_ball.position().y();
            if(ballY >= -24 && ballY <= -3)
            {
                if(!_defeat)
                    _victory=true;

                result.remove_title = true;
                _ball_velocity=bn::fixed_point(-5,-2);
            }
        }
        
    }

    rotate_sprite_around(_spr_club,_club_position,_club_rotation_offset,_club_rotation);

    if(_defeat)
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

void game::fade_out([[maybe_unused]] const mj::game_data& data)
{
}

}
