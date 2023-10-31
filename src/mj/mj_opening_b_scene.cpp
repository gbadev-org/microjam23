#include "mj/mj_opening_b_scene.h"

#include "bn_blending.h"
#include "bn_math.h"
#include "bn_sound_items.h"
#include "bn_music_items.h"

#include "mj/mj_core.h"
#include "mj/mj_scene_type.h"

#include "bn_regular_bg_items_mj_op_b_fx.h"
#include "bn_regular_bg_items_mj_op_b_house.h"
#include "bn_regular_bg_items_mj_op_b_kids.h"
#include "bn_regular_bg_items_mj_op_b_oldman.h"

namespace mj
{
    
constexpr int FADE_IN_DURATION = 8;  // 30
constexpr int START_SCROLLING_AT = 34;
constexpr int OPEN_DOOR_AT       = 114 + 100;
constexpr int FADE_MAN_AT        = 114 + 108;
constexpr int TRICK_OR_TREAT_AT  = 114 + 140;
constexpr int FADE_OUT_AT        = 114 + 250;
constexpr int FADE_OUT_DURATION  = 8;  // 20

opening_b_scene::opening_b_scene(core& core) :
    cutscene(core, FADE_IN_DURATION),
    _house(bn::regular_bg_items::mj_op_b_house.create_bg(0, 0)),
    _oldman(bn::regular_bg_items::mj_op_b_oldman.create_bg(0, 0)),
    _trickortreat(bn::regular_bg_items::mj_op_b_fx.create_bg(0, -28)),
    _kids(bn::regular_bg_items::mj_op_b_kids.create_bg(0, 0))
{
    _trickortreat.set_visible(false);
    _oldman.set_visible(false);
    
    _house.set_z_order(3);
    _oldman.set_z_order(2);
    _kids.set_z_order(1);
    _trickortreat.set_z_order(0);
    
    _scrolly = 72;
    
    bn::music_items::mj_wind.play(0.3);
}

bn::optional<scene_type> opening_b_scene::update()
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
    
    if (_t >= START_SCROLLING_AT)
    {
        if (_scrolly > 0)
        {
            _scrolly -= 0.5;
        }
    }
    
    _house.set_y(_scrolly + 0);
    _oldman.set_y(_scrolly + 0);
    
    if (_t == TRICK_OR_TREAT_AT)
    {
        bn::sound_items::mj_trick_or_treat.play();
    }
    
    if (_t <= TRICK_OR_TREAT_AT)
    {
        int dy = ((_t / 20) % 2) * 2;
        _kids.set_y(_scrolly + dy);
    }
    else
    {
        int dy = ((_t / 10) % 2) * 1;
        _kids.set_y(_scrolly + dy);
    }
    
    if (_t == OPEN_DOOR_AT)
    {
        bn::sound_items::mj_door_open.play();
        _house.set_map(bn::regular_bg_items::mj_op_b_house.map_item(), 1);
    }
    
    if (_t == FADE_MAN_AT)
    {
        _oldman.set_visible(true);
        _oldman.set_blending_enabled(true);
        bn::blending::set_transparency_alpha(0.2);
        _kids.set_z_order(1);
        _trickortreat.set_z_order(0);
    }
    if (_t == FADE_MAN_AT+1)
    {
        bn::blending::set_transparency_alpha(0.4);
    }
    if (_t == FADE_MAN_AT+2)
    {
        bn::blending::set_transparency_alpha(0.6);
    }
    if (_t == FADE_MAN_AT+3)
    {
        bn::blending::set_transparency_alpha(0.8);
    }
    if (_t == FADE_MAN_AT+4)
    {
        bn::sound_items::mj_footstep.play(1.0);
        bn::blending::set_transparency_alpha(1.0);
    }
    
    if (_t == TRICK_OR_TREAT_AT)
    {
        _trickortreat.set_visible(true);
    }
    
    bn::fixed s = bn::sin(bn::fixed(_t) / 32);
    _trickortreat.set_y(_scrolly - 30 + s*2);
    
    if (_t == FADE_OUT_AT)
    {
        _bgs_fader = _create_bgs_fade_out_action(FADE_OUT_DURATION);
        _sprites_fader = _create_sprites_fade_out_action(FADE_OUT_DURATION);
    }
    if (_t >= FADE_OUT_AT + FADE_OUT_DURATION)
    {
        result = scene_type::OPENING_C;
    }
    
    _t++;
    
    return result;
}

}
