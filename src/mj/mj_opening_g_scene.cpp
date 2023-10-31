#include "mj/mj_opening_g_scene.h"

#include "bn_blending.h"
#include "bn_colors.h"
#include "bn_fixed_fwd.h"
#include "bn_math.h"
#include "bn_music_items.h"

#include "mj/mj_core.h"
#include "mj/mj_scene_type.h"
#include "bn_memory.h"
#include "bn_regular_bg_items_mj_op_g_rainbow.h"
#include "bn_regular_bg_items_mj_op_g_rainbow.h"
#include "bn_sprite_items_mj_op_g_kid1.h"
#include "bn_sprite_items_mj_op_g_kid2.h"
#include "bn_sprite_items_mj_op_g_kid3.h"
#include "bn_sprite_items_mj_op_g_oldman.h"

namespace mj
{
    
constexpr int FADE_IN_DURATION = 42;
constexpr int FADE_OUT_AT = 250;
constexpr int FADE_OUT_DURATION = 42;

static bn::fixed approach(bn::fixed val, bn::fixed target, bn::fixed step) {
    return (val < target)
        ? min(target, val + step)
        : max(target, val - step);
}

opening_g_scene::opening_g_scene(core& core) :
    cutscene(core, FADE_IN_DURATION),
    // _vortex(bn::regular_bg_items::mj_op_g_vortex1.create_bg(0, 0)),
    _rainbow(bn::regular_bg_items::mj_op_g_rainbow.create_bg(0, 0)),
    _palette_item(_colors, bn::bpp_mode::BPP_8),
    _palette(bn::bg_palette_ptr::create(_palette_item)),
    _sprites{
        bn::sprite_items::mj_op_g_kid1.create_sprite(-61, 29-10),
        bn::sprite_items::mj_op_g_kid2.create_sprite(4, 49),
        bn::sprite_items::mj_op_g_kid3.create_sprite(67, 24-10),
        bn::sprite_items::mj_op_g_oldman.create_sprite(-2, -39)
    },
    _rotvels{
        +6,
        -5,
        +5,
        -4
    },
    _scales{
        1.2,
        1.2,
        1.4,
        1.5
    }
{
    _rainbow.set_priority(2);
    // _vortex.set_priority(1);
    
    bn::memory::copy(
        bn::regular_bg_items::mj_op_g_rainbow.palette_item().colors_ref()[0],
        bn::regular_bg_items::mj_op_g_rainbow.palette_item().colors_ref().size(),
        _colors.data()[0]
    );
    
    bn::span<const bn::color> colors = bn::regular_bg_items::mj_op_g_rainbow.palette_item().colors_ref();
    
    for (int i = 0; i < colors.size(); i++)
    {
        _colors[i] = colors[i];
    }
    _palette.set_colors(_palette_item);  // schedule colours to be copied into PAL RAM.
    
    _rainbow.set_palette(_palette);
    // _vortex.set_palette(_palette);
    
    // separate the sprites some more.
    for (int i = 0; i < 4; i++)
    {
        bn::sprite_ptr &spr = _sprites[i];
        spr.set_bg_priority(0);
        bn::fixed x = spr.x();
        bn::fixed y = spr.y();
        y -= 120;
        x *= bn::fixed(1.8);
        spr.set_position(x, y);
        spr.set_scale(_scales[i]);
    }
    
    bn::music_items::mj_vortex.play(0.5);
    
}

static inline bn::fixed wrap_angle(bn::fixed a)
{
    if (a < 0)
    {
        a += 360;
    }
    else if (a >= 360)
    {
        a -= 360;
    }
    return a;
}

bn::optional<scene_type> opening_g_scene::update()
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
    
    for (int i = 0; i < 4; i++)
    {
        bn::sprite_ptr &spr = _sprites[i];
        bn::fixed x = spr.x();
        bn::fixed y = spr.y();
        y += 1;
        // x *= (-x * bn::fixed(0.0003));
        x = approach(x, 0, bn::abs(x) / 100);
        spr.set_position(x, y);
        spr.set_rotation_angle(wrap_angle(spr.rotation_angle() + _rotvels[i]));
        _scales[i] = approach(_scales[i], 0, bn::fixed(1)/200);
        if (_scales[i] > 0)
        {
            spr.set_scale(_scales[i]);
        }
        else
        {
            spr.set_visible(false);
        }
    }
    
    // palette cycling
    
    if ((_t % 4) == 0)
    {
        constexpr int start_index = 2;
        constexpr int end_index = 47;
        
        // bn::color temp = _colors[start_index];
        // for (int i = start_index; i < end_index; i++)
        // {
        //     _colors[i] = _colors[i+1];
        // }
        // _colors[end_index] = temp;
        
        bn::color temp = _colors[end_index];
        for (int i = end_index; i > start_index; i--)
        {
            _colors[i] = _colors[i-1];
        }
        _colors[start_index] = temp;
        
        _palette.set_colors(_palette_item);  // schedule colours to be copied into PAL RAM.
    }
    
    if (_t == FADE_OUT_AT)
    {
        _bgs_fader = _create_bgs_fade_out_action(FADE_OUT_DURATION, bn::colors::white);
        _sprites_fader = _create_sprites_fade_out_action(FADE_OUT_DURATION, bn::colors::white);
    }
    if (_t >= FADE_OUT_AT + FADE_OUT_DURATION)
    {
        bn::blending::restore();
        result = SCENE_AFTER_OPENING;
    }
    
    _t++;
    
    return result;
}

}
