#ifndef MJ_GAME_SCENE_H
#define MJ_GAME_SCENE_H

#include "bn_regular_bg_ptr.h"
#include "bn_sprite_ptr.h"
#include "bn_unique_ptr.h"
#include "bn_vector.h"

#include "mj_game.h"
#include "mj_scene.h"

namespace mj
{

class core;

class game_scene : public scene
{

public:
    explicit game_scene(core& core);

    ~game_scene();

    [[nodiscard]] bn::optional<scene_type> update() final;

private:
    core& _core;
    bn::unique_ptr<game> _game;
    game_data _data;
    bn::optional<bn::regular_bg_ptr> _big_pumpkin;
    bn::vector<bn::sprite_ptr, 16> _pause_sprites;
    bn::vector<bn::sprite_ptr, 8> _timer_sprites;
    bn::vector<bn::sprite_ptr, 16> _info_sprites;
    bn::vector<bn::sprite_ptr, 24> _title_sprites;
    bn::vector<bn::fixed, 16> _title_sprites_x;
    int _lives = 4;
    int _completed_games = 0;
    int _pending_frames = 0;
    int _big_pumpkin_stage = 0;
    int _big_pumpkin_counter = 0;
    int _title_counter = 0;
    int _exit_frames = 0;
    bool _playing = false;
    bool _big_pumpkin_inc = true;
    bool _exit_from_pause = false;

    void _print_info();

    void _print_title();

    [[nodiscard]] bool _update_pause();

    void _update_title();

    void _update_timer();

    void _update_play();

    [[nodiscard]] bool _update_fade();
};

}

#endif
