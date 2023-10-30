#ifndef FIXXG3_GAME_H
#define FIXXG3_GAME_H

#include "bn_regular_bg_ptr.h"
#include "bn_sprite_ptr.h"
#include "bn_fixed_point.h"
#include "bn_sprite_animate_actions.h"

#include "mj/mj_game.h"

namespace fixxg3
{

enum class club_state{
    UP,
    SWING_DOWN,
    SWING_UP
};

enum class zombie_state{
    HIDING,
    MOVING_UP,
    UP,
    MOVING_DOWN,
    CLUBBING_DOWN
};

class game : public mj::game
{

public:
    game(int completed_games, const mj::game_data& data);

    [[nodiscard]] bn::string<16> title() const final
    {
        return "Club The Zombie";
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
    bn::fixed _club_rotation = 45;
    const bn::fixed_point _club_rotation_offset=bn::fixed_point(0,25);
    const bn::fixed _club_up_rotation = 45;
    const bn::fixed _club_down_rotation = 45+90;
    const bn::fixed _club_swing_speed = 14;
    const bn::fixed _club_min_y=-40;
    const bn::fixed _club_max_y=50;
    const bn::fixed _club_min_x=-50;
    const bn::fixed _club_max_x=110;
    const bn::fixed _club_move_speed=2;
    const bn::fixed_point _club_hit_offset=bn::fixed_point(-40,25);
    const bn::fixed _club_hit_tolerance = 20;
    bn::fixed_point _club_position =bn::fixed_point(0,30);
    club_state _club_swingstate=club_state::UP;

    bn::sprite_ptr _zombie_head;
    bn::sprite_animate_action<6> _zombie_head_action;
    zombie_state _zombie_state= zombie_state::HIDING;
    const bn::fixed_point _possible_zombie_locations[9] = {
        bn::fixed_point(0,119-(160/2)), // botton center
        bn::fixed_point(179-(240/2),119-(160/2)), // bottom right
        bn::fixed_point(63-(240/2),119-(160/2)), // bottom left
        bn::fixed_point(0,95-(160/2)), // center center
        bn::fixed_point(170-(240/2),95-(160/2)), // center right
        bn::fixed_point(70-(240/2),95-(160/2)), // center left
        bn::fixed_point(0,76-(160/2)), // top center
        bn::fixed_point(164-(240/2),76-(160/2)), // top right
        bn::fixed_point(77-(240/2),76-(160/2)) // top left
    };
    const bn::fixed _possible_zombie_scales[9] = {
        1,1,1, // bottom
        0.8,0.8,0.8, // center
        0.7,0.7,0.7 // top
    };
    const int _max_hide_frames = 10;
    int _hide_frames=0;
    int _max_up_frames = 50;
    int _up_frames = 0;
    int _zombie_pos_index = 0;
    const int _max_zombie_down_frames=10;
    int _zombie_down_frames=0;

    int _total_frames;
    int _show_result_frames = 60;
    bool _victory = false;

    bool does_club_hit_pos(bn::fixed_point pos);
    void zombie_appear(int posIndex);
};

}

#endif
