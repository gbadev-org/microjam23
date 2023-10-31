#include "mj/mj_opening_d_scene.h"

#include "bn_blending.h"
#include "bn_colors.h"
#include "bn_fixed_point.h"
#include "bn_keypad.h"
#include "bn_math.h"
#include "bn_music.h"
#include "bn_string.h"
#include "bn_version.h"
#include "bn_sound_items.h"
#include "bn_music_items.h"

#include "mj/mj_core.h"
#include "mj/mj_scene_type.h"

#include "bn_regular_bg_items_mj_op_d_oldmanface.h"
#include "bn_regular_bg_items_mj_op_d_shadow.h"
#include <cinttypes>

namespace mj
{
    
constexpr int FADE_IN_DURATION = 4; // 10;
constexpr int FADE_OUT_AT = 300;
constexpr int FADE_OUT_DURATION = 8; // 20;

opening_d_scene::opening_d_scene(core& core) :
    cutscene(core, FADE_IN_DURATION),
    _oldmanface(bn::regular_bg_items::mj_op_d_oldmanface.create_bg(0, 0)),
    _shadow(bn::regular_bg_items::mj_op_d_shadow.create_bg(0, 0))
{
    _oldmanface.set_z_order(1);
    _shadow.set_z_order(0);
    
    _y = 50;
    _ytimer = 90;
    _yvel = -1.5;
    _shadow.set_blending_enabled(true);
    bn::blending::set_transparency_alpha(0.4);
    
    bn::music::stop();
    bn::music_items::mj_wind_alt.play(0.3);
}

bn::optional<scene_type> opening_d_scene::update()
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
    
    if (_ytimer > 0)
    {
        _ytimer--;
        if (_ytimer == 0)
        {
            _ytimer = 80;
            _yvel = -1.5;
        }
    }
    
    _yvel = (_yvel < 0) ? (_yvel + 0.02) : 0;
    
    _y += _yvel;
    _shadow.set_y(_y + bn::sin(_t * 0.025) * bn::fixed(1.25));
    
    if (_t == 110)
    {
        _oldmanface.set_map(bn::regular_bg_items::mj_op_d_oldmanface.map_item(), 1);
    }
    if (_t == 170)
    {
        _oldmanface.set_map(bn::regular_bg_items::mj_op_d_oldmanface.map_item(), 2);
    }
    
    if (_t == FADE_OUT_AT - 10)
    {
        bn::sound_items::mj_scare.play(0.6);
    }
    
    if (_t == FADE_OUT_AT)
    {
        _bgs_fader = _create_bgs_fade_out_action(FADE_OUT_DURATION);
        _sprites_fader = _create_sprites_fade_out_action(FADE_OUT_DURATION);
    }
    if (_t >= FADE_OUT_AT + FADE_OUT_DURATION)
    {
        bn::blending::set_transparency_alpha(1.0);
        result = scene_type::OPENING_E;
    }
    
    _t++;
    
    return result;
}

}
