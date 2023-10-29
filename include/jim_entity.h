#ifndef JIM_ENTITY_H
#define JIM_ENTITY_H

#include "bn_affine_bg_map_item.h"
#include "bn_sprite_ptr.h"
#include "bn_fixed_rect.h"
#include "bn_sprite_item.h"

namespace jim
{
    enum Entity_Type
    {
        PLAYER = 1,
        ENEMY = 2,
        PROJECTILE = 3,
        ITEM = 4
    };

    enum Move_Direction
    {
        NORTH = 0,
        SOUTH = 1,
        EAST = 2,
        WEST = 3,
        NORTHWEST = 4,
        NORTHEAST = 5,
        SOUTHWEST = 6,
        SOUTHEAST = 7,
        IDLE = 8
    };

    class Entity
    {
        protected:
            int id;
            bn::fixed _vel_x;
            bn::fixed _vel_y;

            Entity_Type type;
            bn::sprite_ptr sprite;
            bn::sprite_tiles_item tiles;
            bn::fixed_rect _hitbox;
            bool drawable;
            Move_Direction moving_direction;

        public:

            Entity(int id, Entity_Type type, const bn::sprite_ptr& sprite, bn::sprite_item item, bn::fixed_rect hitbox);
            virtual ~Entity() = default;

            int get_id();
            Entity_Type get_type();

            bn::fixed_rect get_hitbox();

            bn::sprite_ptr get_sprite();

            bool is_drawable();

            void set_drawable(bool);

            void set_sprite(bn::sprite_ptr _sp);

            bn::fixed get_vel_x();
            bn::fixed get_vel_y();

            Move_Direction get_moving_direction();

            void set_moving_direction(Move_Direction);

            void move_to(int x, int y);
            void set_vel_x(bn::fixed xs);
            void set_vel_y(bn::fixed yss);
            void check_move_direction();
            bn::fixed_point get_sprite_position();
            bool check_collision_map(bn::fixed x, bn::fixed y, bn::affine_bg_map_item cell);

            virtual void entity_collision([[maybe_unused]] Entity_Type _type);
            virtual void update_entity();
    };

}

#endif
