// SPDX-FileCopyrightText: Copyright 2023 copyrat90 <copyrat90@gmail.com>
// SPDX-License-Identifier: MIT

#include "cr90_ltcd_CircleLight.h"

#include "bn_sprite_items_cr90_ltcd_light_w16.h"
#include "bn_sprite_items_cr90_ltcd_light_w32.h"
#include "bn_sprite_items_cr90_ltcd_light_w64.h"
#include "bn_sprite_items_cr90_ltcd_light_w8.h"

namespace cr90::ltcd
{

static auto create_circle_light_sprite(const bn::fixed_point& position, int radius) -> bn::sprite_ptr
{
    BN_ASSERT(CircleLight::MIN_RADIUS <= radius && radius <= CircleLight::MAX_RADIUS, "CircleLight radius=", radius,
              " not in [", CircleLight::MIN_RADIUS, "..", CircleLight::MAX_RADIUS, "]");

    if (radius <= 4)
        return bn::sprite_items::cr90_ltcd_light_w8.create_sprite(position, radius);
    else if (radius <= 8)
        return bn::sprite_items::cr90_ltcd_light_w16.create_sprite(position, radius - 5);
    else if (radius <= 16)
        return bn::sprite_items::cr90_ltcd_light_w32.create_sprite(position, radius - 9);
    // radius <= 32
    return bn::sprite_items::cr90_ltcd_light_w64.create_sprite(position, radius - 17);
}

CircleLight::CircleLight(const bn::fixed_point& position, int radius)
    : Entity(position), _spr(create_circle_light_sprite(position, radius)), _radius(radius)
{
    _spr.set_bg_priority(2);
    _spr.set_window_enabled(true);
}

void CircleLight::update(const mj::game_data&, Game&)
{
    // copycat of `bn::sprite_animate_action::update()`, but with `create_circle_light_sprite()`.
    if (animation_running())
    {
        if (_current_wait_updates)
        {
            --_current_wait_updates;
        }
        else
        {
            _radius = _radiuses[_current_radiuses_index];
            _current_wait_updates = _wait_updates;

            if (_current_radiuses_index == 0 || _radiuses[_current_radiuses_index - 1] != _radius)
            {
                _spr = create_circle_light_sprite(position(), _radius);
                _spr.set_window_enabled(true);
            }

            if (_forever && _current_radiuses_index == _radiuses.size() - 1)
            {
                _current_radiuses_index = 0;
            }
            else
            {
                ++_current_radiuses_index;
            }
        }
    }
}

void CircleLight::set_radius(int radius)
{
    _spr = create_circle_light_sprite(position(), radius);
    _spr.set_window_enabled(true);

    // stop animation
    _radiuses.clear();
}

void CircleLight::set_radius_animation(bool forever, int wait_updates, const bn::span<const uint16_t>& radiuses)
{
    BN_ASSERT(1 < radiuses.size() && radiuses.size() <= _radiuses.max_size(), "radiuses count=", radiuses.size(),
              " not in [2..", _radiuses.max_size(), "]");

    _radiuses.clear();
    for (int i = 0; i < radiuses.size(); ++i)
    {
        const uint16_t radius = radiuses[i];

        BN_ASSERT(MIN_RADIUS <= radius && radius <= MAX_RADIUS, "CircleLight radiuses[", i, "]=", radius, " not in [",
                  MIN_RADIUS, "..", MAX_RADIUS, "]");

        _radiuses.push_back(radius);
    }

    _wait_updates = wait_updates;
    _current_radiuses_index = 0;
    _current_wait_updates = 0;
    _forever = forever;
}

bool CircleLight::animation_running() const
{
    return !_radiuses.empty() && _current_radiuses_index != _radiuses.size();
}

void CircleLight::set_position(const bn::fixed_point& position)
{
    Entity::set_position(position);

    _spr.set_position(position);
}

void CircleLight::set_x(bn::fixed x)
{
    Entity::set_x(x);

    _spr.set_x(x);
}

void CircleLight::set_y(bn::fixed y)
{
    Entity::set_y(y);

    _spr.set_y(y);
}

} // namespace cr90::ltcd
