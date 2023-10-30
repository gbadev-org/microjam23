// SPDX-FileCopyrightText: Copyright 2023 copyrat90 <copyrat90@gmail.com>
// SPDX-License-Identifier: MIT

#pragma once

#include "mj/mj_game.h"

#include "bn_optional.h"
#include "bn_regular_bg_ptr.h"
#include "bn_vector.h"

#include "cr90_ltcd_Candle.h"
#include "cr90_ltcd_EntityActions.h"
#include "cr90_ltcd_LightParticles.h"
#include "cr90_ltcd_Matchstick.h"

namespace cr90::ltcd
{

class Game final : public mj::game
{
public:
    static constexpr int MAX_CANDLES = 8;

public:
    Game(int completed_games, const mj::game_data&);

    auto title() const -> bn::string<16> final;

    int total_frames() const final;
    bool victory() const final;

public:
    auto speed() const -> bn::fixed;

    auto particles() -> LightParticles&;

public:
    void fade_in(const mj::game_data&) final;
    auto play(const mj::game_data&) -> mj::game_result final;
    void fade_out(const mj::game_data&) final;

public:
    void handle_input(const mj::game_data&);
    void update(const mj::game_data&);
    void spread_fire();

public:
    /**
     * @brief Returns the difficulty level overriden with the played status.
     * e.g. If you've never played `EASY` difficulty before, it's force-set to `EASY`.
     */
    static auto forced_difficulty_level(int completed_games, const mj::game_data& data) -> mj::difficulty_level;

    /**
     * @brief Plays the `spooky_birthday.vgm` with the recommended tempo
     * and returns the total frames the game should last.
     *
     * @return Recommended total frames (maximum number of frames that this game should last).
     */
    static int play_spooky_birthday_vgm(int completed_games, const mj::game_data& data);

private:
    const mj::difficulty_level _difficulty;
    const bn::fixed _speed;

    const int _candles_count;
    int _no_fire_candles_count;

    bn::regular_bg_ptr _bg_cake;
    bn::regular_bg_ptr _bg_black;

    Matchstick _matchstick;
    bn::vector<Candle, MAX_CANDLES> _candles;
    bn::optional<EntityMoveLoopAction> _flying_candle_action;
    LightParticles _particles;

    int _total_frames;
};

} // namespace cr90::ltcd
