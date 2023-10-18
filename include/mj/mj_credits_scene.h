#ifndef MJ_CREDITS_SCENE_H
#define MJ_CREDITS_SCENE_H

#include "bn_affine_bg_attributes.h"
#include "bn_affine_bg_attributes_hbe_ptr.h"
#include "bn_affine_bg_dx_register_hbe_ptr.h"
#include "bn_affine_bg_dy_register_hbe_ptr.h"
#include "bn_affine_bg_pa_register_hbe_ptr.h"
#include "bn_affine_bg_ptr.h"
#include "bn_blending_transparency_attributes.h"
#include "bn_blending_transparency_attributes_hbe_ptr.h"
#include "bn_sprite_animate_actions.h"

#include "mj_credits.h"
#include "mj_scene.h"

namespace mj
{

class core;

class credits_scene final : public scene
{

public:
    explicit credits_scene(core& core);

    [[nodiscard]] bn::optional<scene_type> update() final;

private:
    alignas(int) int16_t _pa_values[160];
    alignas(int) int _dx_values[160];
    alignas(int) int _dy_values[160];
    bn::blending_transparency_attributes _transparency_attributes[160];
    bn::affine_bg_map_ptr _land_map;
    bn::affine_bg_map_ptr _sky_map;
    bn::affine_bg_ptr _pumpkin_bg;
    bn::affine_bg_ptr _bg;
    bn::affine_bg_pa_register_hbe_ptr _pa_hbe;
    bn::affine_bg_dx_register_hbe_ptr _dx_hbe;
    bn::affine_bg_dy_register_hbe_ptr _dy_hbe;
    bn::vector<bn::affine_bg_attributes, 160> _bg_attributes;
    bn::optional<bn::affine_bg_attributes_hbe_ptr> _bg_attributes_hbe;
    bn::sprite_animate_action<4> _pumpkin_animation;
    mj::credits _credits;
    bn::fixed _camera_z;
    int _elapsed_frames = 0;
    bn::blending_transparency_attributes_hbe_ptr _transparency_attributes_hbe;

    void _update_gfx();

    void _update_hbe_values();
};

}

#endif
