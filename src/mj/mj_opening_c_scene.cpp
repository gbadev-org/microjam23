#include "mj/mj_opening_c_scene.h"

#include "bn_sound_items.h"

#include "mj/mj_core.h"
#include "mj/mj_scene_type.h"

#include "bn_regular_bg_items_mj_op_c_trickortreaters.h"
#include "bn_bg_palette_items_mj_op_c_pal0.h"
#include "bn_bg_palette_items_mj_op_c_pal1.h"
#include "bn_bg_palette_items_mj_op_c_pal2.h"
#include "bn_bg_palette_ptr.h"

namespace mj
{

constexpr int FADE_IN_DURATION = 4;
constexpr int FADE_OUT_AT = 150;
constexpr int FADE_OUT_DURATION = 10; // 20

opening_c_scene::opening_c_scene(core& core) :
    cutscene(core, FADE_IN_DURATION),
    _trickortreaters(bn::regular_bg_items::mj_op_c_trickortreaters.create_bg(0, 0))
{
}

bn::optional<scene_type> opening_c_scene::update()
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
    
    bn::bg_palette_ptr pal = _trickortreaters.palette();
    
    // TODO: interpolate smoothly instead of flickering (or just make better palettes)
    
    // if ((_t / 4) % 2 == 0)
    // {
    //     pal.set_colors(bn::bg_palette_items::mj_op_c_pal1);
    // }
    // else
    // {
    //     pal.set_colors(bn::bg_palette_items::mj_op_c_pal2);
    // }
    
    switch ((_t / 6) % 4)
    {
        case 0: pal.set_colors(bn::bg_palette_items::mj_op_c_pal0); break;
        case 1: pal.set_colors(bn::bg_palette_items::mj_op_c_pal1); break;
        case 2: pal.set_colors(bn::bg_palette_items::mj_op_c_pal2); break;
        case 3: pal.set_colors(bn::bg_palette_items::mj_op_c_pal1); break;
        default: break;
    }
    
    // pal.set_colors(const bg_palette_item &palette_item)
    
    // switch ((_t / 4) % 2)
    // {
    //     case 0: pal.set_colors(bn::bg_palette_items::mj_op_c_pal1); break;
    //     case 1: pal.set_colors(bn::bg_palette_items::mj_op_c_pal2); break;
    // }
    
    if (_t == FADE_OUT_AT - 65)
    {
        bn::sound_items::mj_stomps.play(0.6);
    }
    
    if (_t == FADE_OUT_AT)
    {
        _bgs_fader = _create_bgs_fade_out_action(FADE_OUT_DURATION);
        _sprites_fader = _create_sprites_fade_out_action(FADE_OUT_DURATION);
    }
    if (_t >= FADE_OUT_AT + FADE_OUT_DURATION)
    {
        result = scene_type::OPENING_D;
    }
    
    _t++;
    
    return result;
}

}
