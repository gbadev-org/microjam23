#ifndef MJ_OPENING_B_SCENE_H
#define MJ_OPENING_B_SCENE_H

#include "bn_regular_bg_ptr.h"

#include "mj_cutscene.h"

namespace mj
{

class core;

class opening_b_scene : public cutscene
{

public:
    explicit opening_b_scene(core& core);

    [[nodiscard]] bn::optional<scene_type> update() final;

private:
    bn::regular_bg_ptr _house;
    bn::regular_bg_ptr _oldman;
    bn::regular_bg_ptr _trickortreat;
    bn::regular_bg_ptr _kids;
    
    bn::fixed _scrolly;
};

}

#endif
