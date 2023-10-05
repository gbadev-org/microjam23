#include "mj/mj_game_manager.h"

#include "bn_audio.h"
#include "bn_bg_palettes.h"
#include "bn_bg_tiles.h"
#include "bn_bgs_mosaic.h"
#include "bn_blending.h"
#include "bn_core.h"
#include "bn_dmg_music.h"
#include "bn_green_swap.h"
#include "bn_hdma.h"
#include "bn_link.h"
#include "bn_memory.h"
#include "bn_music.h"
#include "bn_rumble.h"
#include "bn_sound.h"
#include "bn_sprite_palettes.h"
#include "bn_sprites.h"
#include "bn_sprites_mosaic.h"
#include "bn_window.h"

#include "mj/mj_core.h"
#include "mj/mj_game_list.h"

namespace mj
{

game_manager::game_manager(int completed_games, const mj::game_data& data, core& core) :
    _used_alloc_ewram(bn::memory::used_alloc_ewram())
{
    bn::span<game_list::function_type> game_list_entries = game_list::get();
    int game_list_entries_count = game_list_entries.size();
    BN_BASIC_ASSERT(game_list_entries_count, "No game list entries found");

    game_list::function_type game_list_entry =
            game_list_entries[core.random().get_int(game_list_entries_count)];
    _game.reset(game_list_entry(completed_games, data));
}

game_manager::~game_manager()
{
    _game.reset();

    BN_BASIC_ASSERT(bn::memory::used_alloc_ewram() == _used_alloc_ewram,
                    "Memory leak detected: ", bn::memory::used_alloc_ewram(), " - ", _used_alloc_ewram);

    bn::core::set_skip_frames(0);
    bn::core::set_vblank_callback(nullptr);
    bn::assert::set_callback(nullptr);
    bn::link::deactivate();
    bn::rumble::set_enabled(false);

    bn::hdma::stop();
    bn::hdma::high_priority_stop();

    bn::bg_palettes::remove_transparent_color();
    bn::bg_palettes::set_brightness(0);
    bn::bg_palettes::set_contrast(0);
    bn::bg_palettes::set_intensity(0);
    bn::bg_palettes::set_inverted(false);
    bn::bg_palettes::set_grayscale_intensity(0);
    bn::bg_palettes::set_hue_shift_intensity(0);
    bn::bg_palettes::set_fade(bn::color(), 0);

    bn::bg_tiles::set_allow_offset(true);

    bn::sprite_palettes::set_brightness(0);
    bn::sprite_palettes::set_contrast(0);
    bn::sprite_palettes::set_intensity(0);
    bn::sprite_palettes::set_inverted(false);
    bn::sprite_palettes::set_grayscale_intensity(0);
    bn::sprite_palettes::set_hue_shift_intensity(0);
    bn::sprite_palettes::set_fade(bn::color(), 0);

    bn::sprites::set_reserved_handles_count(0);

    bn::blending::restore();
    bn::green_swap::set_enabled(false);

    bn::bgs_mosaic::set_stretch(0);
    bn::sprites_mosaic::set_stretch(0);

    bn::window::internal().restore();
    bn::window::external().restore();
    bn::window::sprites().restore();
    bn::window::outside().restore();

    if(bn::music::playing())
    {
        bn::music::stop();
    }

    if(bn::dmg_music::playing())
    {
        bn::dmg_music::stop();
    }

    bn::sound::stop_all();
    bn::sound::set_master_volume(1);
    bn::audio::set_update_on_vblank(false);
    bn::audio::set_dmg_sync_enabled(false);
}

}
