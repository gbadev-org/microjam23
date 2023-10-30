#ifndef MTTHGN_GAME_H
#define MTTHGN_GAME_H

#include "bn_regular_bg_ptr.h"
#include "bn_sprite_ptr.h"
#include "bn_fixed_point.h"
#include "bn_sprite_animate_actions.h"

#include "mj/mj_game.h"

namespace mtthgn 
{
  union direction
  {
      struct
      {
        unsigned up: 1;
        unsigned down: 1;
        unsigned left: 1;
        unsigned right: 1;
      } keys;
      int data = 0;
  };

  class game : public mj::game
  {
    public: 
      game(int completed_games, const mj::game_data& data);

      [[nodiscard]] bn::string<16> title() const final
      {
        return "Trick or Treat!";
      }

      [[nodiscard]] int total_frames() const final
      {
        return _total_frames;
      }

      void fade_in(const mj::game_data& data) final;

      [[nodiscard]] mj::game_result play(const mj::game_data& data) final;

      [[nodiscard]] bool victory() const final
      {
        return _victory;
      }

      void fade_out(const mj::game_data& data) final;
    
    private:
      bn::regular_bg_ptr _bg;
      bn::sprite_ptr _player_sprite;
      bn::fixed_point _player_tile_position;
      bn::sprite_animate_action<4> _pumpkin_animation;
      const bn::regular_bg_map_item& _map_item;
      int _valid_tile_index[9];
      int _lose_tile_index[21];
      int _win_tile_index[2];
      direction _direction;
      int _total_frames;
      bn::fixed _walk_speed;
      int _show_result_frames;
      bool _victory = false;
      bool _defeat = false;
  };


}

#endif
