#include "hh_entity.h"

namespace hh {

entity::entity(bn::fixed x, bn::fixed y, bn::fixed width, bn::fixed height, bn::fixed speed, uint8_t direction) : 
    _pos(x, y), 
    _hitbox(x, y, width, height),
    _speed(speed),
    _direction(direction),
    _can_move(true),
    _lights_on(false) {
}

void entity::update([[maybe_unused]] const mj::game_data& data){
    //if your direction is center, you are not moving

    if(_can_move && _direction != _center){
        bn::fixed dx = _speed;
        bn::fixed dy = _speed;
        if(_direction % 2 == 1){
            //if direction is odd, we are going diagonally
            dx *= _ds;
            dy *= _ds;
        }else if(_direction % 4 == 0){
            //multiples of 4 are left and right. this means no y-component
            dy = 0;
        }else if(_direction % 4 == 2){
            //this gives us the other two cardinal directions, up and down
            //these have no x-component
            dx = 0;
        }

        //directions between 3 (down left) and 5 are negative in the x direction
        if(_down_left <= _direction && _direction <= _up_left){
            dx *= -1;
        }

        //directions between 5 and 7 are negative in the y direction
        if(_up_left <= _direction && _direction <= _up_right){
            dy *= -1;
        }

        if(_pos.x() + dx > _rbound || _pos.x() + dx < _lbound){
            dx = 0;
        } 
        if(_pos.y() + dy > _bbound || _pos.y() + dy < _tbound){
            dy = 0;
        } 

        _pos.set_x(_pos.x() + dx);
        _pos.set_y(_pos.y() + dy);
        _hitbox.set_position(_pos);
    }

}

bool entity::hitting_wall(){
    return hitting_bottom_wall() || 
           hitting_top_wall() || 
           hitting_left_wall() || 
           hitting_right_wall();
}

bool entity::hitting_left_wall(){
    return ((_direction == _up_left || _direction == _left || _direction == _down_left)
       && _pos.x() - _speed < _lbound);
}
bool entity::hitting_right_wall(){
    return ((_direction == _up_right || _direction == _right || _direction == _down_right)
       && _pos.x() + _speed > _rbound);
}

bool entity::hitting_top_wall(){
    return ((_direction == _up_left || _direction == _up || _direction == _up_right)
       && _pos.y() - _speed < _tbound);
}

bool entity::hitting_bottom_wall(){
    return ((_direction == _down_left || _direction == _down || _direction == _down_right)
       && _pos.y() + _speed > _bbound);
}

void entity::bounce_off_walls(){
    BN_ASSERT(hitting_wall(), "entity::bounce_off_walls() can only be called if there is a wall to bounce off of");
    //mirror the direction so it bounces off the walls
    int8_t dirmod = 0;
    if(_direction % 2 == 1){
        dirmod = -2;
    }else{
        dirmod = 4;
    }

    _direction = (_direction + dirmod + 8) % 8;

    //jank ass bug fix but fuck it . 
    //if we are still hitting a wall, after that rotation, rotate again
    if(hitting_wall()){
        _direction = (_direction + 4) % 8;            
    }
    // bn::sound_items::hh_ominous.play(1);
}

}
