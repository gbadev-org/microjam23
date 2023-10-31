#ifndef MJ_OPENING_G_SCENE_H
#define MJ_OPENING_G_SCENE_H

#include "bn_array.h"
#include "bn_bg_palette_item.h"
#include "bn_bg_palette_ptr.h"
#include "bn_fixed_point.h"
#include "bn_regular_bg_ptr.h"
#include "bn_sprite_ptr.h"

#include "mj_cutscene.h"

namespace mj
{

class core;

class opening_g_scene : public cutscene
{

public:
    explicit opening_g_scene(core& core);

    [[nodiscard]] bn::optional<scene_type> update() final;

private:
    // bn::regular_bg_ptr _vortex;
    bn::regular_bg_ptr _rainbow;
    bn::array<bn::color, 128> _colors;
    bn::bg_palette_item _palette_item;
    bn::bg_palette_ptr _palette;
    
    bn::sprite_ptr _sprites[4];
    bn::fixed_point _vels[4];
    bn::fixed _rotvels[4];
    bn::fixed _scales[4];
};

}

#endif
