#ifndef MJ_GAME_RESULT_ANIMATION_H
#define MJ_GAME_RESULT_ANIMATION_H

#include "bn_affine_bg_ptr.h"
#include "bn_sprite_ptr.h"
#include "bn_unique_ptr.h"

namespace mj
{

class game_result_animation
{

public:
    [[nodiscard]] static bn::unique_ptr<game_result_animation> create(int completed_games, bool victory);

    [[nodiscard]] static bn::unique_ptr<game_result_animation> create_speed_inc();

    virtual ~game_result_animation() = default;

    [[nodiscard]] bool update();

protected:
    static constexpr bn::fixed _initial_y = 16;
    static constexpr bn::fixed _initial_hand_y = 160;

    bn::fixed _x;
    bn::fixed _y = _initial_y;
    bn::fixed _horizontal_scale = 1;
    bn::fixed _vertical_scale = 1;
    bn::fixed _rotation_angle;
    bn::fixed _left_eye_x;
    bn::fixed _left_eye_y;
    bn::fixed _left_eye_horizontal_scale = 1;
    bn::fixed _left_eye_vertical_scale = 1;
    bn::fixed _left_eye_rotation_angle;
    bn::fixed _right_eye_x;
    bn::fixed _right_eye_y;
    bn::fixed _right_eye_horizontal_scale = 1;
    bn::fixed _right_eye_vertical_scale = 1;
    bn::fixed _right_eye_rotation_angle;
    bn::fixed _hand_x;
    bn::fixed _hand_y = _initial_hand_y;
    bn::fixed _hand_horizontal_scale = 1;
    bn::fixed _hand_vertical_scale = 1;
    bn::fixed _hand_rotation_angle;
    bn::fixed _fade_intensity;
    int _delay_frames = 8;
    int _pending_frames = 100;

    game_result_animation();

    virtual void _update_impl() = 0;

private:
    bn::affine_bg_ptr _head;
    bn::affine_bg_ptr _hand;
    bn::sprite_ptr _left_eye;
    bn::sprite_ptr _right_eye;

    void _update_gfx();
};

}

#endif
