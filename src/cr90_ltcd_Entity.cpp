// SPDX-FileCopyrightText: Copyright 2023 copyrat90 <copyrat90@gmail.com>
// SPDX-License-Identifier: MIT

#include "cr90_ltcd_Entity.h"

namespace cr90::ltcd
{

Entity::~Entity() = default;

Entity::Entity(const bn::fixed_point& position) : _position(position)
{
}

auto Entity::position() const -> const bn::fixed_point&
{
    return _position;
}

auto Entity::x() const -> bn::fixed
{
    return _position.x();
}

auto Entity::y() const -> bn::fixed
{
    return _position.y();
}

void Entity::set_position(const bn::fixed_point& position)
{
    _position = position;
}

void Entity::set_x(bn::fixed x)
{
    _position.set_x(x);
}

void Entity::set_y(bn::fixed y)
{
    _position.set_y(y);
}

} // namespace cr90::ltcd
