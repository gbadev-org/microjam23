#ifndef MJ_CUTSCENE_H
#define MJ_CUTSCENE_H

#include "bn_bg_palettes_actions.h"
#include "bn_colors.h"
#include "bn_keypad.h"
#include "bn_music_actions.h"
#include "bn_optional.h"
#include "bn_sprite_palettes.h"
#include "bn_sprite_palettes_actions.h"

#include "mj_scene_type.h"
#include "mj_scene.h"

namespace mj
{

class core;

class cutscene : public scene
{

public:
    cutscene([[maybe_unused]] core& core, int frames, bn::color color = bn::colors::black) :
        _bgs_fader(_create_bgs_fade_in_action(frames, color)),
        _sprites_fader(_create_sprites_fade_in_action(frames, color))
    {
    }
    
    static constexpr mj::scene_type SCENE_AFTER_OPENING = scene_type::GAME_ZOOM_OUT;

protected:
    bn::optional<bn::music_volume_to_action> _music_fader;
    bn::bg_palettes_fade_to_action _bgs_fader;
    bn::sprite_palettes_fade_to_action _sprites_fader;
    int _t = 0;
    int _skip_wait_frames = 4;
    bool _skipping = false;

    [[nodiscard]] inline bool _handle_skipping(bn::optional<scene_type> &result)
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
                    result = SCENE_AFTER_OPENING;
                }
            }
            return true;
        }
        else
        {
            if (bn::keypad::any_pressed())
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
    
    // inline void _fade_out(int duration)
    // {
    //     if (duration > 0)
    //     {
    //         _bgs_fader = _create_bgs_fade_out_action(duration);
    //         _sprites_fader = _create_sprites_fade_out_action(duration);
    //     }
    // }

    [[nodiscard]] inline bn::bg_palettes_fade_to_action _create_bgs_fade_in_action(int frames, bn::color color = bn::colors::black)
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

    [[nodiscard]] inline bn::sprite_palettes_fade_to_action _create_sprites_fade_in_action(int frames, bn::color color = bn::colors::black)
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

    [[nodiscard]] inline bn::bg_palettes_fade_to_action _create_bgs_fade_out_action(int frames, bn::color color = bn::colors::black)
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

    [[nodiscard]] inline bn::sprite_palettes_fade_to_action _create_sprites_fade_out_action(int frames, bn::color color = bn::colors::black)
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

};

}

#endif
