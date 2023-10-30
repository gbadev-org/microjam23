// SPDX-FileCopyrightText: Copyright 2023 copyrat90 <copyrat90@gmail.com>
// SPDX-License-Identifier: MIT

#pragma once

#include "bn_value_template_actions.h"

#include <functional>

#include "cr90_ltcd_Entity.h"

namespace cr90::ltcd
{

class EntityPositionManager
{
public:
    static auto get(const std::reference_wrapper<Entity> entity) -> const bn::fixed_point&
    {
        return entity.get().position();
    }

    static void set(const bn::fixed_point& position, std::reference_wrapper<Entity> entity)
    {
        entity.get().set_position(position);
    }
};

class EntityMoveLoopAction
    : public bn::loop_value_template_action<std::reference_wrapper<Entity>, bn::fixed_point, EntityPositionManager>
{
public:
    EntityMoveLoopAction(Entity& entity, int duration_updates, const bn::fixed_point& final_position)
        : loop_value_template_action(entity, duration_updates, final_position)
    {
    }

    EntityMoveLoopAction(Entity& entity, int duration_updates, bn::fixed final_x, bn::fixed final_y)
        : loop_value_template_action(entity, duration_updates, bn::fixed_point(final_x, final_y))
    {
    }
};

} // namespace cr90::ltcd
