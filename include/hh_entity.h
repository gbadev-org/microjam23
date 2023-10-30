#ifndef HH_ENTITY_H
#define HH_ENTITY_H

#include "bn_fixed_rect.h"
#include "mj_game_data.h"


namespace hh {
class entity {
public: 
    virtual ~entity() = default;
    entity(bn::fixed x, bn::fixed y, bn::fixed width, bn::fixed height, bn::fixed speed, uint8_t direction);
    virtual void update(const mj::game_data& data);
    const bn::fixed_rect &hitbox() {return _hitbox;}
    const bn::fixed_point &pos() {return _pos;}
    virtual void disable_movement() { _can_move = false;}

    virtual void lights_on(bn::random &rand)=0;

protected:
    //diagonal scalar -- approximately sqrt(2) / 2
    //multiply this by our speed to get the correct x and y components.
    // of speed, while traveling at a 45 degree angle.
    const bn::fixed _ds = bn::fixed(0.70710678118);

    /// @brief Represents directions in the increment of 45 degrees.
    // 0 is center right, and the . 
    // 8 is a special value indicating center.
    const int8_t _center = 8, _right = 0, _down_right = 1, 
        _down = 2, _down_left = 3, _left = 4, _up_left = 5,
        _up = 6, _up_right = 7;

    bn::fixed_point _pos;
    bn::fixed_rect _hitbox;
    bn::fixed _speed;

    //map boundaries
    const bn::fixed _lbound = -110, _rbound = 110, _tbound = -75, _bbound = 60;

    uint8_t _direction;
    bool _can_move, _lights_on;

    bool hitting_wall();
    bool hitting_left_wall();
    bool hitting_right_wall();
    bool hitting_top_wall();
    bool hitting_bottom_wall();

    void bounce_off_walls();
};

}

#endif
