#include "mj/mj_opening_e_scene.h"

#include "bn_blending.h"
#include "bn_colors.h"
#include "bn_fixed_fwd.h"
#include "bn_fixed_point.h"
#include "bn_keypad.h"
#include "bn_string.h"
#include "bn_version.h"

#include "mj/mj_core.h"
#include "mj/mj_scene_type.h"

#include "bn_regular_bg_items_mj_op_e_bg.h"
#include "bn_regular_bg_items_mj_op_e_kids.h"
#include "bn_regular_bg_items_mj_op_e_pumpkin.h"
#include "bn_regular_bg_items_mj_op_e_pumpkin_face.h"

namespace mj
{
    
constexpr int FADE_IN_DURATION = 32;
constexpr int FADE_IN_PUMPKIN_AT = 50;
constexpr int RAISE_Y_AT = 50;
constexpr int FADE_OUT_AT = 150;
constexpr int FADE_OUT_DURATION = 2; // 20;

opening_e_scene::opening_e_scene(core& core) :
    cutscene(core, FADE_IN_DURATION),
    _bg(bn::regular_bg_items::mj_op_e_bg.create_bg(0, 0)),
    _pumpkin(bn::regular_bg_items::mj_op_e_pumpkin.create_bg(0, 0)),
    _pumpkin_face(bn::regular_bg_items::mj_op_e_pumpkin_face.create_bg(0, 0)),
    _kids(bn::regular_bg_items::mj_op_e_kids.create_bg(0, 0))
{
    _bg.set_blending_enabled(true);
    _pumpkin.set_blending_enabled(true);
    bn::blending::set_fade_alpha(1.0);
    bn::blending::set_fade_color(bn::blending::fade_color_type::BLACK);
    
    _bg.set_z_order(3);
    _pumpkin.set_z_order(2);
    _pumpkin_face.set_z_order(1);
    _kids.set_z_order(0);
    
    _y = 20;
    _vely = 0;
}

bn::optional<scene_type> opening_e_scene::update()
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
    
    if (_t >= FADE_IN_PUMPKIN_AT)
    {
        bn::fixed a = bn::blending::fade_alpha() - 1.0/32;
        if (a < 0) a = 0; 
        bn::blending::set_fade_alpha(a);
    }
    
    if (_t >= RAISE_Y_AT && _t < RAISE_Y_AT + 12)
    {
        _vely -= 0.05;
    }
    else
    {
        _vely = (_vely < 0) ? (_vely + 0.017) : (0);
    }
    _y += _vely;
    
    _pumpkin.set_y(_y);
    _pumpkin_face.set_y(_y);
    
    if (_t == FADE_OUT_AT)
    {
        _bgs_fader = _create_bgs_fade_out_action(FADE_OUT_DURATION);
        _sprites_fader = _create_sprites_fade_out_action(FADE_OUT_DURATION);
    }
    if (_t >= FADE_OUT_AT + FADE_OUT_DURATION)
    {
        bn::blending::set_fade_alpha(1.0);
        result = scene_type::OPENING_F;
    }
    
    _t++;
    
    return result;
}

}
