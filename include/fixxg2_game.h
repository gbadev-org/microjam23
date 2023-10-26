#ifndef FIXXG2_GAME_H
#define FIXXG2_GAME_H

#include "bn_regular_bg_ptr.h"
#include "bn_affine_bg_ptr.h"
#include "bn_sprite_ptr.h"

#include "mj/mj_game.h"

namespace fixxg2
{

class game : public mj::game
{

public:
    game(int completed_games, const mj::game_data& data);

    [[nodiscard]] bn::string<16> title() const final
    {
        return "Shoot zombie!";
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
    bn::affine_bg_ptr _zombie;
    bn::sprite_ptr _crosshair;
    bn::fixed _crosshair_speed_multiplier=1;
    int _elapsed_frames = 0;
    int _total_frames;
    int _show_result_frames = 60;
    bool _victory = false;
    bool _defeat = false;

    bool does_crosshair_hit_zombie();
};

}

#endif
