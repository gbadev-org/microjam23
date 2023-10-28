#include "mtthgn_game.h"

#include "bn_keypad.h"
#include "bn_regular_bg_map_cell_info.h"
#include "bn_sound_items.h"

#include "mj/mj_game_list.h"

#include "bn_regular_bg_items_mtthgn_map.h"
#include "bn_regular_bg_items_mtthgn_map_defeat.h"
#include "bn_regular_bg_items_mtthgn_map_victory.h"
#include "bn_sprite_items_mj_small_pumpkin.h"

namespace
{
    constexpr bn::string_view code_credits[] = { "mtthgn" };
    constexpr bn::string_view graphics_credits[] = { "mtthgn", "Emcee Flesher" };
}

// MJ_GAME_LIST_ADD(mtthgn::game)
MJ_GAME_LIST_ADD_CODE_CREDITS(code_credits)
MJ_GAME_LIST_ADD_GRAPHICS_CREDITS(graphics_credits)

namespace mtthgn
{
  game::game(int completed_games, const mj::game_data& data) :
    _bg(bn::regular_bg_items::mtthgn_map.create_bg(0, 0)),
    _player_sprite(bn::sprite_items::mj_small_pumpkin.create_sprite(0,0)),
    _pumpkin_animation(bn::create_sprite_animate_action_forever(
                           _player_sprite, 16,
                           bn::sprite_items::mj_small_pumpkin.tiles_item(), 1, 0, 1, 2)),
    _map_item(bn::regular_bg_items::mtthgn_map.map_item()),
    _total_frames(play_jingle(mj::game_jingle_type::EXELOTL01, completed_games, data)),
    _walk_speed(recommended_music_tempo(completed_games, data)),
    _show_result_frames((_walk_speed * 120).right_shift_integer())
  {
    for(int i = 0; i < 9; i++) {
      _valid_tile_index[i] = _map_item.cell(i,0);
    }

    for(int i = 0; i < 21; i++) {
      _lose_tile_index[i] = _map_item.cell(i,1);
    }

    for(int i = 0; i < 2; i++) {
      _win_tile_index[i] = _map_item.cell(i, 2);
    }

    _player_tile_position = bn::point(
      (27 * 8) - (_map_item.dimensions().width() * 4) + 4,
      (22 * 8) - (_map_item.dimensions().height() * 4) + 4);

    _player_sprite.set_position(_player_tile_position.x(), _player_tile_position.y());
    _direction.keys.up = true;

    if(completed_games > 30) {
      _walk_speed = 2;
    }
  }

  void game::fade_in([[maybe_unused]] const mj::game_data& data)
  {
  }

  mj::game_result game::play(const mj::game_data& data)
  {
    bn::fixed_point new_player_tile_position = _player_tile_position;
    mj::game_result result;
    direction new_direction;
    bool collision_detected = false;

    result.exit = data.pending_frames == 0;

      if(!_victory && !_defeat)
      {
        if(bn::keypad::left_held())
        {
          new_player_tile_position.set_x(new_player_tile_position.x() - _walk_speed);
          new_direction.keys.left = true;
        } 
        else if(bn::keypad::right_held())
        {
          new_player_tile_position.set_x(new_player_tile_position.x() + _walk_speed);
          new_direction.keys.right = true;
        } 
        else if (bn::keypad::up_held())
        {
          new_player_tile_position.set_y(new_player_tile_position.y() - _walk_speed);
          new_direction.keys.up = true;
        } 
        else if (bn::keypad::down_held())
        {
          new_player_tile_position.set_y(new_player_tile_position.y() + _walk_speed);
          new_direction.keys.down = true;
        }

        if(_direction.data != new_direction.data)
        {
          if(new_direction.keys.left)
          {
            _pumpkin_animation = bn::create_sprite_animate_action_forever(
              _player_sprite, 16,
              bn::sprite_items::mj_small_pumpkin.tiles_item(),
              4, 3, 4, 5
            );
          }
          else if(new_direction.keys.right)
          {
            _pumpkin_animation = bn::create_sprite_animate_action_forever(
              _player_sprite, 16,
              bn::sprite_items::mj_small_pumpkin.tiles_item(),
              10, 9, 10, 11
            );
          }
          else if(new_direction.keys.up)
          {
            _pumpkin_animation = bn::create_sprite_animate_action_forever(
              _player_sprite, 16,
              bn::sprite_items::mj_small_pumpkin.tiles_item(),
              1, 0, 1, 2
            );
          }
          else if(new_direction.keys.down)
          {
            _pumpkin_animation = bn::create_sprite_animate_action_forever(
              _player_sprite, 16,
              bn::sprite_items::mj_small_pumpkin.tiles_item(),
              7, 6, 7, 8
            );
          }

          _direction = new_direction;
        }

        bn::point next_cell_point(
            ((new_player_tile_position.x() + (_map_item.dimensions().width() * 4) -4)/ 8).right_shift_integer(),
            ((new_player_tile_position.y() + (_map_item.dimensions().height() * 4) - 4)/ 8).right_shift_integer());

        bn::regular_bg_map_cell next_cell = _map_item.cell(next_cell_point);
        int next_tile_index = bn::regular_bg_map_cell_info(next_cell).tile_index();

        for(int i = 0; i < 9; i++) {
          if(next_tile_index == _valid_tile_index[i]) {
            collision_detected = true;
            break;
          }
        }

        if(!collision_detected) {
          _player_tile_position = new_player_tile_position;

          for(int i = 0; i < 21; i++) {
            if(next_tile_index == _lose_tile_index[i]) {
              _bg.set_item(bn::regular_bg_items::mtthgn_map_defeat);
              bn::sound_items::mj_pause_exit.play();
              result.remove_title = true;
              _defeat = true;
              break;
            }
          }
        }

        _player_sprite.set_position(_player_tile_position.x(), _player_tile_position.y());
        _pumpkin_animation.update();

        if(bn::keypad::a_pressed())
        {
          bn::point current_cell_point(
              ((_player_tile_position.x() + (_map_item.dimensions().width() * 4) -4)/ 8).right_shift_integer(),
              ((_player_tile_position.y() + (_map_item.dimensions().height() * 4) - 4)/ 8).right_shift_integer());

          bn::regular_bg_map_cell current_cell = _map_item.cell(current_cell_point);
          int current_tile_index = bn::regular_bg_map_cell_info(current_cell).tile_index();

          for(int i = 0; i < 2; i++) {
            if(current_tile_index == _win_tile_index[i]) {
              _bg.set_item(bn::regular_bg_items::mtthgn_map_victory);
              bn::sound_items::mj_pause_exit.play();
              result.remove_title = true;
              _victory = true;
              break;
            }
          }
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
