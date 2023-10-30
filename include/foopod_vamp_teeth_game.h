#ifndef FOOPOD_VAMP_TEETH_GAME_H
#define FOOPOD_VAMP_TEETH_GAME_H

#include "bn_regular_bg_ptr.h"
#include "bn_sprite_ptr.h"
#include "bn_fixed.h"
#include "bn_fixed_point.h"
#include "bn_sprite_animate_actions.h"

#include "bn_regular_bg_items_foopod_vamp_teeth_bg.h"
#include "bn_regular_bg_items_foopod_vamp_bite_bg.h"
#include "bn_sprite_items_foopod_vamp_target.h"
#include "bn_sprite_items_foopod_vamp.h"
#include "bn_sprite_items_foopod_vamp_bite.h"

#include "mj/mj_game.h"

namespace foopod
{
    enum class STATE
    {
        IDLE = 1,
        WALK_RIGHT = 2,
        WALK_LEFT = 3,
        WALK_UP = 4,
        WALK_DOWN = 5,
        CAUGHT = 6,
        MISSED = 7,
    };

}


namespace foopod
{

class vamp_teeth_game : public mj::game
{

public:
    vamp_teeth_game(int completed_games, const mj::game_data& data);

    [[nodiscard]] bn::string<16> title() const final
    {
        return "Sneak Up Behind!";
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
    bn::regular_bg_ptr _bg = bn::regular_bg_items::foopod_vamp_teeth_bg.create_bg(0, 0);
        bn::regular_bg_ptr _neck = bn::regular_bg_items::foopod_vamp_bite_bg.create_bg(64, 0);
        bn::sprite_ptr _target = bn::sprite_items::foopod_vamp_target.create_sprite(-19, 22);
        bn::sprite_ptr _player = bn::sprite_items::foopod_vamp.create_sprite(-80, -40, 8);
        bn::sprite_animate_action<8> _action = bn::create_sprite_animate_action_forever(
            _player, 12, bn::sprite_items::foopod_vamp.tiles_item(), 8, 8, 8, 8, 9, 9, 9, 9);
        bn::sprite_ptr _bite = bn::sprite_items::foopod_vamp_bite.create_sprite(88, 32, 0);
        bn::sprite_animate_action<4> _bite_action = bn::create_sprite_animate_action_forever(
            _bite, 20, bn::sprite_items::foopod_vamp_bite.tiles_item(), 0, 0, 0, 0);
        bn::fixed_point _velocity = bn::fixed_point(0, 0);

        const bn::fixed _friction = 0.90;
        bn::fixed _x_speed = 0.14;
        bn::fixed _y_speed = 0.10;
        STATE _state = STATE::IDLE;
        STATE _old_state = STATE::IDLE;

        int _total_frames;
        int _show_result_frames = 60;
        bool _victory = false;
        bool _defeat = false;
};

}

#endif
