#include "mj/mj_game_result_animation.h"

#include "bn_bg_palette_ptr.h"
#include "bn_math.h"
#include "bn_sprite_palette_ptr.h"

#include "bn_affine_bg_items_mj_speed_up.h"
#include "bn_affine_bg_items_mj_big_pumpkin.h"
#include "bn_affine_bg_items_mj_big_pumpkin_hand.h"
#include "bn_sprite_items_mj_big_pumpkin_eye.h"

namespace mj
{

namespace
{
    class wink_victory_animation : public game_result_animation
    {

    public:
        wink_victory_animation() = default;

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

    class rotate_victory_animation : public game_result_animation
    {

    public:
        rotate_victory_animation() = default;

    protected:
        void _update_impl() final
        {
            if(_pending_frames >= 70)
            {
                int counter = 100 - _pending_frames;
                bn::fixed sin = bn::degrees_lut_sin(bn::fixed(counter * 180) / 30);
                _rotation_angle += sin;
            }
            else
            {
                if(_pending_frames >= 30)
                {
                    int counter = 70 - _pending_frames;
                    bn::fixed sin = bn::degrees_lut_sin(bn::fixed(counter * 180) / 40);
                    _rotation_angle -= sin.unsafe_multiplication(bn::fixed(14.8965));
                    _left_eye_rotation_angle += sin * 2;
                    _right_eye_rotation_angle = _left_eye_rotation_angle;
                }
                else
                {
                    _rotation_angle = 0;
                }

                if(_left_eye_rotation_angle > -360)
                {
                    int counter = 70 - _pending_frames;
                    bn::fixed sin = bn::degrees_lut_sin(bn::fixed(counter * 180) / 70);
                    _left_eye_rotation_angle -= sin.unsafe_multiplication(bn::fixed(9.25));

                    if(_left_eye_rotation_angle < -360)
                    {
                        _left_eye_rotation_angle = -360;
                    }

                    _right_eye_rotation_angle = _left_eye_rotation_angle;
                }
            }
        }
    };

    class up_victory_animation : public game_result_animation
    {

    public:
        up_victory_animation() = default;

    protected:
        void _update_impl() final
        {
            if(_pending_frames >= 90)
            {
                _left_eye_horizontal_scale -= 0.02;
                _left_eye_vertical_scale += 0.01;
            }
            else if(_pending_frames >= 50)
            {
                int counter = 90 - _pending_frames;
                bn::fixed sin = bn::degrees_lut_sin(bn::fixed(counter * 90) / 40);
                _y -= sin.unsafe_multiplication(bn::fixed(7.41));
                _horizontal_scale -= sin.unsafe_multiplication(bn::fixed(0.02));

                if(_pending_frames == 50)
                {
                    _y += 380;
                }
            }
            else if(_pending_frames >= 10)
            {
                int counter = 50 - _pending_frames;
                bn::fixed sin = bn::degrees_lut_sin(90 + (bn::fixed(counter * 90) / 40));
                _y -= sin.unsafe_multiplication(bn::fixed(7.5175));
                _horizontal_scale += sin.unsafe_multiplication(bn::fixed(0.02055));
            }
            else
            {
                _y = _initial_y;
                _horizontal_scale = 1;
                _left_eye_horizontal_scale += 0.02;
                _left_eye_vertical_scale -= 0.01;
            }

            _vertical_scale = 2 - _horizontal_scale;
            _right_eye_horizontal_scale = _left_eye_horizontal_scale;
            _right_eye_vertical_scale = _left_eye_vertical_scale;
        }
    };

    class hand_victory_animation : public game_result_animation
    {

    public:
        hand_victory_animation() = default;

    protected:
        void _update_impl() final
        {
            if(_pending_frames >= 80)
            {
                _x += 1.5;
                _y -= 0.5;
                _rotation_angle -= 0.6;
            }
            else if(_pending_frames >= 20)
            {
            }
            else
            {
                _x -= 1.5;
                _y += 0.5;
                _rotation_angle += 0.6;
            }

            _update_hand();
        }

    private:
        void _update_hand()
        {
            if(_pending_frames >= 90)
            {
                _hand_x = 48;
                _hand_horizontal_scale = 0.45;
                _hand_rotation_angle = -32.5;
            }
            else
            {
                _hand_rotation_angle += 0.05;
                _hand_horizontal_scale += 0.001;

                if(_pending_frames >= 60)
                {
                    int counter = 90 - _pending_frames;
                    bn::fixed cos = bn::degrees_lut_cos(bn::fixed(counter * 90) / 30);
                    _hand_x -= cos.unsafe_multiplication(bn::fixed(5.15));
                    _hand_y -= cos.unsafe_multiplication(bn::fixed(7.75));
                    _hand_horizontal_scale += cos.unsafe_multiplication(bn::fixed(0.0325));
                    _hand_rotation_angle += cos.unsafe_multiplication(bn::fixed(2.425));
                }
                else if(_pending_frames >= 40)
                {
                }
                else if(_pending_frames >= 10)
                {
                    int counter = 40 - _pending_frames;
                    bn::fixed sin = bn::degrees_lut_sin(bn::fixed(counter * 90) / 30);
                    _hand_x += sin.unsafe_multiplication(bn::fixed(5.15));
                    _hand_y += sin.unsafe_multiplication(bn::fixed(7.75));
                    _hand_horizontal_scale -= sin.unsafe_multiplication(bn::fixed(0.0325));
                    _hand_rotation_angle -= sin.unsafe_multiplication(bn::fixed(2.425));
                }
                else
                {
                }
            }

            _hand_vertical_scale = _hand_horizontal_scale;
        }
    };

    class defeat_animation : public game_result_animation
    {

    public:
        explicit defeat_animation(bn::fixed x_desp) :
            _x_desp(x_desp)
        {
        }

    protected:
        void _update_impl() final
        {
            if(_pending_frames >= 70)
            {
                _x += _x_desp;
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
                _x -= _x_desp;
                _y -= 0.9;
                _vertical_scale += 0.015;
                _rotation_angle -= 0.05;
                _left_eye_vertical_scale += 0.01;
                _right_eye_vertical_scale += 0.01;
                _left_eye_rotation_angle += 0.2;
                _right_eye_rotation_angle -= 0.2;
            }
        }

    private:
        bn::fixed _x_desp;
    };

    class speed_inc_animation : public game_result_animation
    {

    public:
        speed_inc_animation()
        {
            _pending_frames = 150;
        }

    protected:
        void _update_impl() final
        {
            if(_pending_frames >= 120)
            {
                _y -= 0.1;
                _horizontal_scale += 0.035;
                _left_eye_rotation_angle -= 5;
            }
            else if(_pending_frames >= 30)
            {
                if(_pending_frames >= 60)
                {
                    _horizontal_scale += 0.005;
                    _left_eye_vertical_scale -= 0.0025;
                }
                else
                {
                    _horizontal_scale -= 0.005 * 2;
                    _left_eye_vertical_scale += 0.0025 * 2;
                }

                _left_eye_horizontal_scale = 2 - _left_eye_vertical_scale;
                _hand_y = 0;
                _update_speed_up();
            }
            else
            {
                _y += 0.1;
                _horizontal_scale -= 0.035;
                _left_eye_horizontal_scale = 1;
                _left_eye_vertical_scale = 1;
                _left_eye_rotation_angle += 5;
                _hand_y = _initial_hand_y;
            }

            _right_eye_horizontal_scale = _left_eye_horizontal_scale;
            _right_eye_vertical_scale = _left_eye_vertical_scale;
            _right_eye_rotation_angle = -_left_eye_rotation_angle;
            _vertical_scale = _horizontal_scale;
        }

    private:
        void _update_speed_up()
        {
            int counter = _pending_frames - 30;

            if(counter >= 70)
            {
                _hand_vertical_scale = 1 + (bn::fixed(70 - counter) / 20);
                _hand_rotation_angle = 32 - (_hand_vertical_scale * 32);
            }
            else if(counter >= 58)
            {
                _fade_intensity += 0.04;
            }
            else if(counter >= 46)
            {
                _fade_intensity -= 0.04;
            }
            else if(counter >= 34)
            {
                _fade_intensity += 0.04;
            }
            else if(counter >= 20)
            {
                _fade_intensity = bn::max(_fade_intensity - 0.04, bn::fixed(0));
            }
            else
            {
                _hand_vertical_scale = bn::fixed(counter) / 20;
                _hand_rotation_angle = (_hand_vertical_scale * 32) - 32;
            }

            _hand_vertical_scale = bn::max(_hand_vertical_scale, bn::fixed(0.01));
        }
    };
}

bn::unique_ptr<game_result_animation> game_result_animation::create(int completed_games, bool victory)
{
    bn::unique_ptr<game_result_animation> result;

    if(victory)
    {
        switch(completed_games % 4)
        {

        case 0:
            result.reset(new hand_victory_animation());
            break;

        case 1:
            result.reset(new rotate_victory_animation());
            break;

        case 2:
            result.reset(new wink_victory_animation());
            break;

        default:
            result.reset(new up_victory_animation());
            break;
        }
    }
    else
    {
        result.reset(new defeat_animation(completed_games % 2 ? bn::fixed(0.2) : bn::fixed(-0.2)));
    }

    return result;
}

bn::unique_ptr<game_result_animation> game_result_animation::create_speed_inc()
{
    auto animation = new speed_inc_animation();
    animation->_hand.set_item(bn::affine_bg_items::mj_speed_up);
    return bn::unique_ptr<game_result_animation>(animation);
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
    _hand(bn::affine_bg_items::mj_big_pumpkin_hand.create_bg(0, 0)),
    _left_eye(bn::sprite_items::mj_big_pumpkin_eye.create_sprite(0, 0)),
    _right_eye(bn::sprite_items::mj_big_pumpkin_eye.create_sprite(0, 0))
{
    _head.set_wrapping_enabled(false);
    _head.set_priority(1);
    _left_eye.set_bg_priority(1);
    _right_eye.set_bg_priority(1);
    _hand.set_wrapping_enabled(false);
    _hand.set_priority(0);

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
    _hand.set_position(_hand_x, _hand_y);
    _hand.set_scale(_hand_horizontal_scale, _hand_vertical_scale);
    _hand.set_rotation_angle(fixed_angle(_hand_rotation_angle));

    bn::bg_palette_ptr bg_palette = _hand.palette();
    bg_palette.set_fade(bn::color(24, 0, 12), _fade_intensity);

    bn::sprite_palette_ptr sprite_palette = _left_eye.palette();
    sprite_palette.set_fade(bn::color(24, 0, 12), _fade_intensity);
}

}
