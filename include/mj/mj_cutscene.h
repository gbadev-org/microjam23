#ifndef MJ_CUTSCENE_H
#define MJ_CUTSCENE_H

#include "bn_bg_palettes_actions.h"
#include "bn_color.h"
#include "bn_music_actions.h"
#include "bn_sprite_palettes_actions.h"

#include "mj_scene_type.h"
#include "mj_scene.h"

namespace mj
{

class core;

class cutscene : public scene
{

public:
    cutscene(core& core, int frames, bn::color color = bn::color());
    
    static constexpr mj::scene_type SCENE_AFTER_OPENING = scene_type::GAME_ZOOM_OUT;

protected:
    core& _core;
    bn::optional<bn::music_volume_to_action> _music_fader;
    bn::bg_palettes_fade_to_action _bgs_fader;
    bn::sprite_palettes_fade_to_action _sprites_fader;
    int _t = 0;
    int _skip_wait_frames = 4;
    bool _skipping = false;

    [[nodiscard]] bool _handle_skipping(bn::optional<scene_type> &result);

    [[nodiscard]] bn::bg_palettes_fade_to_action _create_bgs_fade_in_action(
            int frames, bn::color color = bn::color());

    [[nodiscard]] bn::sprite_palettes_fade_to_action _create_sprites_fade_in_action(
            int frames, bn::color color = bn::color());

    [[nodiscard]] bn::bg_palettes_fade_to_action _create_bgs_fade_out_action(
            int frames, bn::color color = bn::color());

    [[nodiscard]] bn::sprite_palettes_fade_to_action _create_sprites_fade_out_action(
            int frames, bn::color color = bn::color());
};

}

#endif
