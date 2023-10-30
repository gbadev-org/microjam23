
#include "hh_ghost.h"

#include "bn_sprite_items_hh_monster.h"
#include "bn_sprite_items_hh_true_ghost.h"

namespace hh{

ghost::ghost(bn::fixed x, bn::fixed y, uint8_t initial_direction, bn::fixed tempo, bool pacman_ghost) : 
    entity(x, y, 12, 16, bn::fixed(1.5) * tempo, initial_direction),
    _sprite(bn::sprite_items::hh_monster.create_sprite(x,y,pacman_ghost ? 5 : 2)) {
    
    if(pacman_ghost){
        _flip_anim.emplace(_sprite, 5);
    }else{
        _anim.emplace(bn::create_sprite_animate_action_forever(
            _sprite, 5, bn::sprite_items::hh_monster.tiles_item(),
            2, 3));
    }

}

void ghost::update(const mj::game_data& data){

    if(_can_move && hitting_wall()){
        bounce_off_walls();
    }

    entity::update(data);
    _sprite.set_position(_pos);

    if(_can_move){
        if(_anim){
            _anim->update();

            if(2 <= _direction && _direction < 6){
                _sprite.set_horizontal_flip(false);
            }else{
                _sprite.set_horizontal_flip(true);
            }
        }
        if(_flip_anim) _flip_anim->update();
    }
}

void ghost::lights_on(bn::random &rand){
    if(!_lights_on){
        uint8_t graphics_index;
        if(_flip_anim){
            //flip anim = pacman ghost
            graphics_index = 2;
        }else{

            //75% chance for the if block, 25 for the else block
            if(rand.get_int(8)){
                graphics_index = 0;
            }else{
                graphics_index = 1;
            }
        }
        _sprite.set_item(bn::sprite_items::hh_true_ghost, graphics_index);
        _lights_on = true;
    }
}

void ghost::disable_movement(){
    entity::disable_movement();
    if(_anim) _anim->reset(); 
}

}
