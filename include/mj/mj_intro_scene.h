#ifndef MJ_INTRO_SCENE_H
#define MJ_INTRO_SCENE_H

#include "bn_bg_palettes_actions.h"
#include "bn_regular_bg_ptr.h"
#include "bn_sprite_palettes_actions.h"
#include "bn_sprite_ptr.h"
#include "bn_vector.h"

#include "mj_scene.h"

namespace mj
{

class core;

class intro_scene : public scene
{

public:
    explicit intro_scene(core& core);

    [[nodiscard]] bn::optional<scene_type> update() final;

private:
    bn::bg_palettes_fade_to_action _bg_fade_action;
    bn::sprite_palettes_fade_to_action _sprite_fade_action;
    bn::regular_bg_ptr _backdrop;
    bn::regular_bg_ptr _butano;
    bn::vector<bn::sprite_ptr, 20> _text_sprites;
    int _counter = 60 * 4;
    bool _update_butano = false;
};

}

#endif
