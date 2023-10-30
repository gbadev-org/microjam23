#ifndef AHUL_GAME_H
#define AHUL_GAME_H

#include "bn_point.h"
#include "bn_regular_bg_ptr.h"
#include "bn_sprite_ptr.h"
#include "bn_sprite_animate_actions.h"

#include "mj/mj_game.h"

namespace ahul
{

class game : public mj::game
{

public:
    game(int completed_games, const mj::game_data& data);

    [[nodiscard]] bn::string<16> title() const final
    {
        return "Lights On!";
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
    bn::sprite_ptr _character_sprite;
    bn::point _character_map_position;
    bn::sprite_animate_action<4> _action;
    int _total_frames;
    int _show_result_frames;
    int _frames_to_dark = 10;
    bn::fixed _walk_tempo;
    bool _victory = false;
    bool _defeat = false;
    bool _is_dark = false;
};

}

#endif
