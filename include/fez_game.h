#ifndef FEZ_TEST_GAME_H
#define FEZ_TEST_GAME_H

#include "bn_regular_bg_ptr.h"
#include "bn_sprite_position_hbe_ptr.h"

#include "mj/mj_game.h"

namespace fez
{

class game : public mj::game
{

public:
    game(int completed_games, const mj::game_data& data);

    [[nodiscard]] bn::string<16> title() const final
    {
        return "Fall!";
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
	// declare a sprites
	bn::regular_bg_ptr _bg;
	bn::regular_bg_ptr _bg_speed;
	bn::sprite_ptr _moon0;
	bn::sprite_ptr _moon1;
	bn::sprite_ptr _moon2;
	bn::sprite_ptr _moon3;
	bn::sprite_ptr _bat;
	bn::sprite_ptr _umbrella;
	bn::sprite_ptr _stake;
	bn::sprite_ptr _garlic;
	bn::sprite_ptr _cross;
    int _total_frames;
    int _show_result_frames = 60;
    bool _victory = false;
    bool _defeat = false;
};

}

#endif
