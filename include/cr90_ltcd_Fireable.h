// SPDX-FileCopyrightText: Copyright 2023 copyrat90 <copyrat90@gmail.com>
// SPDX-License-Identifier: MIT

#pragma once

#include "cr90_ltcd_Entity.h"

#include "cr90_ltcd_CircleCollider.h"
#include "cr90_ltcd_CircleLight.h"

namespace cr90::ltcd
{

class Fireable : public Entity
{
public:
    virtual ~Fireable() = 0;
    Fireable(const bn::fixed_point& position, bool fire, int light_median_radius, bn::fixed collider_radius,
             int particle_emit_interval, const bn::fixed_point& light_diff = {},
             const bn::fixed_point& collider_diff = {});

    void update(const mj::game_data&, Game&) override;

public:
    bool fire() const;
    virtual void set_fire(bool);

    auto collider() const -> const CircleCollider&;

public:
    void set_position(const bn::fixed_point&) override;
    void set_x(bn::fixed) override;
    void set_y(bn::fixed) override;

private:
    const bn::fixed_point _light_diff;
    const bn::fixed_point _collider_diff;

    const int _light_median_radius;
    const int _particle_emit_interval;

    CircleLight _light;
    CircleCollider _collider;

    int _particle_emit_countdown;
    bool _fire;
};

} // namespace cr90::ltcd
