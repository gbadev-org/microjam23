#include "row_game.h"

#include "bn_keypad.h"

#include "mj/mj_game_list.h"

#include "bn_sprite_items_row_l_button.h"
#include "bn_sprite_items_row_r_button.h"
#include "row_fishy_animation.h"
#include "row_rower_animation.h"
#include "row_water_animation.h"

namespace
{
    constexpr bn::string_view code_credits[] = { "Noah Nelson" };
    constexpr bn::string_view graphics_credits[] = { "asterlil", "jgilhutton", "Noah Nelson" };
}

// MJ_GAME_LIST_ADD(row::game)
MJ_GAME_LIST_ADD_CODE_CREDITS(code_credits)
MJ_GAME_LIST_ADD_GRAPHICS_CREDITS(graphics_credits)
// MJ_GAME_LIST_ADD_MUSIC_CREDITS(music_credits)
// MJ_GAME_LIST_ADD_SFX_CREDITS(sfx_credits)

namespace row
{

game::game([[maybe_unused]] int completed_games, [[maybe_unused]] const mj::game_data& data)
    : _rower(rower_animation::frames[0].create_bg(0, 0))
    , _water(water_animation::frames[0].create_bg(0, 0))
    , _fishy(fishy_animation::frames[0].create_bg(0, 0))
    , _l_button(bn::sprite_items::row_l_button.create_sprite(70, -5))
    , _r_button(bn::sprite_items::row_r_button.create_sprite(70, -5))
    , _total_frames(play_jingle(mj::game_jingle_type::EXELOTL02B, completed_games, data))
{
    _fish_velocity = compute_fish_velocity(_total_frames);

    _rower.set_priority(2);
    _fishy.set_priority(1);
    _current_state = stroked;
    _l_button.set_visible(true);
    _r_button.set_visible(false);
    _l_button.set_bg_priority(1);
    _r_button.set_bg_priority(1);
}

void game::fade_in([[maybe_unused]] const mj::game_data& data)
{
}

static const unsigned END_STROKE_FRAME = 30;
static const unsigned END_WIND_FRAME = 20;

static const bn::fixed stroke_acceleration = 0.65;
static const bn::fixed velocity_decay = 0.99;


static constexpr bn::fixed _fish_max_velocity = 0.30;
static constexpr bn::fixed _fish_min_velocity = 0.11;

bn::fixed game::compute_fish_velocity(int frames)
{
    bn::fixed a = (_fish_max_velocity - _fish_min_velocity) / (minimum_frames - maximum_frames);
    bn::fixed b = (_fish_max_velocity - a * minimum_frames);

    return a * frames + b;
}

bool game::handle_input()
{
    switch (_current_state)
    {
        case stroking:
            _current_row_frame++;
            if (_current_row_frame == END_STROKE_FRAME) {
                _current_state = stroked;
                _l_button.set_visible(true);
            }
            break;
        case winding:
            _current_row_frame++;
            if (_current_row_frame == END_WIND_FRAME) {
                _current_state = wound;
                _r_button.set_visible(true);
            }
            break;
        case stroked:
            if(bn::keypad::l_pressed()) {
                _l_button.set_visible(false);
                _current_state = winding;
            }
            break;
        case wound:
            if(bn::keypad::r_pressed()) {
                _r_button.set_visible(false);
                _velocity = stroke_acceleration;
                _current_state = stroking;
            }
            break;
        default:
            BN_ASSERT(false);
    }

    return false;
}

constexpr bn::fixed initial_fish_distance = 35;
constexpr unsigned FISHY_FRAMES_PRE_EAT = 185 - 90;
constexpr unsigned FISHY_FRAMES_CLEAR_BOAT = 208 - 90;
constexpr unsigned FISHY_FRAMES_PRE_SINK = 229 - 90;

mj::game_result game::play(const mj::game_data& data)
{
    mj::game_result result;
    result.exit = data.pending_frames == 0;
    // XXX odd - we never actually get passed pending_frames == 0
    if (data.pending_frames == 1 && !_defeat) {
        _victory = true;
    }

    if(!(_victory || _defeat))
    {
        _position += _velocity;
        _velocity *= velocity_decay;
        _fish_position += _fish_velocity;
        
        handle_input();
        _current_row_frame %= rower_animation::N_FRAMES;
        _rower.set_item(rower_animation::frames[_current_row_frame]);
        unsigned water_frame = ((_position * 2).round_integer()) % water_animation::N_FRAMES;
        _water.set_item(water_animation::frames[water_frame]);
        bn::fixed fishy_distance = bn::max(_position - _fish_position + initial_fish_distance, bn::fixed(0));
        bn::fixed numerator = bn::max(initial_fish_distance - fishy_distance, bn::fixed(0));
        bn::fixed ratio = numerator / initial_fish_distance; 
        unsigned fishy_frame = (ratio * FISHY_FRAMES_PRE_EAT).round_integer();
        _fishy.set_item(fishy_animation::frames[fishy_frame]);
        if (fishy_frame == FISHY_FRAMES_PRE_EAT) {
            _defeat = true;
            _show_result_frames = 45;
            _eating_fishy_frame = FISHY_FRAMES_PRE_EAT;
        }
    }
    else
    {
        if(_show_result_frames && _defeat)
        {
            _eating_fishy_frame++;
            if (_eating_fishy_frame == FISHY_FRAMES_CLEAR_BOAT) {
                _rower.set_visible(false);
                _l_button.set_visible(false);
                _r_button.set_visible(false);
            }
            _eating_fishy_frame = std::min(_eating_fishy_frame, FISHY_FRAMES_PRE_SINK);
            _fishy.set_item(fishy_animation::frames[_eating_fishy_frame]);
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
