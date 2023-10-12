#ifndef MJ_GAME_OVER_SCENE_H
#define MJ_GAME_OVER_SCENE_H

#include "bn_regular_bg_ptr.h"
#include "bn_sprite_affine_mat_ptr.h"
#include "bn_sprite_ptr.h"
#include "bn_vector.h"

#include "mj_scene.h"

namespace mj
{

class core;

class game_over_scene final : public scene
{

public:
    game_over_scene(int completed_games, core& core);

    ~game_over_scene() final;

    [[nodiscard]] bn::optional<scene_type> update() final;

    void update_gfx();

private:
    bn::regular_bg_ptr _backdrop;
    bn::vector<bn::sprite_ptr, 32> _text_sprites;
    bn::fixed _old_blending_intensity;
    int _update_gfx_counter = 0;
    bn::optional<scene_type> _next_scene;
    bn::sprite_affine_mat_ptr _affine_mat;
    bool _fade_in = true;
};

}

#endif
