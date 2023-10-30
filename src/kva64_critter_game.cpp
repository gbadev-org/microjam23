#include "kva64_critter_game.h"

#include "bn_colors.h"
#include "bn_keypad.h"
#include "bn_music.h"

#include "mj/mj_game_list.h"

#include "bn_music_items.h"

#include "bn_regular_bg_items_kva64_critters_frame.h"
#include "bn_regular_bg_items_kva64_critters.h"

#include "bn_sprite_items_kva64_critters_hints.h"
#include "bn_sprite_items_kva64_critters_fail.h"
#include "bn_sprite_items_kva64_critters_pass.h"

namespace
{
    constexpr bn::string_view code_credits[] = { "kva64" };
    constexpr bn::string_view graphics_credits[] = { "kva64" };
    constexpr bn::string_view music_credits[] = { "kva64" };
    constexpr bn::string_view sfx_credits[] = { "Roger at freesound.org" };
}

// MJ_GAME_LIST_ADD(kva64::critter_carousel_game)
MJ_GAME_LIST_ADD_CODE_CREDITS(code_credits)
MJ_GAME_LIST_ADD_GRAPHICS_CREDITS(graphics_credits)
MJ_GAME_LIST_ADD_MUSIC_CREDITS(music_credits)
MJ_GAME_LIST_ADD_SFX_CREDITS(sfx_credits)

namespace kva64
{
    critter_carousel_game::critter_carousel_game(int completed_games, const mj::game_data& data) :
        current_critter(static_cast<critterType>(data.random.get_int(static_cast<int>(critterType::Devil), static_cast<int>(critterType::Count)))),
        _bg(bn::regular_bg_items::kva64_critters.create_bg(40, -8)),
        _bg_frame(bn::regular_bg_items::kva64_critters_frame.create_bg(8, (256 - 160) / 2)),
        _bg_palette(_bg.palette()),
        _hbe(bn::regular_bg_position_hbe_ptr::create_horizontal(_bg, _hbe_deltas)),
        goal_spr(bn::sprite_items::kva64_critters_hints.create_sprite(-74, -48, static_cast<int>(current_critter))),
        _total_frames(recommended_total_frames(7*60, completed_games, data))
    {
        critter_bg_shifts.insert({critterType::Devil, 56+102+50});
        critter_bg_shifts.insert({critterType::Ghost, 56+52});
        critter_bg_shifts.insert({critterType::Vampire, 4});
        critter_bg_shifts.insert({critterType::Witch, (56+45)*-1});
        critter_bg_shifts.insert({critterType::Zombie, (56+102+46)*-1});

        bn::music::play(bn::music_items::kva64_critter_carousel, 0.75, false);
        bn::music::set_tempo(recommended_music_tempo(completed_games, data));

        _bg.set_x(_bg.x()+critter_bg_shifts[current_critter]);

        _bg_palette.set_grayscale_intensity(0.2);
        _bg_palette.set_fade(bn::colors::red, 0.05);

        carousel_speed = recommended_music_tempo(completed_games, data)*3;

        bn::fixed random_delta_offset{data.random.get_int(0, 512)};
        update_carousel(random_delta_offset);
        _hbe.reload_deltas_ref();

        current_critter_shift = random_delta_offset.integer();
    }

    void critter_carousel_game::fade_in([[maybe_unused]] const mj::game_data& data)
    {
        update_carousel(carousel_speed);
    }

    mj::game_result critter_carousel_game::play(const mj::game_data& data)
    {
        mj::game_result result;
        result.exit = data.pending_frames == 0;

        if(! _victory && ! _defeat)
        {
            update_carousel(carousel_speed);

            if(bn::keypad::a_pressed())
            {
                const int left_bound{512-40};
                const int right_bound{40};

                if (current_critter_shift < left_bound && current_critter_shift > right_bound)
                {
                    result_spr = bn::sprite_items::kva64_critters_fail.create_sprite(-60, 25);
                    result_anim = bn::create_sprite_animate_action_forever(*result_spr, 5, bn::sprite_items::kva64_critters_fail.tiles_item(), 0, 1);

                    result.remove_title = true;
                    _defeat = true;
                }
                else
                {
                    result_spr = bn::sprite_items::kva64_critters_pass.create_sprite(-60, 25);
                    result_anim = bn::create_sprite_animate_action_forever(*result_spr, 5, bn::sprite_items::kva64_critters_pass.tiles_item(), 0, 1);

                    update_carousel(512);

                    result.remove_title = true;
                    _victory = true;
                }
            }
        }
        else
        {
            if(_show_result_frames)
            {
                --_show_result_frames;
            }
            else
            {
                result.exit = true;
            }
            if (result_anim.has_value())
            {
                result_anim->update();
            }
        }

        return result;
    }

    void critter_carousel_game::fade_out([[maybe_unused]] const mj::game_data& data)
    {
        if (result_anim.has_value())
        {
            result_anim->update();
        }
    }

    void critter_carousel_game::update_carousel(const bn::fixed& offset)
    {
        for (int index{bn::display::height()/2-8}; index < bn::display::height(); ++index)
        {
            _hbe_deltas[index] += offset;
            if (_hbe_deltas[index] >= 512)
            {
                _hbe_deltas[index] = 0;
            }
        }
        _hbe.reload_deltas_ref();

        current_critter_shift = _hbe_deltas[140].integer();
    }
}
