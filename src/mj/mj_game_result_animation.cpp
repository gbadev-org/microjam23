#include "mj/mj_game_result_animation.h"

#include "bn_math.h"

#include "bn_affine_bg_items_mj_big_pumpkin.h"
#include "bn_sprite_items_mj_big_pumpkin_eye.h"

namespace mj
{

namespace
{
    class victory_animation : public game_result_animation
    {

    public:
        victory_animation() = default;

    protected:
        void _update_impl() final
        {
            if(_pending_frames >= 70)
            {
                _x += 1;
                _y += 1;
                _horizontal_scale += 0.035;
                _vertical_scale += 0.035;
                _rotation_angle += 0.2;
            }
            else if(_pending_frames >= 60)
            {
            }
            else if(_pending_frames >= 50)
            {
                _left_eye_vertical_scale -= 0.1;
            }
            else if(_pending_frames >= 40)
            {
                _left_eye_vertical_scale += 0.1;
            }
            else if(_pending_frames >= 30)
            {
            }
            else
            {
                _x -= 1;
                _y -= 1;
                _horizontal_scale -= 0.035;
                _vertical_scale -= 0.035;
                _rotation_angle -= 0.2;
            }
        }
    };

    class defeat_animation : public game_result_animation
    {

    public:
        defeat_animation() = default;

    protected:
        void _update_impl() final
        {
            if(_pending_frames >= 70)
            {
                _x += 0.2;
                _y += 0.9;
                _vertical_scale -= 0.015;
                _rotation_angle += 0.05;
                _left_eye_vertical_scale -= 0.01;
                _right_eye_vertical_scale -= 0.01;
                _left_eye_rotation_angle -= 0.2;
                _right_eye_rotation_angle += 0.2;
            }
            else if(_pending_frames >= 50)
            {
                switch(_pending_frames % 4)
                {

                case 0:
                    _x += 2;
                    _y -= 2;
                    break;

                case 1:
                    _x -= 3;
                    _y += 3;
                    break;

                case 2:
                    _x += 2;
                    _y -= 2;
                    break;

                default:
                    _x -= 1;
                    _y += 1;
                    break;
                }
            }
            else if(_pending_frames >= 40)
            {
                _left_eye_vertical_scale -= 0.06;
                _right_eye_vertical_scale -= 0.06;
            }
            else if(_pending_frames >= 30)
            {
                _left_eye_vertical_scale += 0.06;
                _right_eye_vertical_scale += 0.06;
            }
            else
            {
                _x -= 0.2;
                _y -= 0.9;
                _vertical_scale += 0.015;
                _rotation_angle -= 0.05;
                _left_eye_vertical_scale += 0.01;
                _right_eye_vertical_scale += 0.01;
                _left_eye_rotation_angle += 0.2;
                _right_eye_rotation_angle -= 0.2;
            }
        }
    };
}

bn::unique_ptr<game_result_animation> game_result_animation::create(bool victory)
{
    bn::unique_ptr<game_result_animation> result;

    if(victory)
    {
        result.reset(new victory_animation());
    }
    else
    {
        result.reset(new defeat_animation());
    }

    return result;
}

bool game_result_animation::update()
{
    if(_delay_frames)
    {
        --_delay_frames;
    }
    else
    {
        --_pending_frames;

        _update_impl();
        _update_gfx();
    }

    return _pending_frames;
}

game_result_animation::game_result_animation() :
    _head(bn::affine_bg_items::mj_big_pumpkin.create_bg(0, 0)),
    _left_eye(bn::sprite_items::mj_big_pumpkin_eye.create_sprite(0, 0)),
    _right_eye(bn::sprite_items::mj_big_pumpkin_eye.create_sprite(0, 0))
{
    _head.set_wrapping_enabled(false);
    _head.set_priority(0);
    _left_eye.set_bg_priority(0);
    _right_eye.set_bg_priority(0);

    _update_gfx();
}

void game_result_animation::_update_gfx()
{
    auto fixed_angle = [](bn::fixed angle)
    {
        while(angle < 0)
        {
            angle += 360;
        }

        while(angle > 360)
        {
            angle -= 360;
        }

        return angle;
    };

    auto update_eye = [&fixed_angle](
            bn::fixed head_x, bn::fixed head_y, bn::fixed head_horizontal_scale, bn::fixed head_vertical_scale,
            bn::fixed head_rotation_angle, bn::fixed eye_x, bn::fixed eye_y, bn::fixed eye_horizontal_scale,
            bn::fixed eye_vertical_scale, bn::fixed eye_rotation_angle, bn::sprite_ptr& sprite)
    {
        eye_x = eye_x.unsafe_multiplication(head_horizontal_scale);
        eye_y = eye_y.unsafe_multiplication(head_vertical_scale);

        bn::pair<bn::fixed, bn::fixed> sin_and_cos = bn::degrees_lut_sin_and_cos(360 - head_rotation_angle);
        bn::fixed sin = sin_and_cos.first;
        bn::fixed cos = sin_and_cos.second;
        bn::fixed x = eye_x.unsafe_multiplication(cos) - eye_y.unsafe_multiplication(sin);
        bn::fixed y = eye_y.unsafe_multiplication(cos) + eye_x.unsafe_multiplication(sin);
        eye_x = x;
        eye_y = y;

        sprite.set_position(head_x + eye_x, head_y + eye_y);
        sprite.set_scale(head_horizontal_scale.unsafe_multiplication(eye_horizontal_scale),
                         head_vertical_scale.unsafe_multiplication(eye_vertical_scale));
        sprite.set_rotation_angle(fixed_angle(head_rotation_angle + eye_rotation_angle));
    };

    bn::fixed rotation_angle = fixed_angle(_rotation_angle);
    _head.set_position(_x, _y);
    _head.set_scale(_horizontal_scale, _vertical_scale);
    _head.set_rotation_angle(rotation_angle);
    update_eye(_x, _y, _horizontal_scale, _vertical_scale, rotation_angle, _left_eye_x - 26, _left_eye_y - 24,
               _left_eye_horizontal_scale, _left_eye_vertical_scale, _left_eye_rotation_angle, _left_eye);
    update_eye(_x, _y, _horizontal_scale, _vertical_scale, rotation_angle, _right_eye_x + 25, _right_eye_y - 24,
               _right_eye_horizontal_scale, _right_eye_vertical_scale, _right_eye_rotation_angle, _right_eye);
}

}
