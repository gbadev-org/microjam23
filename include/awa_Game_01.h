#ifndef AWA_TEST_GAME_H
#define AWA_TEST_GAME_H

#include "bn_fixed_point.h"
#include "bn_regular_bg_ptr.h"
#include "bn_sprite_ptr.h"
#include "bn_sprite_actions.h"
#include "bn_sprite_animate_actions.h"

#include "mj/mj_game.h"

namespace awa
{

class Game01 : public mj::game
{

public:
    Game01(int completed_games, const mj::game_data& data);

    [[nodiscard]] bn::string<16> title() const final
    {
        return "Bob for apples!";
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
    const int _APPLE_RADIUS = 20;
    const int _BARREL_RADIUS = 65;

    bn::regular_bg_ptr _bg;
    bn::sprite_ptr _apple_sprite_0;
    bn::sprite_ptr _apple_sprite_1;
    bn::sprite_ptr _apple_sprite_2;
    bn::sprite_ptr _player_sprite;
    bn::sprite_ptr _vic_sprite;
    bn::sprite_ptr _def_sprite;

    bn::sprite_animate_action<4> _apple_anim_0;
    bn::sprite_animate_action<4> _apple_anim_1;
    bn::sprite_animate_action<4> _apple_anim_2;

    bn::sprite_move_by_action _apple_moveby_0;
    bn::sprite_move_by_action _apple_moveby_1;
    bn::sprite_move_by_action _apple_moveby_2;

    bn::fixed _tempo;
    bn::fixed _updates;
    int _total_frames;
    int _show_result_frames;
    bool _victory = false;
    bool _defeat = false;

    virtual int estimate_hypoteneuse(bn::fixed player_x, bn::fixed player_y) final;
    virtual bool bounds_check(bn::fixed x_pos, bn::fixed y_pos) final;
    virtual bool is_colliding_apple(bn::fixed_point player, bn::fixed_point apple) final;
    virtual bn::fixed_point rotate_vector(bn::fixed_point direction) final;
    virtual void update_animations() final;
    virtual void update_apple_position(bn::sprite_ptr& apple, bn::sprite_move_by_action& apple_moveby) final;

    void _play_impl(mj::game_result& result);
};

}

#endif
