#include "jim_entity.h"

#include "bn_affine_bg_map_cell_info.h"

namespace jim
{
    Entity::Entity(int _id, jim::Entity_Type _type, const bn::sprite_ptr& _sprite, bn::sprite_item _item, bn::fixed_rect _h) : sprite(_sprite),
                                                                                                                              tiles(_item.tiles_item())
    {
        id = _id;
        type = _type;
        _hitbox = _h;
    }

    int Entity::get_id() { return id; }
    Entity_Type Entity::get_type() { return type; }

    bn::fixed_rect Entity::get_hitbox() { return _hitbox; }

    bn::fixed Entity::get_vel_x() { return _vel_x; }
    bn::fixed Entity::get_vel_y() { return _vel_y; }

    void Entity::set_vel_x(bn::fixed xs) { _vel_x = xs; }
    void Entity::set_vel_y(bn::fixed ys) { _vel_y = ys; }

    Move_Direction Entity::get_moving_direction()
    {
        return moving_direction;
    }
    bool Entity::is_drawable()
    {
        return drawable;
    }

    void Entity::set_drawable(bool _d)
    {
        drawable = _d;
        sprite.set_visible(drawable);
    }

    bn::sprite_ptr Entity::get_sprite()
    {
        return sprite;
    }

    void Entity::set_sprite(bn::sprite_ptr _sp)
    {
        sprite = bn::move(_sp);
    }

    void Entity::move_to(int x, int y)
    {
        sprite.set_x(x);
        sprite.set_y(y);
        _hitbox.set_position(sprite.x(), sprite.y());
    }

    bool Entity::check_collision_map(bn::fixed x, bn::fixed y, bn::affine_bg_map_item map)
    {
        bn::affine_bg_map_cell entity_cell = map.cell(x.integer() / 8, y.integer() / 8);

        if (bn::affine_bg_map_cell_info(entity_cell).tile_index() > 0)
        {
            return true;
        }

        return false;
    }

    void Entity::check_move_direction()
    {
        if (get_vel_x() > 0 && get_vel_y() == 0)
        {
            set_moving_direction(EAST);
        }
        else if (get_vel_x() < 0 && get_vel_y() == 0)
        {
            set_moving_direction(WEST);
        }
        else if (get_vel_x() == 0 && get_vel_y() > 0)
        {
            set_moving_direction(SOUTH);
        }
        else if (get_vel_x() == 0 && get_vel_y() < 0)
        {
            set_moving_direction(NORTH);
        }
        else if (get_vel_x() > 0 && get_vel_y() > 0)
        {
            set_moving_direction(SOUTHEAST);
        }
        else if (get_vel_x() > 0 && get_vel_y() < 0)
        {
            set_moving_direction(NORTHEAST);
        }
        else if (get_vel_x() < 0 && get_vel_y() > 0)
        {
            set_moving_direction(SOUTHWEST);
        }
        else if (get_vel_x() < 0 && get_vel_y() < 0)
        {
            set_moving_direction(NORTHWEST);
        }
        else if (get_vel_x() == 0 && get_vel_y() == 0)
        {
            set_moving_direction(IDLE);
        }
    }

    bn::fixed_point Entity::get_sprite_position()
    {
        return sprite.position();
    }

    void Entity::set_moving_direction(Move_Direction d)
    {
        moving_direction = d;
    }

    void Entity::update_entity()
    {
    }

    void Entity::entity_collision([[maybe_unused]] Entity_Type _type)
    {
    }
}
