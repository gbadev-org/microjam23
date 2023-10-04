#include "exe_poke_game.h"

#include "bn_memory.h"
#include "bn_regular_bg_map_cell_info.h"
#include "bn_size.h"
#include "bn_tile.h"

#include "mj/mj_game_list.h"

// MJ_GAME_LIST_ADD(exe_poke::game)

namespace exe_poke
{

game::game([[maybe_unused]] int completed_games, [[maybe_unused]] const mj::game_data& data) :
    _palette_item(_colors, bn::bpp_mode::BPP_8),
    _palette(bn::bg_palette_ptr::create(_palette_item)),
    _tiles(bn::regular_bg_tiles_ptr::allocate(30 * 20 * 2, bn::bpp_mode::BPP_8)),
    _map(bn::regular_bg_map_ptr::allocate(bn::size(32, 32), _tiles, _palette)),
    _bg(bn::regular_bg_ptr::create((256 - 240) / 2, (256 - 160) / 2, _map)),
    _tiles_span(*_tiles.vram()),
    _map_span(*_map.vram())
{
    // Clear all VRAM tiles:
    bn::memory::set_words(0, _tiles_span.size_bytes() / 4, _tiles_span.data());

    // Point all VRAM tilemap cells to the first VRAM tile:
    int first_tile_index = _tiles.id() * 32;
    bn::regular_bg_map_cell_info cell_info;
    cell_info.set_tile_index(first_tile_index);
    bn::memory::set_half_words(cell_info.cell(), _map_span.size_bytes() / 2, _map_span.data());

    // Set the second 8BPP VRAM tile to color 1:
    bn::memory::set_half_words((1 << 8) + 1, 64 / 2, _tiles_span.data() + 2);

    // Point the second VRAM tilemap cell to the second VRAM tile:
    cell_info.set_tile_index(first_tile_index + 1);
    bn::memory::set_half_words(cell_info.cell(), 1, _map_span.data() + 1);

    // You can't get a palette VRAM span, so you have to write the colors in WRAM and commit them to VRAM:
    _colors.fill(bn::color(0, 0, 31));
    _colors[1] = bn::color(31, 0, 0);
    _palette.set_colors(_palette_item);
}

void game::fade_in([[maybe_unused]] const mj::game_data& data)
{
}

mj::game_result game::play(const mj::game_data& data)
{
    mj::game_result result;
    result.exit = data.pending_frames == 0;

    return result;
}

void game::fade_out([[maybe_unused]] const mj::game_data& data)
{
}

}
