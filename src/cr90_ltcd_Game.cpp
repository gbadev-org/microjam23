// SPDX-FileCopyrightText: Copyright 2023 copyrat90 <copyrat90@gmail.com>
// SPDX-License-Identifier: MIT

#include "cr90_ltcd_Game.h"

#include "bn_bitset.h"
#include "bn_blending.h"
#include "bn_display.h"
#include "bn_fixed_point.h"
#include "bn_fixed_size.h"
#include "bn_format.h"
#include "bn_sound.h"
#include "bn_window.h"

#include "mj/mj_game_list.h"

#include "bn_regular_bg_items_cr90_ltcd_black.h"
#include "bn_regular_bg_items_cr90_ltcd_cake.h"
#include "bn_sound_items.h"

#include "bn_dmg_music_items_cr90_ltcd_spooky_birthday_t138.h"
#include "bn_dmg_music_items_cr90_ltcd_spooky_birthday_t143.h"
#include "bn_dmg_music_items_cr90_ltcd_spooky_birthday_t149.h"
#include "bn_dmg_music_items_cr90_ltcd_spooky_birthday_t154.h"
#include "bn_dmg_music_items_cr90_ltcd_spooky_birthday_t160.h"
#include "bn_dmg_music_items_cr90_ltcd_spooky_birthday_t165.h"
#include "bn_dmg_music_items_cr90_ltcd_spooky_birthday_t171.h"
#include "bn_dmg_music_items_cr90_ltcd_spooky_birthday_t176.h"
#include "bn_dmg_music_items_cr90_ltcd_spooky_birthday_t182.h"
#include "bn_dmg_music_items_cr90_ltcd_spooky_birthday_t187.h"
#include "bn_dmg_music_items_cr90_ltcd_spooky_birthday_t193.h"
#include "bn_dmg_music_items_cr90_ltcd_spooky_birthday_t198.h"
#include "bn_dmg_music_items_cr90_ltcd_spooky_birthday_t204.h"
#include "bn_dmg_music_items_cr90_ltcd_spooky_birthday_t209.h"
#include "bn_dmg_music_items_cr90_ltcd_spooky_birthday_t215.h"
#include "bn_dmg_music_items_cr90_ltcd_spooky_birthday_t220.h"

namespace
{
constexpr bn::string_view code_credits[] = {"copyrat90"};
constexpr bn::string_view graphics_credits[] = {"copyrat90"};
constexpr bn::string_view music_credits[] = {"copyrat90"};
constexpr bn::string_view sfx_credits[] = {"copyrat90", "kingsrow"};
} // namespace

// MJ_GAME_LIST_ADD(cr90::ltcd::Game)
MJ_GAME_LIST_ADD_CODE_CREDITS(code_credits)
MJ_GAME_LIST_ADD_GRAPHICS_CREDITS(graphics_credits)
MJ_GAME_LIST_ADD_MUSIC_CREDITS(music_credits)
MJ_GAME_LIST_ADD_SFX_CREDITS(sfx_credits)

namespace cr90::ltcd
{

namespace
{

constexpr bn::fixed CANDLE_BOTTOM_DIFF = 46;

constexpr int FLYING_CHANGE_DIRECTION_INTERVAL = 45;
constexpr bn::size CANDLE_GRID_SIZE = {24, 5};

constexpr bn::fixed_point CANDLE_GRID_TOP_LEFT = {
    40 - bn::display::width() / 2,
    70 - bn::display::height() / 2,
};

constexpr bn::fixed_point CANDLE_GRID_BOTTOM_RIGHT = {
    200 - bn::display::width() / 2,
    130 - bn::display::height() / 2,
};

constexpr bn::fixed_point CANDLE_GRID_CENTER = (CANDLE_GRID_TOP_LEFT + CANDLE_GRID_BOTTOM_RIGHT) / 2;

constexpr bn::fixed_size CANDLE_GRID_DIMENSIONS = {
    CANDLE_GRID_BOTTOM_RIGHT.x() - CANDLE_GRID_TOP_LEFT.x(),
    CANDLE_GRID_BOTTOM_RIGHT.y() - CANDLE_GRID_TOP_LEFT.y(),
};

constexpr bn::fixed_size CANDLE_GRID_CELL_SIZE = {
    CANDLE_GRID_DIMENSIONS.width() / CANDLE_GRID_SIZE.width(),
    CANDLE_GRID_DIMENSIONS.height() / CANDLE_GRID_SIZE.height(),
};

struct PlayedDifficulties
{
    bool easy : 1 = false;
    bool normal : 1 = false;
};

PlayedDifficulties g_played_difficulties;

} // namespace

static auto init_speed(bn::fixed speed) -> bn::fixed
{
    return 1 + (speed - 1) / 4;
}

static int init_candles_count(mj::difficulty_level difficulty)
{
    constexpr bn::array<int, 3> CANDLES_COUNTS = {6, 7, 8};

    static_assert(
        [CANDLES_COUNTS]() -> bool {
            for (int candles_count : CANDLES_COUNTS)
                if (candles_count > Game::MAX_CANDLES || candles_count < 2)
                    return false;
            return true;
        }(),
        "`candles_count` must be in [2..`MAX_CANDLES`]");

    return CANDLES_COUNTS[int(difficulty)];
}

static bool init_matchstick_fire(mj::difficulty_level difficulty, bn::random& random)
{
    if (difficulty == mj::difficulty_level::EASY)
        return true;
    if (difficulty == mj::difficulty_level::NORMAL)
        return random.get_fixed(0, 1) <= 0.65;
    // difficulty: HARD
    return random.get_fixed(0, 1) <= 0.35;
}

Game::Game(int completed_games, const mj::game_data& data)
    : _difficulty(forced_difficulty_level(completed_games, data)),
      _speed(init_speed(recommended_music_tempo(completed_games, data))),
      _candles_count(init_candles_count(_difficulty)), _no_fire_candles_count(_candles_count),
      _bg_cake(bn::regular_bg_items::cr90_ltcd_cake.create_bg((256 - 240) / 2, (256 - 160) / 2)),
      _bg_black(bn::regular_bg_items::cr90_ltcd_black.create_bg((256 - 240) / 2, (256 - 160) / 2)),
      _matchstick(bn::fixed_point(0, 40), init_matchstick_fire(_difficulty, data.random)),
      _total_frames(play_spooky_birthday_vgm(completed_games, data))
{
    // init shadow effect
    _bg_black.set_priority(2);
    _bg_black.set_blending_enabled(true);
    bn::blending::set_transparency_alpha(0.5);
    bn::window::sprites().set_show_bg(_bg_black, false);

    // init candles
    const int flying_candle_index =
        ((_difficulty == mj::difficulty_level::HARD) ? data.random.get_int(_candles_count) : -1);
    bn::bitset<CANDLE_GRID_SIZE.width()> column_used;

    for (int i = 0; i < _candles_count; ++i)
    {
        // select unused `column` in 24 X 5 candle grid
        const int available_columns = column_used.size() - column_used.count();
        const int column_order = data.random.get_int(available_columns);

        int column = -1;
        for (int j = 0, order = 0; j < column_used.size(); ++j)
        {
            if (!column_used[j] && order++ == column_order)
            {
                column = j;

                // mark adjacent columns as used
                column_used[j] = true;
                if (j - 1 >= 0)
                    column_used[j - 1] = true;
                if (j + 1 < column_used.size())
                    column_used[j + 1] = true;

                break;
            }
        }
        BN_ASSERT(column >= 0, "logical error in random column selection");

        // select any `row` in 24 X 5 candle grid
        const int row = data.random.get_int(CANDLE_GRID_SIZE.height());

        // get candle position within (column, row) cell
        auto x = data.random.get_fixed(CANDLE_GRID_TOP_LEFT.x() + column * CANDLE_GRID_CELL_SIZE.width(),
                                       CANDLE_GRID_TOP_LEFT.x() + (column + 1) * CANDLE_GRID_CELL_SIZE.width());
        auto y = data.random.get_fixed(CANDLE_GRID_TOP_LEFT.y() + row * CANDLE_GRID_CELL_SIZE.height(),
                                       CANDLE_GRID_TOP_LEFT.y() + (row + 1) * CANDLE_GRID_CELL_SIZE.height());

        y -= CANDLE_BOTTOM_DIFF;

        const bool candle_fire = (i == _candles_count - 1 && !_matchstick.fire());
        const bool candle_flying = (i == flying_candle_index);

        _candles.emplace_back(bn::fixed_point{x, y}, candle_fire, candle_flying, _speed);

        if (candle_flying)
        {
            static constexpr bn::fixed TOP_Y =
                CANDLE_GRID_TOP_LEFT.y() + CANDLE_GRID_CELL_SIZE.height() / 2 - CANDLE_BOTTOM_DIFF;
            static constexpr bn::fixed BOTTOM_Y =
                CANDLE_GRID_BOTTOM_RIGHT.y() - CANDLE_GRID_CELL_SIZE.height() / 2 - CANDLE_BOTTOM_DIFF;

            bn::fixed_point final_position(x, 0);

            if (y > CANDLE_GRID_CENTER.y())
            {
                // flying: bottom -> top
                _candles.back().set_y(BOTTOM_Y);
                final_position.set_y(TOP_Y);
            }
            else
            {
                // flying: top -> bottom
                _candles.back().set_y(TOP_Y);
                final_position.set_y(BOTTOM_Y);
            }

            _flying_candle_action = EntityMoveLoopAction(
                _candles.back(), (FLYING_CHANGE_DIRECTION_INTERVAL / _speed).round_integer(), final_position);
        }

        _no_fire_candles_count -= candle_fire;
    }
}

auto Game::title() const -> bn::string<16>
{
    return bn::format<16>("Light {} candle{}!", _no_fire_candles_count, (_no_fire_candles_count > 1 ? "s" : ""));
}

int Game::total_frames() const
{
    return _total_frames;
}

bool Game::victory() const
{
    return _no_fire_candles_count <= 0;
}

auto Game::speed() const -> bn::fixed
{
    return _speed;
}

auto Game::particles() -> LightParticles&
{
    return _particles;
}

void Game::fade_in(const mj::game_data& data)
{
    update(data);
}

auto Game::play(const mj::game_data& data) -> mj::game_result
{
    handle_input(data);
    update(data);
    spread_fire();

    return mj::game_result{.exit = false, .remove_title = victory()};
}

void Game::fade_out(const mj::game_data& data)
{
    update(data);
}

void Game::handle_input(const mj::game_data& data)
{
    _matchstick.handle_input(data, *this);
}

void Game::update(const mj::game_data& data)
{
    _matchstick.update(data, *this);

    if (_flying_candle_action)
        _flying_candle_action->update();

    for (auto& candle : _candles)
        candle.update(data, *this);

    _particles.update(data, *this);
}

void Game::spread_fire()
{
    const bool prev_victory = victory();

    for (auto& candle : _candles)
    {
        auto spread_fire_to = [](Fireable& fireable) {
            bn::sound::stop_all();
            bn::sound_items::cr90_ltcd_set_fire.play();
            fireable.set_fire(true);
        };

        if (_matchstick.fire() && !candle.fire())
        {
            if (_matchstick.collider().collide_with(candle.collider()))
            {
                spread_fire_to(candle);
                --_no_fire_candles_count;
            }
        }
        else if (candle.fire() && !_matchstick.fire())
        {
            if (_matchstick.collider().collide_with(candle.collider()))
            {
                spread_fire_to(_matchstick);
            }
        }
    }

    if (victory() && !prev_victory)
        bn::sound_items::cr90_ltcd_correct.play();
}

auto Game::forced_difficulty_level(int completed_games, const mj::game_data& data) -> mj::difficulty_level
{
    auto difficulty = recommended_difficulty_level(completed_games, data);

    if (!g_played_difficulties.easy)
    {
        g_played_difficulties.easy = true;
        difficulty = mj::difficulty_level::EASY;
    }
    else if (difficulty >= mj::difficulty_level::NORMAL && !g_played_difficulties.normal)
    {
        g_played_difficulties.normal = true;
        difficulty = mj::difficulty_level::NORMAL;
    }

    return difficulty;
}

int Game::play_spooky_birthday_vgm(int completed_games, const mj::game_data& data)
{
    const int tempo = (recommended_music_tempo(completed_games, data) * 120).round_integer();

    switch (tempo)
    {
    case 138:
        bn::dmg_music_items::cr90_ltcd_spooky_birthday_t138.play(1, false);
        break;
    case 143:
        bn::dmg_music_items::cr90_ltcd_spooky_birthday_t143.play(1, false);
        break;
    case 149:
        bn::dmg_music_items::cr90_ltcd_spooky_birthday_t149.play(1, false);
        break;
    case 154:
        bn::dmg_music_items::cr90_ltcd_spooky_birthday_t154.play(1, false);
        break;
    case 160:
        bn::dmg_music_items::cr90_ltcd_spooky_birthday_t160.play(1, false);
        break;
    case 165:
        bn::dmg_music_items::cr90_ltcd_spooky_birthday_t165.play(1, false);
        break;
    case 171:
        bn::dmg_music_items::cr90_ltcd_spooky_birthday_t171.play(1, false);
        break;
    case 176:
        bn::dmg_music_items::cr90_ltcd_spooky_birthday_t176.play(1, false);
        break;
    case 182:
        bn::dmg_music_items::cr90_ltcd_spooky_birthday_t182.play(1, false);
        break;
    case 187:
        bn::dmg_music_items::cr90_ltcd_spooky_birthday_t187.play(1, false);
        break;
    case 193:
        bn::dmg_music_items::cr90_ltcd_spooky_birthday_t193.play(1, false);
        break;
    case 198:
        bn::dmg_music_items::cr90_ltcd_spooky_birthday_t198.play(1, false);
        break;
    case 204:
        bn::dmg_music_items::cr90_ltcd_spooky_birthday_t204.play(1, false);
        break;
    case 209:
        bn::dmg_music_items::cr90_ltcd_spooky_birthday_t209.play(1, false);
        break;
    case 215:
        bn::dmg_music_items::cr90_ltcd_spooky_birthday_t215.play(1, false);
        break;
    case 220:
        bn::dmg_music_items::cr90_ltcd_spooky_birthday_t220.play(1, false);
        break;

    default:
        BN_ERROR("Unsupported tempo=", tempo, " for `spooky_birthday.vgm`");
    }

    const int base_total_frames = 13 * 60 / 2 - 24 - 24;
    return recommended_total_frames(base_total_frames, completed_games, data);
}

} // namespace cr90::ltcd
