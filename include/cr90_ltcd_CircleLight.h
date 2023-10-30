// SPDX-FileCopyrightText: Copyright 2023 copyrat90 <copyrat90@gmail.com>
// SPDX-License-Identifier: MIT

#pragma once

#include "cr90_ltcd_Entity.h"

#include "bn_span.h"
#include "bn_sprite_ptr.h"
#include "bn_vector.h"

namespace cr90::ltcd
{

class CircleLight final : public Entity
{
public:
    static constexpr int MAX_FRAMES = 8;

    static constexpr int MIN_RADIUS = 0;
    static constexpr int MAX_RADIUS = 32;

public:
    CircleLight(const bn::fixed_point& position, int radius);

    void update(const mj::game_data&, Game&) override;

public:
    /**
     * @brief Set the `CircleLight` sprite radius.
     * Calling this stops animation, too.
     *
     * @param radius radius in the range [0..32].
     */
    void set_radius(int radius);

    /**
     * @brief Set the `CircleLight` sprite animation.
     *
     * @param radiuses radiuses in the range [0..32]. max frames are 8.
     */
    void set_radius_animation(bool forever, int wait_updates, const bn::span<const uint16_t>& radiuses);

    bool animation_running() const;

public:
    void set_position(const bn::fixed_point&) override;
    void set_x(bn::fixed) override;
    void set_y(bn::fixed) override;

private:
    bn::sprite_ptr _spr;
    int _radius;

    bn::vector<uint16_t, MAX_FRAMES> _radiuses;
    int _wait_updates = 0;
    int _current_radiuses_index = 0;
    int _current_wait_updates = 0;
    bool _forever = true;
};

} // namespace cr90::ltcd
