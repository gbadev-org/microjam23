#ifndef MJ_CUTSCENE_H
#define MJ_CUTSCENE_H

#include "bn_bg_palettes_actions.h"
#include "bn_colors.h"
#include "bn_regular_bg_ptr.h"
#include "bn_sprite_palettes_actions.h"
#include "bn_sprite_ptr.h"
#include "bn_vector.h"

#include "mj_scene_type.h"
#include "mj_scene.h"

namespace mj
{

class core;

class cutscene : public scene
{

public:
    cutscene(core& core, int frames, bn::color color = bn::colors::black) :
    _bgs_fader(_create_bgs_fade_in_action(frames, color)),
    _sprites_fader(_create_sprites_fade_in_action(frames, color)),
    _skipping(false)
    {
    }
    
    static constexpr mj::scene_type SCENE_AFTER_OPENING = scene_type::GAME;

protected:
    bn::bg_palettes_fade_to_action _bgs_fader;
    bn::sprite_palettes_fade_to_action _sprites_fader;
    bool _skipping;

    [[nodiscard]] inline bool _handle_skipping(bn::optional<scene_type> &result)
    {
        if (_skipping)
        {
            if (_bgs_fader.done())
            {
                result = SCENE_AFTER_OPENING;
            }
            else
            {
                _bgs_fader.update();
                _sprites_fader.update();
            }
            return true;
        }
        else
        {
            // TODO: check for skipping
            return false;
        }
    }

    [[nodiscard]] inline bn::bg_palettes_fade_to_action _create_bgs_fade_in_action(int frames, bn::color color = bn::colors::black)
    {
        bn::bg_palettes::set_fade(color, 1.0);
        return bn::bg_palettes_fade_to_action(frames, 0.0);
    }

    [[nodiscard]] inline bn::sprite_palettes_fade_to_action _create_sprites_fade_in_action(int frames, bn::color color = bn::colors::black)
    {
        bn::sprite_palettes::set_fade(color, 1.0);
        return bn::sprite_palettes_fade_to_action(frames, 0.0);
    }

    [[nodiscard]] inline bn::bg_palettes_fade_to_action _create_bgs_fade_out_action(int frames, bn::color color = bn::colors::black)
    {
        bn::bg_palettes::set_fade(color, 0.0);
        return bn::bg_palettes_fade_to_action(frames, 1.0);
    }

    [[nodiscard]] inline bn::sprite_palettes_fade_to_action _create_sprites_fade_out_action(int frames, bn::color color = bn::colors::black)
    {
        bn::sprite_palettes::set_fade(color, 0.0);
        return bn::sprite_palettes_fade_to_action(frames, 1.0);
    }

};

}

#endif
