// SPDX-FileCopyrightText: Copyright 2023 copyrat90 <copyrat90@gmail.com>
// SPDX-License-Identifier: MIT

#include "cr90_ltcd_CircleCollider.h"

namespace cr90::ltcd
{

CircleCollider::CircleCollider(const bn::fixed_point position, bn::fixed radius) : Entity(position), _radius(radius)
{
}

bool CircleCollider::collide_with(const CircleCollider& other) const
{
    const auto dx = x() - other.x();
    const auto dy = y() - other.y();
    const auto rr = _radius + other._radius;

    return dx * dx + dy * dy < rr * rr;
}

} // namespace cr90::ltcd
