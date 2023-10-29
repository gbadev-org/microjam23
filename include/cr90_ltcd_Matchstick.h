// SPDX-FileCopyrightText: Copyright 2023 copyrat90 <copyrat90@gmail.com>
// SPDX-License-Identifier: MIT

#pragma once

#include "cr90_ltcd_Fireable.h"

#include "bn_optional.h"
#include "bn_sprite_animate_actions.h"
#include "bn_sprite_ptr.h"

namespace cr90::ltcd
{

class Matchstick final : public Fireable
{
public:
    Matchstick(const bn::fixed_point& position, bool fire);

    void handle_input(const mj::game_data&, Game&) override;
    void update(const mj::game_data&, Game&) override;

public:
    void set_fire(bool) override;

public:
    void set_position(const bn::fixed_point&) override;
    void set_x(bn::fixed) override;
    void set_y(bn::fixed) override;

private:
    bn::sprite_ptr _spr_stick;
    bn::optional<bn::sprite_animate_action<4>> _anim;
};

} // namespace cr90::ltcd
