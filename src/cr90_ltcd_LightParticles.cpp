// SPDX-FileCopyrightText: Copyright 2023 copyrat90 <copyrat90@gmail.com>
// SPDX-License-Identifier: MIT

#include "cr90_ltcd_LightParticles.h"

#include "cr90_ltcd_Game.h"

namespace cr90::ltcd
{

void LightParticles::add_particle(const bn::fixed_point& position)
{
    if (_particles_queue.full())
        _particles_queue.pop_front();

    CircleLight particle(position, 6);
    particle.set_radius_animation(false, 4, bn::array<const uint16_t, 5>{4, 3, 2, 1, 0});

    _particles_queue.push_back(std::move(particle));
}

void LightParticles::update(const mj::game_data& data, Game& game)
{
    constexpr bn::fixed MOVE_SPEED = 2.5;

    for (auto& particle : _particles_queue)
    {
        particle.set_y(particle.y() - MOVE_SPEED * game.speed());
        particle.update(data, game);
    }

    while (!_particles_queue.empty() && !_particles_queue.front().animation_running())
        _particles_queue.pop_front();
}

} // namespace cr90::ltcd
