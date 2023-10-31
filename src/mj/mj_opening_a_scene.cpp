#include "mj/mj_opening_a_scene.h"

#include "bn_bg_palettes_actions.h"
#include "bn_color.h"
#include "bn_music_actions.h"
#include "bn_sound_items.h"
#include "bn_music_items.h"

#include "mj/mj_core.h"
#include "mj/mj_cutscene.h"
#include "mj/mj_scene_type.h"

#include "bn_regular_bg_items_mj_op_a_chair.h"
#include "bn_regular_bg_items_mj_op_a_fx_01.h"
#include "bn_sprite_items_mj_op_a_fx_02.h"
#include "bn_sprite_items_mj_op_a_oldman_pipe.h"
#include "bn_regular_bg_items_mj_op_a_oldman_01.h"
#include "bn_regular_bg_items_mj_op_a_oldman_02.h"
#include "bn_regular_bg_items_mj_op_a_oldman_03.h"

namespace mj
{

constexpr int FADE_IN_DURATION = 60;
constexpr int RING_AT = 120;
constexpr int FADE_OUT_AT = 220;
// constexpr int FADE_OUT_DURATION = 8;
constexpr int FADE_OUT_DURATION = 4;

opening_a_scene::opening_a_scene(core& core) :
    cutscene(core, FADE_IN_DURATION),
    _chair(bn::regular_bg_items::mj_op_a_chair.create_bg(0, 0)),
    _oldman(bn::regular_bg_items::mj_op_a_oldman_01.create_bg(0, 0)),
    _dingdong(bn::regular_bg_items::mj_op_a_fx_01.create_bg(76, 10)),
    _exclaim(bn::sprite_items::mj_op_a_fx_02.create_sprite(60, -54)),
    _pipe(bn::sprite_items::mj_op_a_oldman_pipe.create_sprite(-82, 44))
{
    _chair.set_z_order(2);
    _oldman.set_z_order(1);
    _dingdong.set_visible(false);
    _dingdong.set_z_order(0);
    _exclaim.set_visible(false);
    
    bn::music_items::mj_fireplace.play(0.5);
}

bn::optional<scene_type> opening_a_scene::update()
{
    bn::optional<scene_type> result;
    
    if (!_bgs_fader.done())
    {
        _bgs_fader.update();
        _sprites_fader.update();
    }
    
    if (_music_fader.has_value() && !_music_fader.value().done())
    {
        _music_fader.value().update();
    }
    
    if (_handle_skipping(result))
    {
        return result;
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
        bn::sound_items::mj_doorbell.play(0.5);
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
    
    if (_t == FADE_OUT_AT - 30)
    {
        _music_fader = bn::music_volume_to_action(32, 0);
    }
    if (_t == FADE_OUT_AT)
    {
        _bgs_fader = _create_bgs_fade_out_action(FADE_OUT_DURATION);
        _sprites_fader = _create_sprites_fade_out_action(FADE_OUT_DURATION);
    }
    
    if (_t >= FADE_OUT_AT + FADE_OUT_DURATION)
    {
        result = scene_type::OPENING_B;
    }
    
    _t++;
    return result;
}

}
