#include "jim_player.h"

#include "bn_keypad.h"

#include "bn_sprite_items_jim_player_sprites.h"

namespace jim
{
    player::player(bn::affine_bg_map_item map) : Entity(1, jim::Entity_Type::PLAYER, bn::sprite_items::jim_player_sprites.create_sprite(0, 0, 0), bn::sprite_items::jim_player_sprites, bn::fixed_rect(0, 0, 12, 14)),
                                                 _player_animation(bn::create_sprite_animate_action_forever(this->sprite, 6, this->tiles, 0, 0, 0, 0, 0, 0)), collision_map(map), base_palette(sprite.palette())
    {
        set_moving_direction(IDLE);
        _dead = false;
        set_vel_x(0);
        set_vel_y(0);
        drawable = true;
        _is_invulnerable = true;
    }

    player::player(int _x, int _y, bn::affine_bg_map_item map) : Entity(1, jim::Entity_Type::PLAYER, bn::sprite_items::jim_player_sprites.create_sprite(_x, _y, 0), bn::sprite_items::jim_player_sprites, bn::fixed_rect(_x, _y, 12, 14)),
                                                                 _player_animation(bn::create_sprite_animate_action_forever(this->sprite, 6, this->tiles, 0, 0, 0, 0, 0, 0)), collision_map(map), base_palette(sprite.palette())
    {
        set_moving_direction(IDLE);
        _dead = false;
        set_vel_x(0);
        set_vel_y(0);
        _health = 3;
        _invuln_time = 30;
        drawable = true;
        _is_invulnerable = true;

    }

    int player::get_health()
    {
        return _health;
    }

    void player::update_entity()
    {
        set_vel_x(0);
        set_vel_y(0);

        if(_invuln_time > 0)
        {
            _invuln_time -= 1;

        }else if(_invuln_time == 0 && _is_invulnerable == true)
        {
            base_palette.set_inverted(false);
            _is_invulnerable = false;
        }

        if(_health <= 0)
        {
            _dead = true;
        }

        // direction speed setting
        if (bn::keypad::right_held())
        {
            set_vel_x(2);
        }

        if (bn::keypad::left_held())
        {
            set_vel_x(-2);
        }
        if (bn::keypad::up_held())
        {
            set_vel_y(-2);
        }

        if (bn::keypad::down_held())
        {
            set_vel_y(2);
        }

        check_move_direction();
        
        switch(get_moving_direction())
        {
            case IDLE:
                _player_animation = bn::create_sprite_animate_action_forever(this->sprite, 6, this->tiles, 0, 0, 0, 0, 0, 0);
                break;
            case NORTH:
                 if(_player_animation.graphics_indexes().front() != 18) 
                    _player_animation = bn::create_sprite_animate_action_forever(this->sprite, 6, this->tiles, 18, 4, 4, 18, 16, 16);
                break;
            case SOUTH:
                if(_player_animation.graphics_indexes().back() != 3)
                    _player_animation = bn::create_sprite_animate_action_forever(this->sprite, 6, this->tiles, 0, 15, 15, 0, 3, 3);
                break;
            case WEST:
                if(_player_animation.graphics_indexes().front() != 10)
                    _player_animation = bn::create_sprite_animate_action_forever(this->sprite, 6, this->tiles, 10, 17, 17, 10, 22, 22);
                break;
            case EAST:
                if(_player_animation.graphics_indexes().front() != 6)
                    _player_animation = bn::create_sprite_animate_action_forever(this->sprite, 6, this->tiles, 6, 13, 13, 6, 7, 7);
                break;
            case SOUTHWEST:
                if(_player_animation.graphics_indexes().front() != 5)
                    _player_animation = bn::create_sprite_animate_action_forever(this->sprite, 6, this->tiles, 5, 8, 8, 5, 22, 22);
                break;
            case SOUTHEAST:
                if(_player_animation.graphics_indexes().front() != 1)
                    _player_animation = bn::create_sprite_animate_action_forever(this->sprite, 6, this->tiles, 1, 19, 19, 1, 23, 23);
                break;
            case NORTHWEST:
                if(_player_animation.graphics_indexes().front() != 14)
                    _player_animation = bn::create_sprite_animate_action_forever(this->sprite, 6, this->tiles, 14, 20, 20, 14, 21, 21);
                break;
            case NORTHEAST:
                if(_player_animation.graphics_indexes().front() != 11)
                    _player_animation = bn::create_sprite_animate_action_forever(this->sprite, 6, this->tiles, 11, 9, 9, 11, 12, 12);
                break;
            default:
                //something bad happened...
                _player_animation = bn::create_sprite_animate_action_forever(this->sprite, 6, this->tiles, 2, 2, 2, 2, 2, 2);
                break;
        }

        //could probably clean this up using rectangle intersection call
        if (check_collision_map(get_hitbox().left() + get_vel_x(), get_hitbox().top() + get_vel_y(), collision_map) ||
            check_collision_map(get_hitbox().right() + get_vel_x(), get_hitbox().top() + get_vel_y(), collision_map) ||
            check_collision_map(get_hitbox().left() + get_vel_x(), get_hitbox().bottom() + get_vel_y(), collision_map) ||
            check_collision_map(get_hitbox().right() + get_vel_x(), get_hitbox().bottom() + get_vel_y(), collision_map))
        {
            set_vel_x(0);
            set_vel_y(0);
        }

        sprite.set_position(get_sprite().x() + get_vel_x(), get_sprite().y() + get_vel_y());
        _hitbox.set_position(get_sprite().x(), get_sprite().y());
        _player_animation.update();
    }

    void player::do_contact_damage()
    {
        _is_invulnerable = true;
        _invuln_time = 18;
        base_palette.set_inverted(true);
        _health -= 1;
        
    }

    void player::entity_collision(Entity_Type t)
    {
        
        switch(t)
        {
            case Entity_Type::ENEMY:
                //_dead = true;
                if(!_is_invulnerable)
                {
                    do_contact_damage();
                }
                break;
            default:
                //wtf happened...
                break;
        }
    }

    bool player::is_dead()
    {
        return _dead;
    }
}
