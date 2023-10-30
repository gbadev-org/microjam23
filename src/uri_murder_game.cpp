#include "uri_murder_game.h"

#include "bn_keypad.h"
#include "bn_fixed_point.h"

#include "mj/mj_game_list.h"

#include "bn_sprite_items_uri_char_1_alt.h"
#include "bn_sprite_items_uri_char_2_alt.h"
#include "bn_sprite_items_uri_char_3_alt.h"
#include "bn_regular_bg_items_uri_end_lose.h"
#include "bn_regular_bg_items_uri_end_win.h"
#include "bn_regular_bg_items_uri_detention_room_bg.h"

namespace
{
    constexpr bn::string_view code_credits[] = { "Jono Shields" };
    constexpr bn::string_view graphics_credits[] = { "Uri" };
}

// MJ_GAME_LIST_ADD(uri::murder_game)
MJ_GAME_LIST_ADD_CODE_CREDITS(code_credits)
MJ_GAME_LIST_ADD_GRAPHICS_CREDITS(graphics_credits)

namespace uri
{

namespace
{
    bn::fixed_point get_arrow_position(int position){
        int x = (position) * 60 + -90;

        return bn::fixed_point(x,-60);
    }
}

murder_game::murder_game(int completed_games, const mj::game_data& data):
    _total_frames(play_jingle(mj::game_jingle_type::TOTSNUK07, completed_games, data))
{
    
}

void murder_game::fade_in([[maybe_unused]] const mj::game_data& data)
{
    if(!_is_setup){
        _front_bg.set_priority(1);
        _select_arrow.set_vertical_flip(true);
        _killer = data.random.get_int(3);
        _position = 0;

        if(_killer == 0){
            _char_1 = bn::sprite_items::uri_char_1_alt;
        } else if (_killer == 1){
            _char_2 = bn::sprite_items::uri_char_2_alt;
        } else {
            _char_3 = bn::sprite_items::uri_char_3_alt;
        }
        _is_setup = true;
    }
}

mj::game_result murder_game::play(const mj::game_data& data)
{
    mj::game_result result;
    result.exit = data.pending_frames == 0;

    if(_is_game_over){
        if(_position > 60){
            result.exit = true;
        } else {
            _position += 1;
        }
        return result;
    } else {
        _bg = bn::regular_bg_items::uri_detention_room_bg.create_bg(0,0);
        
    }
    
    if(!zoom_action.done()){
        zoom_action.update();
        move_action.update();
    }
    if(bn::keypad::left_pressed()){
        _position -= 1;
        if(_position < 0){
            _position = 3;
        }
    }
    if(bn::keypad::right_pressed()){
        _position += 1;
        if(_position > 3){
            _position = 0;
        }
    }
    if(!_is_game_over && (bn::keypad::a_pressed() || bn::keypad::b_pressed())){
        _is_game_over = true;
        _char_1_top.set_visible(false);
        _char_1_bottom.set_visible(false);
        _char_2_top.set_visible(false);
        _char_2_bottom.set_visible(false);
        _char_3_top.set_visible(false);
        _char_3_bottom.set_visible(false);
        _char_4_top.set_visible(false);
        _char_4_bottom.set_visible(false);
        _front_bg.set_visible(false);
        _select_arrow.set_visible(false);
        
        if(_position == _killer){
            _bg = bn::regular_bg_items::uri_end_win.create_bg(0,0);
            _victory = true;
        } else {
            _bg = bn::regular_bg_items::uri_end_lose.create_bg(0,0);
            _defeat = true;
        }
    }

    switch(_position){
        case 0:
            _char_1_top.set_item(_char_1, 0);
            _char_1_bottom.set_item(_char_1, 1);
            _char_2_top.set_item(_char_2, 2);
            _char_2_bottom.set_item(_char_2, 3);
            _char_3_top.set_item(_char_3, 2);
            _char_3_bottom.set_item(_char_3, 3);
            _char_4_top.set_item(_char_4, 2);
            _char_4_bottom.set_item(_char_4, 3);
        break;
        case 1:
            _char_1_top.set_item(_char_1, 2);
            _char_1_bottom.set_item(_char_1, 3);
            _char_2_top.set_item(_char_2, 0);
            _char_2_bottom.set_item(_char_2, 1);
            _char_3_top.set_item(_char_3, 2);
            _char_3_bottom.set_item(_char_3, 3);
            _char_4_top.set_item(_char_4, 2);
            _char_4_bottom.set_item(_char_4, 3);
        break;
        case 2:
            _char_1_top.set_item(_char_1, 2);
            _char_1_bottom.set_item(_char_1, 3);
            _char_2_top.set_item(_char_2, 2);
            _char_2_bottom.set_item(_char_2, 3);
            _char_3_top.set_item(_char_3, 0);
            _char_3_bottom.set_item(_char_3, 1);
            _char_4_top.set_item(_char_4, 2);
            _char_4_bottom.set_item(_char_4, 3);
        break;
        default:
            _char_1_top.set_item(_char_1, 2);
            _char_1_bottom.set_item(_char_1, 3);
            _char_2_top.set_item(_char_2, 2);
            _char_2_bottom.set_item(_char_2, 3);
            _char_3_top.set_item(_char_3, 2);
            _char_3_bottom.set_item(_char_3, 3);
            _char_4_top.set_item(_char_4, 0);
            _char_4_bottom.set_item(_char_4, 1);
        break;
    }

    _select_arrow.set_position(get_arrow_position(_position));

    return result;
}

void murder_game::fade_out([[maybe_unused]] const mj::game_data& data)
{
}

}
