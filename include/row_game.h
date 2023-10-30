#ifndef ROW_GAME_H
#define ROW_GAME_H

#include "bn_regular_bg_ptr.h"
#include "bn_sprite_ptr.h"

#include "mj/mj_game.h"

namespace row
{

class game : public mj::game
{

public:
    game(int completed_games, const mj::game_data& data);

    [[nodiscard]] bn::string<16> title() const final
    {
        return "ROW ROW ROW!";
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
    bn::regular_bg_ptr _rower;
    bn::regular_bg_ptr _water;
    bn::regular_bg_ptr _fishy;

    bn::sprite_ptr _l_button;
    bn::sprite_ptr _r_button;

    int _total_frames;
    unsigned _current_row_frame = 0;
    int _show_result_frames = 60;

    bn::fixed _position = 0;
    bn::fixed _velocity = 0;
    bn::fixed _fish_position = 0;
    bn::fixed _fish_velocity = 0;
    unsigned _eating_fishy_frame = 0;

    bn::fixed compute_fish_velocity(int frames);

    enum rower_state {
        wound,
        stroked,
        winding,
        stroking,
    };

    enum rower_state _current_state;

    bool _victory = false;
    bool _defeat = false;

    bool handle_input();
};

}

#endif
