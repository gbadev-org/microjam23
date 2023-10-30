#include "mj/mj_opening_a_scene.h"

#include "bn_bg_palettes.h"
#include "bn_bg_palettes_actions.h"
#include "bn_color.h"
#include "bn_colors.h"
#include "bn_fixed_point.h"
#include "bn_keypad.h"
#include "bn_sound_items.h"
#include "bn_sprite_palettes.h"
#include "bn_string.h"
#include "bn_version.h"

#include "mj/mj_core.h"
#include "mj/mj_cutscene.h"
#include "mj/mj_scene_type.h"

#include "bn_regular_bg_items_mj_op_a_chair.h"
#include "bn_regular_bg_items_mj_op_a_fx_01.h"
#include "bn_sprite_items_mj_op_a_fx_02.h"
#include "bn_sprite_items_mj_op_a_oldman_pipe.h"
#include "bn_regular_bg_items_mj_op_a_oldman_01.h"
#include "bn_regular_bg_items_mj_op_a_oldman_01b.h"
#include "bn_regular_bg_items_mj_op_a_oldman_02.h"
#include "bn_regular_bg_items_mj_op_a_oldman_03.h"

namespace mj
{

constexpr int FADE_IN_DURATION = 60;
constexpr int RING_AT = 120;
constexpr int FADE_OUT_AT = 220;
constexpr int FADE_OUT_DURATION = 8;

opening_a_scene::opening_a_scene(core& core) :
    cutscene(core, FADE_IN_DURATION),
    _chair(bn::regular_bg_items::mj_op_a_chair.create_bg(0, 0)),
    _oldman(bn::regular_bg_items::mj_op_a_oldman_01.create_bg(0, 0)),
    _dingdong(bn::regular_bg_items::mj_op_a_fx_01.create_bg(76, 10)),
    _exclaim(bn::sprite_items::mj_op_a_fx_02.create_sprite(60, -54)),
    _pipe(bn::sprite_items::mj_op_a_oldman_pipe.create_sprite(-82, 44))
{
    _t = 0;
    _skipping = false;
    _chair.set_z_order(2);
    _oldman.set_z_order(1);
    _dingdong.set_visible(false);
    _dingdong.set_z_order(0);
    _exclaim.set_visible(false);
}

bn::optional<scene_type> opening_a_scene::update()
{
    bn::optional<scene_type> result;
    
    if (_handle_skipping(result))
    {
        return result;
    }
    
    if (_t < FADE_IN_DURATION)
    {
        _bgs_fader.update();
        _sprites_fader.update();
    }
    
    if (_t < RING_AT)
    {
        _pipe.set_tiles(bn::sprite_items::mj_op_a_oldman_pipe.tiles_item(), (_t / 20) % 2);
    }
    else if (_t == RING_AT + 1*5)
    {
        _pipe.set_tiles(bn::sprite_items::mj_op_a_oldman_pipe.tiles_item(), 2);
    }
    else if (_t == RING_AT + 2*5)
    {
        _pipe.set_tiles(bn::sprite_items::mj_op_a_oldman_pipe.tiles_item(), 3);
    }
    else if (_t == RING_AT + 3*5)
    {
        _pipe.set_tiles(bn::sprite_items::mj_op_a_oldman_pipe.tiles_item(), 4);
    }
    else if (_t == RING_AT + 4*5)
    {
        _pipe.set_visible(false);
    }
    
    if (_t == RING_AT)
    {
        _dingdong.set_visible(true);
        bn::sound_items::mj_pause_begin.play();  // TEMP
    }
    
    if (_t == RING_AT + 6)
    {
        _oldman = bn::regular_bg_items::mj_op_a_oldman_02.create_bg(0, 0);
        _oldman.set_z_order(1);
    }
    if (_t == RING_AT + 9)
    {
        _oldman = bn::regular_bg_items::mj_op_a_oldman_03.create_bg(0, 0);
        _oldman.set_z_order(1);
        _exclaim.set_visible(true);
    }
    
    if (_t == FADE_OUT_AT)
    {
        _bgs_fader = _create_bgs_fade_out_action(FADE_OUT_DURATION);
        _sprites_fader = _create_sprites_fade_out_action(FADE_OUT_DURATION);
    }
    
    if (_t >= FADE_OUT_AT && _t < FADE_OUT_AT + FADE_OUT_DURATION)
    {
        _bgs_fader.update();
        _sprites_fader.update();
    }
    
    // result = scene_type::TITLE;
    _t++;
    return result;
}

}
