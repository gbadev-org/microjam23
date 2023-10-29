#include "gvsnb_hand.h"

#include "bn_fixed_point.h"
#include "bn_keypad.h"

#include "gvsnb_candy.h"
#include "gvsnb_utils.h"

#include "bn_sprite_items_gvsnb_hand.h"
#include "bn_sprite_tiles_items_gvsnb_candy_big.h"

namespace gvsnb
{

namespace
{
    constexpr bn::fixed move_speed = 3;
    constexpr bn::fixed candy_y = 80 - 52;
    constexpr bn::fixed frames_per_shoot_frame = 3.5;
}

hand::hand(const candy_gfx& candy_gfx) :
    _candy_sprite(bn::sprite_ptr::create(
            0, 0, bn::sprite_tiles_items::gvsnb_candy_big_shape_size,
            bn::sprite_tiles_items::gvsnb_candy_big.create_tiles(), candy_gfx.palettes[0])),
    _hand_sprite(bn::sprite_items::gvsnb_hand.create_sprite(0, 48, 0))
{
    _candy_sprite.set_z_order(utils::hand_z_order);
    _hand_sprite.set_z_order(utils::hand_z_order);
}

bool hand::update(bn::fixed tempo, bool active)
{
    bool shoot = false;

    if(active)
    {
        bool left = bn::keypad::left_held();

        if(left || bn::keypad::right_held())
        {
            bn::fixed x_inc = move_speed.unsafe_multiplication(tempo);

            if(left)
            {
                x_inc = -x_inc;
            }

            _x = bn::clamp(_x + x_inc, bn::fixed(-utils::door_x), bn::fixed(utils::door_x));
            _hand_sprite.set_x(_x.unsafe_multiplication(utils::x_mult(candy_y)));
        }

        if(_shoot_frames < frames_per_shoot_frame)
        {
            if(bn::keypad::a_pressed() || bn::keypad::b_pressed())
            {
                _shoot_frames = frames_per_shoot_frame * 4;
                _fire = true;
            }
        }
    }
    else
    {
        _fire = false;
    }

    if(_shoot_frames > 0)
    {
        _shoot_frames = bn::max(_shoot_frames - tempo, bn::fixed(0));
    }

    int tiles_index = (_shoot_frames / frames_per_shoot_frame).right_shift_integer();
    _hand_sprite.set_tiles(bn::sprite_items::gvsnb_hand.tiles_item(), tiles_index);

    switch(tiles_index)
    {

    case 0:
        _candy_sprite.set_scale(1);
        _candy_sprite.set_rotation_angle(0);
        _candy_sprite.set_position(_hand_sprite.position() + bn::fixed_point(-26, 14));
        _candy_sprite.set_visible(true);
        break;

    case 1:
        _candy_sprite.set_visible(false);
        break;

    case 2:
        _candy_sprite.set_visible(false);

        if(_fire)
        {
            shoot = true;
            _fire = false;
        }
        break;

    default:
        _candy_sprite.set_scale(0.75);
        _candy_sprite.set_rotation_angle(360 - 30);
        _candy_sprite.set_position(_hand_sprite.position() + bn::fixed_point(-8, 0));
        _candy_sprite.set_visible(true);
        break;
    }


    return shoot;
}

candy hand::create_candy(const candy_gfx& candy_gfx)
{
    candy result(_x, candy_y, _candy_sprite.palette(), candy_gfx);

    ++_candy_palette_index;
    _candy_palette_index %= candy_gfx.palettes.size();
    _candy_sprite.set_palette(candy_gfx.palettes[_candy_palette_index]);

    return result;
}

}
