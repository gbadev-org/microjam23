#ifndef MJ_GAME_SCENE_H
#define MJ_GAME_SCENE_H

#include "mj_pause.h"
#include "mj_scene.h"
#include "mj_game_lives.h"
#include "mj_game_backdrop.h"
#include "mj_game_manager.h"
#include "mj_game_timer.h"
#include "mj_game_title.h"
#include "mj_next_game_transition.h"

namespace mj
{

class core;
class game_over_scene;
class game_result_animation;

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
    bn::unique_ptr<game_result_animation> _speed_inc_animation;
    bn::optional<next_game_transition> _next_game_transition;
    bn::unique_ptr<game_over_scene> _game_over_scene;
    bn::fixed _music_tempo;
    bn::fixed _music_volume_dec;
    bn::fixed _dmg_music_left_volume_dec;
    bn::fixed _dmg_music_right_volume_dec;
    bn::fixed _updates;
    bn::optional<scene_type> _next_scene;
    int _completed_games;
    int _pending_frames = 0;
    int _total_frames = 1;
    int _big_pumpkin_stage = 0;
    int _big_pumpkin_counter = 0;
    int _fade_in_frames = 0;
    int _fade_out_frames = 0;
    bool _playing = false;
    bool _victory = false;
    bool _big_pumpkin_inc = true;
    bool _first_game_played = false;

    void _update_play();

    [[nodiscard]] bool _update_fade(bool update_again);

    void _update_big_pumpkin(const bn::regular_bg_item* bg_item);

    void _update_volume_dec();
};

}

#endif
