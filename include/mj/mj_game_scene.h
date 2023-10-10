#ifndef MJ_GAME_SCENE_H
#define MJ_GAME_SCENE_H

#include "mj_pause.h"
#include "mj_scene.h"
#include "mj_game_lives.h"
#include "mj_game_backdrop.h"
#include "mj_game_manager.h"
#include "mj_game_result_animation.h"
#include "mj_game_timer.h"
#include "mj_game_title.h"
#include "mj_next_game_transition.h"

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
    game_data _data;
    pause _pause;
    game_backdrop _backdrop;
    game_title _title;
    game_timer _timer;
    game_lives _lives;
    bn::optional<game_manager> _game_manager;
    bn::optional<bn::regular_bg_ptr> _big_pumpkin;
    bn::unique_ptr<game_result_animation> _result_animation;
    bn::optional<next_game_transition> _next_game_transition;
    bn::fixed _music_volume_dec;
    bn::fixed _dmg_music_left_volume_dec;
    bn::fixed _dmg_music_right_volume_dec;
    int _completed_games = 0;
    int _pending_frames = 0;
    int _total_frames = 1;
    int _big_pumpkin_stage = 0;
    int _big_pumpkin_counter = 0;
    int _exit_frames = 0;
    bool _playing = false;
    bool _victory = false;
    bool _big_pumpkin_inc = true;

    void _update_play();

    [[nodiscard]] bool _update_fade();

    void _update_volume_dec();
};

}

#endif
