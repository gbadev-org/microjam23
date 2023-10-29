#ifndef KVA64_PEPO_GAME_H
#define KVA64_PEPO_GAME_H

#include "bn_array.h"
#include "bn_optional.h"
#include "bn_point.h"
#include "bn_regular_bg_ptr.h"
#include "bn_sprite_ptr.h"
#include "bn_unordered_map.h"

#include "mj/mj_game.h"

namespace
{
    constexpr int map_size{8};
}

namespace kva64
{
class pepo_surgeon_game : public mj::game
{
public:
    enum pepoType
    {
        Gourd,
        Small,
        Big
    };

    pepo_surgeon_game(int completed_games, const mj::game_data& data);

    [[nodiscard]] bn::string<16> title() const final
    {
        return "Cut!";
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
    bn::optional<bn::regular_bg_ptr> _bg;
    bn::array<bn::optional<bn::sprite_ptr>, 2> pepo_face;
    bn::array<bn::optional<bn::sprite_ptr>, 2> pepo_hat;
    bn::optional<bn::sprite_ptr> knife;

    bn::optional<bn::sprite_ptr> dpad_directions;

    bn::unordered_map<pepoType, int, map_size> pepo_hat_ys;
    bn::unordered_map<pepoType, bn::point, map_size> pepo_knife_pos;
    bn::unordered_map<pepoType, int, map_size> pepo_cut_count;

    pepoType current_pepo;
    bool cutting_direction; // true for up, false for down
    int current_cut_count;

    int _total_frames;
    int _show_result_frames = 100;
    int _show_result_anim_frames = 50;
    bool _victory = false;
    bool _defeat = false;
};
}

#endif // KVA64_PEPO_GAME_H
