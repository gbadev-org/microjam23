#ifndef FIXXG1_GAME_H
#define FIXXG1_GAME_H

#include "bn_regular_bg_ptr.h"
#include "bn_sprite_ptr.h"
#include "bn_fixed_point.h"

#include "mj/mj_game.h"

namespace fixxg1
{

class game : public mj::game
{

public:
    game(int completed_games, const mj::game_data& data);

    [[nodiscard]] bn::string<16> title() const final
    {
        return "Pumpkin Baseball";
    }

    [[nodiscard]] int total_frames() const final
    {
        return _total_frames;
    }

    void fade_in(const mj::game_data& data) final;

    [[nodiscard]] mj::game_result play(const mj::game_data& data) final;

    [[nodiscard]] bool victory() const final
    {
        return _victory;
    }

    void fade_out(const mj::game_data& data) final;

private:
    bn::regular_bg_ptr _bg;
    bn::sprite_ptr _spr_club;
    bn::sprite_ptr _spr_ball;
    bn::fixed _club_rotation = 320;
    bn::fixed_point _club_position =bn::fixed_point(0,30);
    bn::fixed_point _club_rotation_offset=bn::fixed_point(0,25);
    int _swing_frames=20;
    int _ball_fall_delay=30;
    bn::fixed_point _ball_velocity=bn::fixed_point(0,0);
    bn::fixed _ball_gravity=0.04;
    int _total_frames;
    int _show_result_frames = 60;
    bool _swinging=false;
    bool _ball_falling=false;
    bool _victory = false;
    bool _defeat = false;
};

}

#endif
