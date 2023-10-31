#ifndef MJ_OPENING_F_SCENE_H
#define MJ_OPENING_F_SCENE_H

#include "bn_affine_bg_map_ptr.h"
#include "bn_bg_palettes_actions.h"
#include "bn_fixed_fwd.h"
#include "bn_fixed_point.h"
#include "bn_regular_bg_ptr.h"
#include "bn_affine_bg_ptr.h"
#include "bn_sprite_palettes_actions.h"
#include "bn_sprite_ptr.h"
#include "bn_vector.h"
#include "bn_bg_palette_ptr.h"
#include "bn_bg_palette_item.h"

#include "mj_scene.h"
#include "mj_cutscene.h"

namespace mj
{

class core;

class opening_f_scene : public cutscene
{

public:
    explicit opening_f_scene(core& core);

    [[nodiscard]] bn::optional<scene_type> update() final;
    
    static inline bn::fixed wrap_angle(bn::fixed a)
    {
        if (a < 0)
        {
            a += 360;
        }
        else if (a >= 360)
        {
            a -= 360;
        }
        return a;
    }
    

private:
    bn::regular_bg_ptr _house;
    bn::affine_bg_ptr _pumpkin;
    bn::sprite_ptr _kid1;
    bn::sprite_ptr _kid2;
    bn::sprite_ptr _kid3;
    bn::sprite_ptr _oldman;
    
    bn::array<bn::color, 128> _colors;
    bn::bg_palette_item _palette_item;
    bn::bg_palette_ptr _palette;

    bool _mouth_open;
    
    bn::fixed _house_blend;
    int _house_cycle;
    
    bn::fixed _spring;
    bn::fixed _spring_vel;
    
    bn::fixed_point _targets[4];
    
};

}

#endif
