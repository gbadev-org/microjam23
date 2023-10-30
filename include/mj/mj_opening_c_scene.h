#ifndef MJ_OPENING_C_SCENE_H
#define MJ_OPENING_C_SCENE_H

#include "bn_bg_palettes_actions.h"
#include "bn_regular_bg_ptr.h"
#include "bn_sprite_palettes_actions.h"
#include "bn_sprite_ptr.h"
#include "bn_vector.h"

#include "mj_scene.h"
#include "mj_cutscene.h"

namespace mj
{

class core;

class opening_c_scene : public cutscene
{

public:
    explicit opening_c_scene(core& core);

    [[nodiscard]] bn::optional<scene_type> update() final;

private:
    bn::regular_bg_ptr _trickortreaters;
};

}

#endif
