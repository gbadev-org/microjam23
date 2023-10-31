#ifndef MJ_OPENING_A_SCENE_H
#define MJ_OPENING_A_SCENE_H

#include "bn_regular_bg_ptr.h"
#include "bn_sprite_ptr.h"
#include "bn_vector.h"

#include "mj_cutscene.h"

namespace mj
{

class core;

class opening_a_scene : public cutscene
{

    // enum class scene_a_state : uint8_t
    // {
    //     FADE_IN,
    //     FADE_IN,
    //     FADE_OUT,
    // };

public:
    explicit opening_a_scene(core& core);

    [[nodiscard]] bn::optional<scene_type> update() final;

private:
    bn::regular_bg_ptr _chair;
    bn::regular_bg_ptr _oldman;
    bn::regular_bg_ptr _dingdong;
    bn::sprite_ptr _exclaim;
    bn::sprite_ptr _pipe;
    bn::vector<bn::sprite_ptr, 4> _skip_text_sprites;
};

}

#endif
