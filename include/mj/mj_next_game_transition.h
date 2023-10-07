#ifndef MJ_NEXT_GAME_TRANSITION_H
#define MJ_NEXT_GAME_TRANSITION_H

#include "bn_rect_window_boundaries_hbe_ptr.h"
#include "bn_regular_bg_ptr.h"
#include "bn_sprite_ptr.h"
#include "bn_vector.h"

namespace mj
{

class next_game_transition
{

public:
    explicit next_game_transition(int completed_games);

    [[nodiscard]] bool update();

private:
    bn::pair<bn::fixed, bn::fixed> _window_boundaries[160];
    bn::vector<bn::sprite_ptr, 2> _coffin_sprites;
    bn::vector<bn::sprite_ptr, 3> _digit_sprites;
    bn::optional<bn::regular_bg_ptr> _light_bg;
    int _completed_games;
    int _stage = 0;
    int _counter = 0;
    bn::optional<bn::rect_window_boundaries_hbe_ptr> _light_window_hbe;

    void _fill_window_boundaries();

    void _update_stage_0();

    void _update_stage_1();

    void _update_stage_2();

    void _update_stage_3();
};

}

#endif
