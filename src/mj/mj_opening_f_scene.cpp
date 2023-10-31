#include "mj/mj_opening_f_scene.h"

#include "bn_color.h"
#include "bn_colors.h"
#include "bn_fixed_fwd.h"
#include "bn_fixed_point.h"
#include "bn_keypad.h"
#include "bn_sprite_ptr.h"
#include "bn_string.h"
#include "bn_version.h"

#include "mj/mj_core.h"
#include "mj/mj_scene_type.h"

#include "bn_regular_bg_items_mj_op_f_house.h"
#include "bn_affine_bg_items_mj_op_f_pumpkin01.h"
#include "bn_affine_bg_items_mj_op_f_pumpkin02.h"
#include "bn_bg_palette_items_mj_op_f_palpink.h"
#include "bn_sprite_items_mj_op_f_kid1.h"
#include "bn_sprite_items_mj_op_f_kid2.h"
#include "bn_sprite_items_mj_op_f_kid3.h"
#include "bn_sprite_items_mj_op_f_oldman.h"
#include "bn_log.h"
#include "bn_memory.h"

namespace mj
{

constexpr int FADE_IN_DURATION = 2;
constexpr int PINK_AT = 27;
constexpr int SQUISH_AT = 35;
constexpr int SQUISH_DURATION = 10;
constexpr int OPEN_MOUTH_AT = SQUISH_AT + SQUISH_DURATION + 4;

constexpr int SQUISH2_AT = 185;
constexpr int SQUISH2_DURATION = 10;
constexpr int CLOSE_MOUTH_AT = SQUISH2_AT + SQUISH2_DURATION;

constexpr int UNPINK_AT = 200;

constexpr int FADE_OUT_AT = 290;
constexpr int FADE_OUT_DURATION = 20;

constexpr int PUMPKIN_X = -62;
constexpr int PUMPKIN_Y = 47;

static bn::fixed approach(bn::fixed val, bn::fixed target, bn::fixed step) {
    return (val < target)
        ? min(target, val + step)
        : max(target, val - step);
}

opening_f_scene::opening_f_scene(core& core) :
    cutscene(core, FADE_IN_DURATION),
    _house(bn::regular_bg_items::mj_op_f_house.create_bg(0, 0)),
    _pumpkin(bn::affine_bg_items::mj_op_f_pumpkin01.create_bg(PUMPKIN_X, PUMPKIN_Y)),
    _kid1(bn::sprite_items::mj_op_f_kid1.create_sprite(122-128, 180-128)),
    _kid2(bn::sprite_items::mj_op_f_kid2.create_sprite(137-128, 183-128)),
    _kid3(bn::sprite_items::mj_op_f_kid3.create_sprite(153-128, 180-128)),
    _oldman(bn::sprite_items::mj_op_f_oldman.create_sprite(172-128, 173-128)),
    _palette_item(_colors, bn::bpp_mode::BPP_8),
    _palette(bn::bg_palette_ptr::create(_palette_item)),
    _targets{_kid1.position(), _kid2.position(), _kid3.position(), _oldman.position()}
{
    // _house.set_priority(2);
    _pumpkin.set_priority(1);
    
    _house.set_z_order(2);
    _pumpkin.set_z_order(1);
    
    _kid1.set_bg_priority(0);
    _kid2.set_bg_priority(0);
    _kid3.set_bg_priority(0);
    _oldman.set_bg_priority(0);
    
    _kid1.set_z_order(0);
    _kid2.set_z_order(0);
    _kid3.set_z_order(0);
    _oldman.set_z_order(0);
    
    _mouth_open = false;
    
    _spring = 0.0;
    _spring_vel = 0.0;
    
    // _colors.fill(bn::color(0, 0, 31));
    
    bn::memory::copy(
        bn::affine_bg_items::mj_op_f_pumpkin01.palette_item().colors_ref()[0],
        bn::affine_bg_items::mj_op_f_pumpkin01.palette_item().colors_ref().size(),
        _colors.data()[0]
    );
    
    bn::span<const bn::color> colors = bn::affine_bg_items::mj_op_f_pumpkin01.palette_item().colors_ref();
    
    for (int i = 0; i < colors.size(); i++)
    {
        _colors[i] = colors[i];
    }
    _palette.set_colors(_palette_item);  // schedule colours to be copied into PAL RAM.
    
    _house.set_palette(_palette);
    _pumpkin.set_palette(_palette);
}

bn::optional<scene_type> opening_f_scene::update()
{
    bn::optional<scene_type> result;
    
    if (!_bgs_fader.done())
    {
        _bgs_fader.update();
        _sprites_fader.update();
    }
    
    if (_handle_skipping(result))
    {
        return result;
    }
    
    if (_t == OPEN_MOUTH_AT)
    {
        _pumpkin = bn::affine_bg_items::mj_op_f_pumpkin02.create_bg(-62, +47);
        _pumpkin.set_priority(1);
        _mouth_open = true;
        _spring *= -1;
        _spring_vel *= -1;
    }
    if (_t == CLOSE_MOUTH_AT)
    {
        _pumpkin = bn::affine_bg_items::mj_op_f_pumpkin01.create_bg(-62, +47);
        _pumpkin.set_priority(1);
        _mouth_open = false;
        _spring *= -1;
        _spring_vel *= -1;
    }
    
    if (_t > UNPINK_AT)
    {
        _house_blend = approach(_house_blend, 0.0, 0.02);
    }
    else if (_t > PINK_AT)
    {
        _house_blend = approach(_house_blend, 1.0, 0.02);
    }
    
    // Pumpkin jiggle
    
    if (_t < SQUISH2_AT)
    {
        if (_t >= SQUISH_AT && _t < SQUISH_AT + SQUISH_DURATION)
        {
            _spring += 17;
        }
        else
        {
            bn::fixed target = 0.0;
            _spring_vel += (target - _spring) * bn::fixed(0.5);
            _spring_vel *= bn::fixed(0.9);
            _spring += _spring_vel;
        }
    }
    else
    {
        if (_t >= SQUISH2_AT && _t < SQUISH2_AT + SQUISH2_DURATION)
        {
            _spring += 17;
        }
        else
        {
            bn::fixed target = 0.0;
            _spring_vel += (target - _spring) * bn::fixed(0.4);
            _spring_vel *= bn::fixed(0.85);
            _spring += _spring_vel;
        }
    }
    
    bn::fixed a = _spring / (1 << 10);
    _pumpkin.set_scale(1.0 + a, 1.0 - a);
    _pumpkin.set_position(PUMPKIN_X, PUMPKIN_Y + 18*a);
    
    if (_mouth_open)
    {
        // spinning
        _kid1.set_rotation_angle(wrap_angle(_kid1.rotation_angle() + 12));
        _kid2.set_rotation_angle(wrap_angle(_kid2.rotation_angle() - 11));
        _kid3.set_rotation_angle(wrap_angle(_kid3.rotation_angle() + 10));
        _oldman.set_rotation_angle(wrap_angle(_oldman.rotation_angle() - 8));
        
        // attract the sprites to the pumpkin center
        
        bn::fixed targ_x = -80;
        bn::fixed targ_y = 0;
        
        int t = _t - OPEN_MOUTH_AT;
        
        int i = 0;
        for (auto &p : _targets)
        {
            if (i*8 < t)
            {
                bn::fixed x = approach(p.x(), targ_x, 1.25);
                bn::fixed y = approach(p.y(), targ_y, 1.25);
                p = {x, y};
                
                bn::sprite_ptr *spr;
                switch (i)
                {
                    case 0: spr = &_kid1; break;
                    case 1: spr = &_kid2; break;
                    case 2: spr = &_kid3; break;
                    default: spr = &_oldman; break;
                }
                spr->set_x(spr->x() + (x - spr->x()) * bn::fixed(0.125));
                spr->set_y(spr->y() + (y - spr->y()) * bn::fixed(0.125));
                
                if (x < -67)
                {
                    bn::fixed scale = spr->horizontal_scale();
                    scale = approach(scale, 0, 1.0/16);
                    if (scale <= 0)
                    {
                        spr->set_visible(false);
                    }
                    else
                    {
                        spr->set_scale(scale);
                    }
                }
            }
            i++;
        }
    }
    
    
    // Palette shenanigans
    {
        bn::span<const bn::color> colors_a = bn::affine_bg_items::mj_op_f_pumpkin01.palette_item().colors_ref();
        bn::span<const bn::color> colors_b = bn::bg_palette_items::mj_op_f_palpink.colors_ref();
        
        for (int i = 0; i < 17; i++)
        {
            bn::color a = colors_a[i];
            bn::color b = colors_b[i];
            _colors[i].set_components(
                (a.red() * (1.0 - _house_blend) + b.red() * _house_blend).floor_integer(),
                (a.green() * (1.0 - _house_blend) + b.green() * _house_blend).floor_integer(),
                (a.blue() * (1.0 - _house_blend) + b.blue() * _house_blend).floor_integer()
            );
        }
        
        if ((_t % 8) == 0)
        {
            bn::color temp = _colors[30];
            _colors[30] = _colors[31];
            _colors[31] = _colors[32];
            _colors[32] = _colors[33];
            _colors[33] = _colors[34];
            _colors[34] = _colors[35];
            _colors[35] = temp;
        }
        _palette.set_colors(_palette_item);  // schedule colours to be copied into PAL RAM.
    }
    
    if (_t == FADE_OUT_AT)
    {
        _bgs_fader = _create_bgs_fade_out_action(FADE_OUT_DURATION);
        _sprites_fader = _create_sprites_fade_out_action(FADE_OUT_DURATION);
    }
    if (_t >= FADE_OUT_AT + FADE_OUT_DURATION)
    {
        result = scene_type::OPENING_G;
    }
    
    _t++;
    
    return result;
}

}
