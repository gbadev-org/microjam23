#ifndef GVSNB_GAME_H
#define GVSNB_GAME_H

#include "bn_list.h"
#include "bn_regular_bg_ptr.h"

#include "mj/mj_game.h"

#include "gvsnb_big_enemy.h"
#include "gvsnb_candy.h"
#include "gvsnb_enemy.h"
#include "gvsnb_hand.h"
#include "gvsnb_sky.h"

namespace gvsnb
{

class game : public mj::game
{

public:
    game(int completed_games, const mj::game_data& data);

    [[nodiscard]] bn::string<16> title() const final
    {
        return "Shinobi!";
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
    static constexpr int _max_enemies = 16;
    static constexpr int _max_candies = 8;

    struct hit_type
    {
        gvsnb::candy* candy;
        gvsnb::enemy* enemy;
        bn::fixed distance;

        [[nodiscard]] bool operator<(const hit_type& other) const
        {
            return other.distance < other.distance;
        }
    };

    sky _sky;
    bn::regular_bg_ptr _building;
    enemy_gfx _enemy_gfx;
    big_enemy _big_enemy;
    candy_gfx _candy_gfx;
    hand _hand;
    bn::list<enemy, _max_enemies> _enemies;
    bn::list<candy, _max_candies> _candies;
    bn::vector<hit_type, _max_enemies * _max_candies> _hits;
    bn::fixed _tempo;
    bn::fixed _new_enemy_frames;
    bn::fixed _defeat_frames;
    bn::fixed _hit_sound_cooldown_frames;
    int _total_frames;
    uint8_t _previous_enemy_difficulty_level[2] = {};
    uint8_t _previous_enemy_door_level[2] = {};
    bool _victory = true;
    bool _previous_enemy_right[2] = {};

    void _add_enemy(bn::random& random);

    void _check_hits();

    void _update(bool playing, bn::random& random);
};

}

#endif
