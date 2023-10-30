
#include "hh_spider.h"

#include "bn_sprite_items_hh_monster.h"
#include "bn_sprite_items_hh_true_spider_bart.h"
#include "bn_sprite_items_hh_true_spider_frank.h"
#include "bn_math.h"

namespace hh{

spider::spider(bn::fixed x, bn::fixed y, bn::fixed tempo) : 
    entity(x, y, 16, 16, bn::fixed(0.5) * tempo, 0),
    _sprite(bn::sprite_items::hh_monster.create_sprite(x,y)),
    _anim(bn::create_sprite_animate_action_forever(_sprite, 5, 
            bn::sprite_items::hh_monster.tiles_item(), 0, 1)) {
}

void spider::update(const mj::game_data& data){
    entity::update(data);
    if(_can_move){
        _sprite.set_position(_pos);
        _anim.update();
    }
 }

void spider::point_at(bn::fixed_point pos){
    if(_can_move){
        //this gives us the angle as a value between -0.5 and 0.5
        bn::fixed angle = bn::diamond_angle((pos.y() - _pos.y()).floor_integer(),
                                            (pos.x() - _pos.x()).floor_integer());

        //normalize it
        //scale it up to be within our 8 directions
        angle *= 8;
        //convert it back to my dumbass way of measuring these stupid things
        int8_t maybe_negative_dir = angle.round_integer();
        _direction = (8 + maybe_negative_dir) % 8;
        _sprite.set_rotation_angle((270 + 360 - _direction * 45) % 360);
    }
}

void spider::lights_on(bn::random &rand){
    if(!_lights_on){
        uint8_t costume = rand.get_int(2);
        _sprite.set_rotation_angle(0);

        // int8_t sign = 1;

        // if(_direction == 7 || _direction <= 2 ){
        //     sign = -1;
        // }


        // bn::fixed_point new_pos((_pos.x() + (23 * sign)), (_pos.y() - 2));
        bn::fixed_point new_pos(_pos.x(), _pos.y() - 23);
        

        _sprite.set_z_order(10);

        if(costume == 0){
            _sprite = bn::sprite_items::hh_true_spider_frank.create_sprite(new_pos);
        }else if(costume == 1){
            _sprite = bn::sprite_items::hh_true_spider_bart.create_sprite(new_pos);
        }else{
            // _sprite = bn::sprite_items::hh_true_spider_herob.create_sprite(new_pos);
        }
        _sprite.set_z_order(9);

        // if(sign == -1){
        //     _sprite.set_horizontal_flip(true);
        // }
    }

    _lights_on = true;

}

void spider::disable_movement(){
    entity::disable_movement();
    _anim.reset(); 
    _sprite.set_item(bn::sprite_items::hh_monster, 0);
}

}
