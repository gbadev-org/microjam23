#include "fixxg3_game.h"

#include "bn_keypad.h"
#include "bn_math.h"


#include "mj/mj_game_list.h"


#include "bn_regular_bg_items_fixxg3_bg.h"
#include "bn_sprite_items_fixxg3_club.h"
#include "bn_sprite_items_fixxg3_zombie_head.h"
#include "bn_sound_items.h"

namespace
{
    constexpr bn::string_view code_credits[] = { "fixx" };
    constexpr bn::string_view graphics_credits[] = { "fixx" };
    constexpr bn::string_view sfx_credits[] = {"Danjocross", "thecrow_br"};

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

    bn::sprite_animate_action<5> _create_animation_head_up(const bn::sprite_ptr& sprite)
    {
        return bn::sprite_animate_action<5>::once(sprite, 2, bn::sprite_items::fixxg3_zombie_head.tiles_item(), bn::array<uint16_t, 5>{{4,3,2,1,0}});
    }

    bn::sprite_animate_action<6> _create_animation_head_down(const bn::sprite_ptr& sprite)
    {
        return bn::sprite_animate_action<6>::once(sprite, 2, bn::sprite_items::fixxg3_zombie_head.tiles_item(), bn::array<uint16_t, 6>{{0,1,2,3,4,10}});
    }

    bn::sprite_animate_action<6> _create_animation_head_clubbed_down(const bn::sprite_ptr& sprite)
    {
        return bn::sprite_animate_action<6>::once(sprite, 8, bn::sprite_items::fixxg3_zombie_head.tiles_item(), bn::array<uint16_t, 3>{{5,6,7}});
    }

    bn::sprite_animate_action<6> _create_animation_head_hiding(const bn::sprite_ptr& sprite)
    {
        return bn::sprite_animate_action<6>::once(sprite, 2, bn::sprite_items::fixxg3_zombie_head.tiles_item(), bn::array<uint16_t, 2>{{10,10}});
    }

    bool is_in_range(bn::fixed_point a, bn::fixed_point b, bn::fixed radius)
    {
        bn::fixed_point diff = b-a;
        return diff.x() * diff.x() + diff.y() * diff.y() <= radius * radius;
    }

}

// MJ_GAME_LIST_ADD(fixxg3::game)
MJ_GAME_LIST_ADD_CODE_CREDITS(code_credits)
MJ_GAME_LIST_ADD_GRAPHICS_CREDITS(graphics_credits)
// MJ_GAME_LIST_ADD_MUSIC_CREDITS(music_credits)
MJ_GAME_LIST_ADD_SFX_CREDITS(sfx_credits)

namespace fixxg3
{

game::game(int completed_games, const mj::game_data& data) :
    _bg(bn::regular_bg_items::fixxg3_bg.create_bg((256 - 240) / 2, (256 - 160) / 2)),
    _spr_club(bn::sprite_items::fixxg3_club.create_sprite(0,0)),
    _zombie_head(bn::sprite_items::fixxg3_zombie_head.create_sprite(0,0)),
    _zombie_head_action(_create_animation_head_hiding(_zombie_head)),
    _total_frames(play_jingle(mj::game_jingle_type::TOTSNUK08, completed_games, data))
{

    switch (recommended_difficulty_level(completed_games, data)) {
        default:
        case mj::difficulty_level::EASY:
        //BN_LOG("easy");
            _max_up_frames = 60;
            break;
        case mj::difficulty_level::NORMAL:
        //BN_LOG("normal");
            _max_up_frames = 45;
            break;
        case mj::difficulty_level::HARD:
        //BN_LOG("hard");
            _max_up_frames = 35;
            break;
    }

    _zombie_head.set_z_order(2);
    _zombie_head_action.update();
    _zombie_pos_index = data.random.get_int() % 9;
    zombie_appear(_zombie_pos_index);
    
    rotate_sprite_around(_spr_club,_club_position,_club_rotation_offset,_club_rotation);
}

bool game::does_club_hit_pos(bn::fixed_point pos)
{
    bn::fixed_point hitPos = _club_position + _club_hit_offset;
    return is_in_range(hitPos,pos,_club_hit_tolerance);
}

void game::zombie_appear(int posIndex)
{
    _zombie_head.set_position(_possible_zombie_locations[posIndex]);
    _zombie_head.set_scale(_possible_zombie_scales[posIndex]);
    _zombie_head_action = _create_animation_head_up(_zombie_head);
    _zombie_state=zombie_state::MOVING_UP;
}

void game::fade_in([[maybe_unused]] const mj::game_data& data)
{
}

mj::game_result game::play(const mj::game_data& data)
{
    mj::game_result result;
    result.exit = data.pending_frames == 0;

    switch(_club_swingstate)
    {
        default:
        case club_state::UP:
        {
            if(bn::keypad::a_pressed() || bn::keypad::b_pressed())
            {
                _club_swingstate=club_state::SWING_DOWN;
                bn::sound_items::fixxg3_swoosh.play(0.8);
                //BN_LOG("club pos: ",_club_position.x()," ",_club_position.y());
            }
            
            if(bn::keypad::left_held())
            {
                _club_position.set_x(max(_club_min_x, _club_position.x() - _club_move_speed));
            }

            if(bn::keypad::right_held())
            {
                _club_position.set_x(min(_club_max_x, _club_position.x() + _club_move_speed));
            }

            if(bn::keypad::up_held())
            {
                _club_position.set_y(max(_club_min_y, _club_position.y() - _club_move_speed));
            }

            if(bn::keypad::down_held())
            {
                _club_position.set_y(min(_club_max_y, _club_position.y() + _club_move_speed));
            }

        }break;
        case club_state::SWING_DOWN:
        {
            _club_rotation+=_club_swing_speed;
            if(_club_rotation >= _club_down_rotation)
            {
                _club_rotation = _club_down_rotation;
                _club_swingstate = club_state::SWING_UP;

                if((_zombie_state == zombie_state::UP || _zombie_state == zombie_state::MOVING_UP
                || (_zombie_state==zombie_state::MOVING_DOWN && _zombie_down_frames>0)) 
                && does_club_hit_pos(_zombie_head.position()))
                {
                    _zombie_head_action=_create_animation_head_clubbed_down(_zombie_head);
                    _zombie_state=zombie_state::CLUBBING_DOWN;
                    bn::sound_items::fixxg3_club.play(0.8);
                    _victory=true;
                }
            }
        }break;
        case club_state::SWING_UP:
        {
            _club_rotation -= _club_swing_speed;
            if(_club_rotation <= _club_up_rotation)
            {
                _club_rotation=_club_up_rotation;
                _club_swingstate=club_state::UP;
            }
        }break;
    }

    rotate_sprite_around(_spr_club,_club_position,_club_rotation_offset,_club_rotation);

    /*
    if(does_club_hit_pos(_zombie_head.position()))
    {
        _spr_club.set_scale(1.3);
    }
    else
    {
        _spr_club.set_scale(1);
    }
    */


    if(!_zombie_head_action.done())
    {
        _zombie_head_action.update();
        if(_zombie_state==zombie_state::MOVING_DOWN)
        {
            _zombie_down_frames--;
        }
    }
    else
    {
        //_bg.set_visible(false);
        switch(_zombie_state)
        {
            case zombie_state::MOVING_UP:
            {
                _zombie_state=zombie_state::UP;
                _up_frames=_max_up_frames;
            }break;
            case zombie_state::UP:
            {
                _up_frames--;
                if(_up_frames<=0)
                {
                    _zombie_head_action = _create_animation_head_down(_zombie_head);
                    _zombie_state=zombie_state::MOVING_DOWN;
                    _zombie_down_frames=_max_zombie_down_frames;
                }
            }break;
            case zombie_state::MOVING_DOWN:
            {
                _zombie_state=zombie_state::HIDING;
                _hide_frames = _max_hide_frames;
            }break;
            case zombie_state::HIDING:
            {
                _hide_frames--;
                if(_hide_frames<=0)
                {
                    _zombie_pos_index = data.random.get_int() % 9;
                    zombie_appear(_zombie_pos_index);
                }
            }break;
            case zombie_state::CLUBBING_DOWN:
            default:
            {
            }break;
        }
    }

    

    if(_victory)
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
