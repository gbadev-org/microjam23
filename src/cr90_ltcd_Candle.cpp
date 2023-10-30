// SPDX-FileCopyrightText: Copyright 2023 copyrat90 <copyrat90@gmail.com>
// SPDX-License-Identifier: MIT

#include "cr90_ltcd_Candle.h"

#include "bn_fixed_point.h"

#include "cr90_ltcd_Game.h"

#include "bn_sprite_items_cr90_ltcd_candle.h"
#include "bn_sprite_items_cr90_ltcd_flying_candle.h"

namespace cr90::ltcd
{

namespace
{
constexpr auto CANDLE_DIFF = bn::fixed_point(0, 14);

constexpr int LIGHT_RADIUS = 10;
constexpr bn::fixed COLL_RADIUS = 4.5;

constexpr int PARTICLE_INTERVAL = 8;

} // namespace

static constexpr auto spr_item(bool flying) -> bn::sprite_item
{
    return flying ? bn::sprite_items::cr90_ltcd_flying_candle : bn::sprite_items::cr90_ltcd_candle;
}

Candle::Candle(const bn::fixed_point& position, bool fire, bool flying, bn::fixed game_speed)
    : Fireable(position, fire, LIGHT_RADIUS, COLL_RADIUS * game_speed, PARTICLE_INTERVAL, {}, {}), _flying(flying),
      _spr_candle(spr_item(flying).create_sprite(position + CANDLE_DIFF, fire))
{
    set_fire(fire);
    set_position(position);
}

void Candle::update(const mj::game_data& data, Game& game)
{
    Fireable::update(data, game);

    if (_anim && !_anim->done())
        _anim->update();
}

bool Candle::flying() const
{
    return _flying;
}

void Candle::set_fire(bool fire)
{
    Fireable::set_fire(fire);

    if (fire)
    {
        _anim = bn::create_sprite_animate_action_forever(_spr_candle, 4, spr_item(_flying).tiles_item(), 2, 3, 2, 1);
    }
    else
    {
        _anim.reset();
        _spr_candle.set_tiles(spr_item(flying()).tiles_item(), 0);
    }
}

void Candle::set_position(const bn::fixed_point& position)
{
    Fireable::set_position(position);

    _spr_candle.set_position(position + CANDLE_DIFF);
    _spr_candle.set_z_order((-position.y()).floor_integer());
}

void Candle::set_x(bn::fixed x)
{
    Fireable::set_x(x);

    _spr_candle.set_x(x + CANDLE_DIFF.x());
}

void Candle::set_y(bn::fixed y)
{
    Fireable::set_y(y);

    _spr_candle.set_y(y + CANDLE_DIFF.y());
    _spr_candle.set_z_order((-y).floor_integer());
}

} // namespace cr90::ltcd
