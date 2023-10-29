#include "jim_ghost.h"

#include "bn_math.h"

#include "bn_sprite_items_jim_ghost.h"

namespace jim
{
    ghost::ghost() : Entity(2, Entity_Type::ENEMY, bn::sprite_items::jim_ghost.create_sprite(0, 0, 0),
                            bn::sprite_items::jim_ghost,
                            bn::fixed_rect(0, 0, 12, 12)),
                     _ghost_animation(bn::create_sprite_animate_action_forever(sprite, 0, tiles, 0, 0, 0, 0, 0, 0))
    {
        set_vel_y(0);
        set_vel_x(0);
        set_drawable(false);
    }

    ghost::ghost(int x, int y, jim::player *_p, int diff, bn::random* rand) : Entity(2, Entity_Type::ENEMY, bn::sprite_items::jim_ghost.create_sprite(x, y, 0),
                                                                         bn::sprite_items::jim_ghost,
                                                                         bn::fixed_rect(x, y, 12, 12)),
                                                                  _ghost_animation(bn::create_sprite_animate_action_forever(sprite, 0, tiles, 0, 0, 0, 0, 0, 0))
    {
        set_vel_y(0);
        set_vel_x(0);
        set_drawable(true);
        difficulty = diff;
        _player = _p;

        _rand = rand;
        _rand->update();
        _path_timer = 30;

        _speed = bn::clamp((difficulty + _rand->get_fixed(-2.0, 2.0)), bn::fixed(1.0), bn::fixed(2.0));

        set_vel_x(0);
        set_vel_y(0);
    }

    bn::fixed ghost::sqr(bn::fixed val)
    {
        return val * val;
    }

    bn::sprite_animate_action<8> ghost::get_animation()
    {
        return _ghost_animation;
    }

    void ghost::set_animation(bn::sprite_animate_action<6> act)
    {
        _ghost_animation = bn::move(act);
    }

    void ghost::update_entity()
    {
        if(_path_timer > 0)
        {
            _path_timer--;
        }else
            _path_timer = 30 / difficulty;

        if (_hitbox.center() != _player->get_hitbox().center() && _path_timer == 0)
        {  
            _rand->update();
            bn::fixed offset = _rand->get_fixed(-4,4);
            vec_x = bn::sqrt(sqr((_player->get_hitbox().x() + offset) - _hitbox.x()) + sqr((_player->get_hitbox().y() + offset) - _hitbox.y()));
            vec_y = bn::sqrt(sqr((_player->get_hitbox().y() + offset) - _hitbox.y()) + sqr((_player->get_hitbox().x() + offset) - _hitbox.x()));
            if (vec_x == 0.0)
            {
                set_vel_x(0);
            }
            else
                set_vel_x(_speed * (((_player->get_hitbox().x() + offset) - _hitbox.x())).safe_division(vec_x));

            if (vec_y == 0.0)
            {
                set_vel_y(0);
            }
            else
                set_vel_y(_speed * (((_player->get_hitbox().y() + offset) - _hitbox.y())).safe_division(vec_y));
        }

        //no need to update animation when not on screen
        if (is_drawable())
        {
            check_move_direction();
            switch (get_moving_direction())
            {
            case IDLE:
                _ghost_animation = bn::create_sprite_animate_action_forever(this->sprite, 8, this->tiles, 0, 0, 0, 0, 0, 0, 0, 0);
                break;
            case EAST:
                if (this->_ghost_animation.graphics_indexes().front() != 5)
                    _ghost_animation = bn::create_sprite_animate_action_forever(this->sprite, 8, this->tiles, 5, 5, 5, 6, 6, 6, 6, 5);
                break;
            case WEST:
                if (this->_ghost_animation.graphics_indexes().front() != 1)
                    _ghost_animation = bn::create_sprite_animate_action_forever(this->sprite, 8, this->tiles, 1, 1, 1, 1, 2, 2, 2, 2);
                break;
            case NORTH:
                if (this->_ghost_animation.graphics_indexes().front() != 4)
                    _ghost_animation = bn::create_sprite_animate_action_forever(this->sprite, 8, this->tiles, 4, 4, 3, 3, 4, 4, 7, 7);
                break;
            case SOUTH:
                if (this->_ghost_animation.graphics_indexes().back() != 1)
                    _ghost_animation = bn::create_sprite_animate_action_forever(this->sprite, 8, this->tiles, 0, 0, 5, 5, 0, 0, 1, 1);
                break;
            case NORTHWEST:
                if (this->_ghost_animation.graphics_indexes().front() != 3)
                    _ghost_animation = bn::create_sprite_animate_action_forever(this->sprite, 8, this->tiles, 3, 3, 2, 2, 4, 4, 2, 2);
                break;
            case NORTHEAST:
                if (this->_ghost_animation.graphics_indexes().front() != 7)
                    _ghost_animation = bn::create_sprite_animate_action_forever(this->sprite, 8, this->tiles, 7, 7, 6, 6, 4, 4, 6, 6);
                break;
            case SOUTHWEST:
                if (this->_ghost_animation.graphics_indexes().at(5) != 1)
                    _ghost_animation = bn::create_sprite_animate_action_forever(this->sprite, 8, this->tiles, 1, 1, 2, 2, 1, 1, 0, 0);
                break;
            case SOUTHEAST:
                if (this->_ghost_animation.graphics_indexes().at(4) != 5)
                    _ghost_animation = bn::create_sprite_animate_action_forever(this->sprite, 8, this->tiles, 5, 5, 6, 6, 5, 5, 0, 0);
                break;
            default:
                break;
            }
        }
        sprite.set_position(get_sprite().x() + get_vel_x(), get_sprite().y() + get_vel_y());
        _hitbox.set_position(get_sprite().x(), get_sprite().y());
        _ghost_animation.update();
    }

    void ghost::entity_collision([[maybe_unused]] Entity_Type _t)
    {
    }
}
