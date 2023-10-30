// SPDX-FileCopyrightText: Copyright 2023 copyrat90 <copyrat90@gmail.com>
// SPDX-License-Identifier: MIT

#pragma once

#include "cr90_ltcd_Fireable.h"

#include "bn_optional.h"
#include "bn_sprite_animate_actions.h"
#include "bn_sprite_ptr.h"

namespace cr90::ltcd
{

class Candle final : public Fireable
{
public:
    Candle(const bn::fixed_point& position, bool fire, bool flying, bn::fixed game_speed);

    void update(const mj::game_data&, Game&) override;

public:
    bool flying() const;

    void set_fire(bool) override;

public:
    void set_position(const bn::fixed_point&) override;
    void set_x(bn::fixed) override;
    void set_y(bn::fixed) override;

private:
    const bool _flying;

    bn::sprite_ptr _spr_candle;
    bn::optional<bn::sprite_animate_action<4>> _anim;
};

} // namespace cr90::ltcd
