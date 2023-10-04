#ifndef EXE_POKE_GAME_H
#define EXE_POKE_GAME_H

#include "bn_array.h"
#include "bn_color.h"
#include "bn_bg_palette_ptr.h"
#include "bn_bg_palette_item.h"
#include "bn_regular_bg_tiles_ptr.h"
#include "bn_regular_bg_map_ptr.h"
#include "bn_regular_bg_ptr.h"

#include "mj/mj_game.h"

namespace exe_poke
{

class game : public mj::game
{

public:
    game(int completed_games, const mj::game_data& data);

    [[nodiscard]] bn::string<16> title() const final
    {
        return "POKE STUFF!";
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
    bn::array<bn::color, 128> _colors;
    bn::bg_palette_item _palette_item;
    bn::bg_palette_ptr _palette;
    bn::regular_bg_tiles_ptr _tiles;
    bn::regular_bg_map_ptr _map;
    bn::regular_bg_ptr _bg;
    bn::span<bn::tile> _tiles_span;
    bn::span<bn::regular_bg_map_cell> _map_span;
    int _total_frames = 5 * 60;
    bool _victory = false;
};

}

#endif
