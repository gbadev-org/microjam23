#ifndef RNTBA_GAME_H
#define RNTBA_GAME_H

#include "rntba_hand.h"
#include "rntba_spider.h"

#include "bn_regular_bg_ptr.h"
#include "bn_sprite_ptr.h"

#include "mj/mj_game.h"

namespace rntba
{

class test_game : public mj::game
{

public:
    test_game(int completed_games, const mj::game_data& data);

    [[nodiscard]] bn::string<16> title() const final
    {
        return "Flick!";
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

    [[nodiscard]] int hand_mode() const
    {
        return _hand.mode();
    }

private:
    bn::regular_bg_ptr _bg;
    rntba::hand _hand;
    int _total_frames;
    rntba::spider _spider;
    bn::sprite_ptr _pumpkin;
    bn::sprite_ptr _frame;
    bn::sprite_ptr _moon;
    int hit_frame = 0;
    int hit_timer = 0;
    int pumpkin_timer = 0;
    int pumpkin_pal = 0;
    int _show_result_frames = 60;
    bool _victory = false;
    bool _defeat = false;
};

}

#endif
