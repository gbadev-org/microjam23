#include "ahul_game.h"

#include "bn_keypad.h"
#include "mj/mj_game_list.h"
#include "bn_sprite_animate_actions.h"

//Resources
#include "bn_regular_bg_items_ahul_background.h"
#include "bn_regular_bg_items_ahul_background_dark.h"
#include "bn_sprite_items_ahul_girl.h"

namespace
{
    constexpr bn::string_view code_credits[] = { "ahul" };
    constexpr bn::string_view graphics_credits[] = { "ahul" };
}

MJ_GAME_LIST_ADD(ahul::game)
MJ_GAME_LIST_ADD_CODE_CREDITS(code_credits)
MJ_GAME_LIST_ADD_GRAPHICS_CREDITS(graphics_credits)

namespace ahul
{

game::game(int completed_games, const mj::game_data& data) :
    mj::game("ahul"),
    _bg(bn::regular_bg_items::ahul_background.create_bg((256 - 240) / 2, (256 - 160) / 2)),
    _character_sprite(bn::sprite_items::ahul_girl.create_sprite(0, 0)),
    _action(bn::create_sprite_animate_action_forever(
                    _character_sprite, 8, bn::sprite_items::ahul_girl.tiles_item(), 0, 1, 2, 3))
{
    _total_frames = play_jingle(mj::game_jingle_type::TOTSNUK09, completed_games, data);
    _walk_tempo = recommended_music_tempo(completed_games, data);
    _show_result_frames = (60 / _walk_tempo).right_shift_integer();

    bn::point character_map_position(0, 0);
    _character_map_position = character_map_position;
    _character_sprite.set_position(60, _character_map_position.y()); // 0, 0
}

void game::fade_in([[maybe_unused]] const mj::game_data& data)
{
}

mj::game_result game::play(const mj::game_data& data)
{
    mj::game_result result;
    result.exit = data.pending_frames == 0;
    _frames_to_dark -= 1;

    if (_frames_to_dark == 0) {
        _is_dark = true;
        int random_frames_to_skip = (180 / _walk_tempo).right_shift_integer();
        int minimum_frames_to_skip = (60 * _walk_tempo).right_shift_integer();
        _frames_to_dark = data.random.get_int(random_frames_to_skip) + minimum_frames_to_skip;
    }

    if (_is_dark) {
        _bg.set_item(bn::regular_bg_items::ahul_background_dark);
    } else {
        _bg.set_item(bn::regular_bg_items::ahul_background);
    }

    if(! _victory && ! _defeat)
    {
        if (_is_dark) {
            bn::fixed character_sprite_x =  _character_sprite.x() + (0.5 * _walk_tempo);
            if (character_sprite_x > 100) {
                character_sprite_x = 100;
            }
            _character_sprite.set_position(character_sprite_x, 0);
            _character_sprite.set_horizontal_flip(true);
        } else {
            bn::fixed character_sprite_x = _character_sprite.x() - (0.75 * _walk_tempo);
            _character_sprite.set_position(character_sprite_x, 0);
            _character_sprite.set_horizontal_flip(false);
        }

        if (_character_sprite.x() < -30) {
            result.remove_title = true;
            _victory = true;
        }
        
        _action.update();

        if(bn::keypad::a_pressed() || bn::keypad::b_pressed() || bn::keypad::r_pressed() || bn::keypad::l_pressed())
        {
            _is_dark = !_is_dark;
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

    return result;
}

void game::fade_out([[maybe_unused]] const mj::game_data& data)
{
}

}
