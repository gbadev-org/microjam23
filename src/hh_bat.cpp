
#include "hh_bat.h"

#include "bn_sprite_items_hh_monster.h"
#include "bn_sprite_items_hh_true_bat.h"
#include "bn_sprite_items_hh_true_bat_shaggy.h"
#include "bn_sprite_items_hh_true_bat_wizard.h"

namespace hh{

bat::bat(bn::fixed x, bn::fixed y, uint8_t initial_direction, bn::fixed tempo, bn::random &rand) : 
    entity(x, y, 16, 16, bn::fixed(1.5) * tempo, initial_direction),
    _rand(rand),
    _tempo(tempo),
    _sprite(bn::sprite_items::hh_monster.create_sprite(x,y, 4)),
    _anim(bn::sprite_vertical_flip_toggle_action(_sprite, 5)),
    _turn_time(((bn::fixed(20) / tempo) * rand.get_fixed(0.75, 1.5)).round_integer()),
    _timer(_turn_time),
    _clockwise(_rand.get_int(2)) {    
}

void bat::update(const mj::game_data& data){
    if(_can_move){
        entity::update(data);
        _sprite.set_position(_pos);
        _anim.update();
    }

    //bouncing off walls logic. kinda jank
    if(_can_move && hitting_wall()){
        bounce_off_walls();
    //     _timer = _turn_time;
    //     int8_t turn_amount = _clockwise ? 2 : -2;
    //     _direction = (_direction + turn_amount) % 8;
    }

    if(_timer == 0){
        _timer = _turn_time;
        int8_t turn_amount = _clockwise ? 1 : -1;
        _direction = (_direction + turn_amount + 8) % 8;

        //every half circle, change the radius of the circle.
        // if(_direction % 4 == 0){
            // _turn_time = ((bn::fixed(20) / _tempo) * _rand.get_fixed(0.75, 1.5)).round_integer();
        // }
    }else{
        --_timer;
    }


}

void bat::lights_on(bn::random &rand){
    _sprite.set_vertical_flip(false);
    if(!_lights_on){
        uint8_t costume = rand.get_int(3);
        _sprite.set_rotation_angle(0);

        int8_t sign = 1;

        if(_sprite.x() > 100) sign = -1;


        bn::fixed_point new_pos((_pos.x() + (22 * sign)), (_pos.y() - 7));
        

        _sprite.set_z_order(10);
        _sprite.set_visible(false);

        if(costume == 0){
            _sprite = bn::sprite_items::hh_true_bat.create_sprite(new_pos);
        }else if(costume == 1){
            _sprite = bn::sprite_items::hh_true_bat_shaggy.create_sprite(new_pos);
        }else{
            _sprite = bn::sprite_items::hh_true_bat_wizard.create_sprite(new_pos);
        }
        _sprite.set_z_order(9);

        if(sign == -1){
            _sprite.set_horizontal_flip(true);
        }
    }

    _lights_on = true;    
}

}
