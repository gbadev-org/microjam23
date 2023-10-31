#ifndef MJ_OPENING_E_SCENE_H
#define MJ_OPENING_E_SCENE_H

#include "bn_regular_bg_ptr.h"

#include "mj_cutscene.h"

namespace mj
{

class core;

class opening_e_scene : public cutscene
{

public:
    explicit opening_e_scene(core& core);

    [[nodiscard]] bn::optional<scene_type> update() final;

private:
    bn::regular_bg_ptr _bg;
    bn::regular_bg_ptr _pumpkin;
    bn::regular_bg_ptr _pumpkin_face;
    bn::regular_bg_ptr _kids;
    bn::fixed _y, _vely;
};

}

#endif
