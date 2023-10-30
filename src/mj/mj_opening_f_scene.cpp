#include "mj/mj_opening_f_scene.h"

#include "bn_color.h"
#include "bn_colors.h"
#include "bn_fixed_point.h"
#include "bn_keypad.h"
#include "bn_string.h"
#include "bn_version.h"

#include "mj/mj_core.h"
#include "mj/mj_scene_type.h"

#include "bn_regular_bg_items_mj_op_f_house.h"
#include "bn_regular_bg_items_mj_op_f_pumpkin.h"
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
constexpr int FADE_OUT_AT = 220;
constexpr int FADE_OUT_DURATION = 2;

opening_f_scene::opening_f_scene(core& core) :
    cutscene(core, FADE_IN_DURATION),
    _house(bn::regular_bg_items::mj_op_f_house.create_bg(0, 0)),
    _pumpkin(bn::regular_bg_items::mj_op_f_pumpkin.create_bg(0, 0)),
    _kid1(bn::sprite_items::mj_op_f_kid1.create_sprite(122-128, 180-128)),
    _kid2(bn::sprite_items::mj_op_f_kid2.create_sprite(137-128, 182-128)),
    _kid3(bn::sprite_items::mj_op_f_kid3.create_sprite(153-128, 180-128)),
    _oldman(bn::sprite_items::mj_op_f_oldman.create_sprite(172-128, 173-128)),
    _palette_item(_colors, bn::bpp_mode::BPP_8),
    _palette(bn::bg_palette_ptr::create(_palette_item))
{
    _house.set_z_order(2);
    _pumpkin.set_z_order(1);
    _kid1.set_z_order(0);
    _kid2.set_z_order(0);
    _kid3.set_z_order(0);
    _oldman.set_z_order(0);
    
    _pumpkin.set_map(bn::regular_bg_items::mj_op_f_pumpkin.map_item(), 1);
    
    // _colors.fill(bn::color(0, 0, 31));
    
    bn::memory::copy(
        bn::regular_bg_items::mj_op_f_pumpkin.palette_item().colors_ref()[0],
        bn::regular_bg_items::mj_op_f_pumpkin.palette_item().colors_ref().size(),
        _colors.data()[0]
    );
    
    bn::span<const bn::color> colors = bn::regular_bg_items::mj_op_f_pumpkin.palette_item().colors_ref();
    
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
    
    if (_t == FADE_OUT_AT)
    {
        _bgs_fader = _create_bgs_fade_out_action(FADE_OUT_DURATION);
        _sprites_fader = _create_sprites_fade_out_action(FADE_OUT_DURATION);
    }
    if (_t >= FADE_OUT_AT + FADE_OUT_DURATION)
    {
        result = scene_type::OPENING_F;
    }
    
    _t++;
    
    return result;
}

}
