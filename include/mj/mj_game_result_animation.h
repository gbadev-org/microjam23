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
    [[nodiscard]] static bn::unique_ptr<game_result_animation> create(bool victory);

    virtual ~game_result_animation() = default;

    [[nodiscard]] bool update();

protected:
    bn::fixed _x;
    bn::fixed _y = 16;
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
    int _delay_frames = 8;
    int _pending_frames = 100;

    game_result_animation();

    virtual void _update_impl() = 0;

private:
    bn::affine_bg_ptr _head;
    bn::sprite_ptr _left_eye;
    bn::sprite_ptr _right_eye;

    void _update_gfx();
};

}

#endif
