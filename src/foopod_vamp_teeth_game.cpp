#include "foopod_vamp_teeth_game.h"

#include "bn_keypad.h"
#include "bn_math.h"

#include "mj/mj_game_list.h"


namespace
{
    constexpr bn::string_view code_credits[] = { "Jono Shields" };
    constexpr bn::string_view graphics_credits[] = { "Jono Shields" };
}

MJ_GAME_LIST_ADD(foopod::vamp_teeth_game)
MJ_GAME_LIST_ADD_CODE_CREDITS(code_credits)
MJ_GAME_LIST_ADD_GRAPHICS_CREDITS(graphics_credits)

namespace foopod
{

vamp_teeth_game::vamp_teeth_game(int completed_games, const mj::game_data& data) :
    mj::game("foopod"),
    _total_frames(play_jingle(mj::game_jingle_type::TOTSNUK06, completed_games, data))
{
    _neck.set_visible(false);
}

void vamp_teeth_game::fade_in([[maybe_unused]] const mj::game_data& data)
{
}

mj::game_result vamp_teeth_game::play(const mj::game_data& data)
{
    mj::game_result result;
        result.exit = data.pending_frames == 0;

        if (bn::keypad::left_held())
        {
            _velocity.set_x(_velocity.x() - _x_speed);
        }

        if (bn::keypad::right_held())
        {
            _velocity.set_x(_velocity.x() + _x_speed);
        }

        if (bn::keypad::up_held())
        {
            _velocity.set_y(_velocity.y() - _y_speed);
        }

        if (bn::keypad::down_held())
        {
            _velocity.set_y(_velocity.y() + _y_speed);
        }

        _velocity = _velocity * _friction;

        _old_state = _state;
        if (_velocity.x() > 0.2)
        {
            _state = STATE::WALK_RIGHT;
            _player.set_horizontal_flip(false);
        }
        else if (_velocity.x() < -0.2)
        {
            _state = STATE::WALK_LEFT;
            _player.set_horizontal_flip(true);
        }
        else if (_velocity.y() > 0.1)
        {
            _state = STATE::WALK_UP;
        }
        else if (_velocity.y() < -0.1)
        {
            _state = STATE::WALK_DOWN;
        }
        else
        {
            _state = STATE::IDLE;
        }

        if (_state != STATE::CAUGHT && bn::abs(_player.x() + 6 - _target.x()) < 16 && bn::abs(_player.y() + 6 - _target.y()) < 16)
        {
            _target.set_visible(false);
            _player.set_position(bn::fixed_point(_target.x() + 8, _target.y() - 8));
            _player.set_horizontal_flip(false);
            _state = STATE::CAUGHT;
            _velocity = bn::fixed_point(0, 0);
            _victory = true;
            _neck.set_visible(true);
        }

        if (_state == STATE::CAUGHT && _neck.x() > 0)
        {
            _neck.set_x(_neck.x() - 2);
        }

        if (_old_state != _state)
        {
            switch (_state)
            {
            case STATE::WALK_LEFT:
                _action = bn::create_sprite_animate_action_forever(
                    _player, 12, bn::sprite_items::foopod_vamp.tiles_item(), 0, 1, 2, 3, 4, 5, 6, 7);
                break;
            case STATE::WALK_RIGHT:
                _action = bn::create_sprite_animate_action_forever(
                    _player, 12, bn::sprite_items::foopod_vamp.tiles_item(), 0, 1, 2, 3, 4, 5, 6, 7);
                break;
            case STATE::WALK_UP:
                _action = bn::create_sprite_animate_action_forever(
                    _player, 12, bn::sprite_items::foopod_vamp.tiles_item(), 0, 1, 2, 3, 4, 5, 6, 7);
                break;
            case STATE::WALK_DOWN:
                _action = bn::create_sprite_animate_action_forever(
                    _player, 12, bn::sprite_items::foopod_vamp.tiles_item(), 0, 1, 2, 3, 4, 5, 6, 7);
                break;
            case STATE::CAUGHT:
                _action = bn::create_sprite_animate_action_once(
                    _player, 20, bn::sprite_items::foopod_vamp.tiles_item(), 10, 10, 10, 10, 11, 11, 11, 11);
                _bite_action = bn::create_sprite_animate_action_once(
                    _bite, 40, bn::sprite_items::foopod_vamp_bite.tiles_item(), 0, 1, 2, 3);
                break;
            case STATE::IDLE:
            case STATE::MISSED:
            default:
                _action = bn::create_sprite_animate_action_forever(
                    _player, 40, bn::sprite_items::foopod_vamp.tiles_item(), 8, 8, 8, 8, 9, 9, 9, 9);
                break;
            }
        }

        _player.set_x(_player.x() + _velocity.x());
        _player.set_y(_player.y() + _velocity.y());

        if (!_bite_action.done())
        {
            _bite_action.update();
        }
        if (!_action.done())
        {
            _action.update();
        }
        else
        {
            result.exit = true;
        }

        return result;
}

void vamp_teeth_game::fade_out([[maybe_unused]] const mj::game_data& data)
{
}

}
