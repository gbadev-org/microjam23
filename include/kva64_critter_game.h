#ifndef KVA64_CRITTER_GAME_H
#define KVA64_CRITTER_GAME_H

#include "bn_bg_palette_ptr.h"
#include "bn_display.h"
#include "bn_fixed.h"
#include "bn_optional.h"
#include "bn_regular_bg_ptr.h"
#include "bn_regular_bg_position_hbe_ptr.h"
#include "bn_sprite_ptr.h"
#include "bn_sprite_animate_actions.h"
#include "bn_unordered_map.h"

#include "mj/mj_game.h"

namespace
{
    constexpr int map_size{8};
}

namespace kva64
{

class critter_carousel_game : public mj::game
{

public:
    enum critterType
    {
        Devil,
        Ghost,
        Vampire,
        Witch,
        Zombie,
        Count
    };

    critter_carousel_game(int completed_games, const mj::game_data& data);

    [[nodiscard]] bn::string<16> title() const final
    {
        switch (current_critter)
        {
        case critterType::Devil:
            return "Devil!";
        case critterType::Ghost:
            return "Ghost!";
        case critterType::Vampire:
            return "Vampire!";
        case critterType::Witch:
            return "Witch!";
        case critterType::Zombie:
            return "Zombie!";
        case critterType::Count:
        default:
            return "Match!";
        }
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
    void update_carousel(const bn::fixed& offset);

    critterType current_critter;
    bn::fixed carousel_speed;

    bn::array<bn::fixed, bn::display::height()> _hbe_deltas;
    bn::regular_bg_ptr _bg;
    bn::regular_bg_ptr _bg_frame;
    bn::bg_palette_ptr _bg_palette;
    bn::regular_bg_position_hbe_ptr _hbe;

    bn::optional<bn::sprite_animate_action<2>> result_anim;

    bn::sprite_ptr goal_spr;
    bn::optional<bn::sprite_ptr> result_spr;

    bn::unordered_map<critterType, int, map_size> critter_bg_shifts;
    bn::unordered_map<critterType, bn::fixed, map_size> critter_centers;

    int current_critter_shift;

    int _total_frames;
    int _show_result_frames = 60;
    bool _victory = false;
    bool _defeat = false;
};
}

#endif // KVA64_CRITTER_GAME_H
