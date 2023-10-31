#include "mj/mj_cutscene.h"

#include "bn_bg_palettes_actions.h"
#include "bn_blending.h"
#include "bn_colors.h"
#include "bn_keypad.h"
#include "bn_sound.h"

#include "mj/mj_core.h"

namespace mj
{

cutscene::cutscene(core& core, int frames, bn::color color) :
    _core(core),
    _bgs_fader(_create_bgs_fade_in_action(frames, color)),
    _sprites_fader(_create_sprites_fade_in_action(frames, color))
{
}

bool cutscene::_handle_skipping(bn::optional<scene_type> &result)
{
    if (_skipping)
    {
        if (_bgs_fader.done())
        {
            if (_skip_wait_frames > 0)
            {
                _skip_wait_frames--;
            }
            else
            {
                bn::blending::restore();
                bn::bg_palettes::set_transparent_color(bn::colors::white);
                bn::sound::stop_all();
                result = SCENE_AFTER_OPENING;
            }
        }
        return true;
    }
    else
    {
        if (_core.sram_data().allow_intro_skip() && bn::keypad::a_pressed())
        {
            // Skip the cutscenes.
            constexpr int fade_frames = 32;
            bn::bg_palettes::set_fade(bn::colors::white, 0.0);
            bn::sprite_palettes::set_fade(bn::colors::white, 0.0);
            _bgs_fader = bn::bg_palettes_fade_to_action(fade_frames, 1.0);
            _sprites_fader = bn::sprite_palettes_fade_to_action(fade_frames, 1.0);
            _music_fader = bn::music_volume_to_action(fade_frames, 0);
            _skipping = true;
        }
        return false;
    }
}

bn::bg_palettes_fade_to_action cutscene::_create_bgs_fade_in_action(int frames, bn::color color)
{
    if (frames > 0)
    {
        bn::bg_palettes::set_fade(color, 1.0);
        return bn::bg_palettes_fade_to_action(frames, 0.0);
    }
    else
    {
        auto result = bn::bg_palettes_fade_to_action(1, bn::bg_palettes::fade_intensity());
        result.update();
        return result;
    }
}

bn::sprite_palettes_fade_to_action cutscene::_create_sprites_fade_in_action(int frames, bn::color color)
{
    if (frames > 0)
    {
        bn::sprite_palettes::set_fade(color, 1.0);
        return bn::sprite_palettes_fade_to_action(frames, 0.0);
    }
    else
    {
        auto result = bn::sprite_palettes_fade_to_action(1, bn::sprite_palettes::fade_intensity());
        result.update();
        return result;
    }
}

bn::bg_palettes_fade_to_action cutscene::_create_bgs_fade_out_action(int frames, bn::color color)
{
    if (frames > 0)
    {
        bn::bg_palettes::set_fade(color, 0.0);
        return bn::bg_palettes_fade_to_action(frames, 1.0);
    }
    else
    {
        auto result = bn::bg_palettes_fade_to_action(1, bn::bg_palettes::fade_intensity());
        result.update();
        return result;
    }
}

bn::sprite_palettes_fade_to_action cutscene::_create_sprites_fade_out_action(int frames, bn::color color)
{
    if (frames > 0)
    {
        bn::sprite_palettes::set_fade(color, 0.0);
        return bn::sprite_palettes_fade_to_action(frames, 1.0);
    }
    else
    {
        auto result = bn::sprite_palettes_fade_to_action(1, bn::sprite_palettes::fade_intensity());
        result.update();
        return result;
    }
}

}
