// SPDX-FileCopyrightText: Copyright 2023 copyrat90 <copyrat90@gmail.com>
// SPDX-License-Identifier: MIT

#include "cr90_ltcd_Fireable.h"

#include "bn_fixed_point.h"

#include "cr90_ltcd_Game.h"

namespace cr90::ltcd
{

Fireable::~Fireable() = default;

Fireable::Fireable(const bn::fixed_point& position, bool fire, int light_median_radius, bn::fixed collider_radius,
                   int particle_emit_interval, const bn::fixed_point& light_diff, const bn::fixed_point& collider_diff)
    : Entity(position), _light_diff(light_diff), _collider_diff(collider_diff),
      _light_median_radius(light_median_radius), _particle_emit_interval(particle_emit_interval),
      _light(position + light_diff, (fire ? light_median_radius : 0)),
      _collider(position + collider_diff, collider_radius), _particle_emit_countdown(particle_emit_interval),
      _fire(fire)
{
    BN_ASSERT(CircleLight::MIN_RADIUS + 1 <= light_median_radius && light_median_radius <= CircleLight::MAX_RADIUS - 1,
              "light_median_radius not in [", CircleLight::MIN_RADIUS + 1, "..", CircleLight::MAX_RADIUS - 1, "]");

    Fireable::set_fire(fire);
}

void Fireable::update(const mj::game_data& data, Game& game)
{
    _light.update(data, game);

    if (_fire)
    {
        if (--_particle_emit_countdown <= 0)
        {
            _particle_emit_countdown = _particle_emit_interval;

            auto particle_position = position();
            particle_position.set_x(x() + data.random.get_int(-3, 3));
            particle_position.set_y(y() - _light_median_radius * 2 / 3);
            game.particles().add_particle(particle_position);
        }
    }
}

bool Fireable::fire() const
{
    return _fire;
}

void Fireable::set_fire(bool fire)
{
    _fire = fire;

    if (fire)
    {
        const uint16_t r = _light_median_radius;
        const bn::array<const uint16_t, 4> radiuses = {r, (uint16_t)(r + 1), r, (uint16_t)(r - 1)};

        _light.set_radius_animation(true, 4, radiuses);
    }
    else
    {
        _light.set_radius(0);
    }
}

auto Fireable::collider() const -> const CircleCollider&
{
    return _collider;
}

void Fireable::set_position(const bn::fixed_point& position)
{
    Entity::set_position(position);

    _light.set_position(position + _light_diff);
    _collider.set_position(position + _collider_diff);
}

void Fireable::set_x(bn::fixed x)
{
    Entity::set_x(x);

    _light.set_x(x + _light_diff.x());
    _collider.set_x(x + _collider_diff.x());
}

void Fireable::set_y(bn::fixed y)
{
    Entity::set_y(y);

    _light.set_y(y + _light_diff.y());
    _collider.set_y(y + _collider_diff.y());
}

} // namespace cr90::ltcd
