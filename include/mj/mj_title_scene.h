#ifndef MJ_TITLE_SCENE_H
#define MJ_TITLE_SCENE_H

#include "bn_affine_mat_attributes.h"
#include "bn_bg_palettes_actions.h"
#include "bn_music_actions.h"
#include "bn_sprite_affine_mat_ptr.h"
#include "bn_sprite_affine_mat_attributes_hbe_ptr.h"
#include "bn_sprite_ptr.h"
#include "bn_sprite_palettes_actions.h"
#include "bn_unique_ptr.h"
#include "bn_vector.h"

#include "mj_scene.h"

namespace mj
{

class core;
class title_backdrop;

class title_scene final : public scene
{

public:
    explicit title_scene(core& core);

    ~title_scene() final;

    [[nodiscard]] bn::optional<scene_type> update() final;

private:
    bn::affine_mat_attributes _affine_mat_attributes[160];
    bn::unique_ptr<title_backdrop> _backdrop;
    bn::optional<bn::bg_palettes_fade_to_action> _bgs_fade_action;
    bn::optional<bn::sprite_palettes_fade_to_action> _sprites_fade_action;
    bn::optional<bn::music_volume_to_action> _music_volume_action;
    bn::vector<bn::sprite_ptr, 16> _title_sprites_1;
    bn::vector<bn::sprite_ptr, 16> _title_sprites_2;
    bn::vector<bn::sprite_ptr, 8> _high_score_sprites;
    bn::vector<bn::sprite_ptr, 4> _play_sprites;
    bn::vector<bn::sprite_ptr, 4> _credits_sprites;
    bn::sprite_ptr _cursor_sprite;
    bn::optional<scene_type> _next_scene;
    bn::sprite_affine_mat_ptr _affine_mat;
    bn::sprite_affine_mat_attributes_hbe_ptr _affine_mat_hbe;

    void _update_bgs();

    void _create_title_sprites();

    void _update_title_sprites(bool update_first);

    void _set_menu_visible(bool visible);

    void _update_menu();
};

}

#endif
