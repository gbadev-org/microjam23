#ifndef TDC_GAME_H
#define TDC_GAME_H

#include "bn_regular_bg_ptr.h"

#include "mj/mj_game.h"

namespace tdc
{

class tdc_game : public mj::game
{

public:
    tdc_game(int completed_games, const mj::game_data& data);

    [[nodiscard]] bn::string<16> title() const final
    {
        return "Don't Click!";
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
    bn::regular_bg_ptr _bg2;
    bn::regular_bg_ptr _frame;
    bn::fixed _tempo;
    int _total_frames;
    int _show_result_frames;
    bool _victory = true;
};

}

#endif
