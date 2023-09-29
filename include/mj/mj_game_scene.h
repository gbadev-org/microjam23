#ifndef MJ_GAME_SCENE_H
#define MJ_GAME_SCENE_H

#include "bn_regular_bg_ptr.h"
#include "bn_unique_ptr.h"

#include "mj_game.h"
#include "mj_pause.h"
#include "mj_scene.h"
#include "mj_game_timer.h"
#include "mj_game_title.h"

namespace mj
{

class core;

class game_scene final : public scene
{

public:
    explicit game_scene(core& core);

    ~game_scene() final;

    [[nodiscard]] bn::optional<scene_type> update() final;

private:
    core& _core;
    bn::unique_ptr<game> _game;
    game_data _data;
    pause _pause;
    game_title _title;
    game_timer _timer;
    bn::optional<bn::regular_bg_ptr> _big_pumpkin;
    bn::vector<bn::sprite_ptr, 16> _info_sprites;
    int _lives = 4;
    int _completed_games = 0;
    int _pending_frames = 0;
    int _big_pumpkin_stage = 0;
    int _big_pumpkin_counter = 0;
    int _exit_frames = 0;
    bool _playing = false;
    bool _big_pumpkin_inc = true;

    void _print_info();

    void _update_play();

    [[nodiscard]] bool _update_fade();
};

}

#endif
