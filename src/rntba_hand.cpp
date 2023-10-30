#include "rntba_hand.h"

#include "bn_keypad.h"
//#include "bn_sound_items.h"

#include "bn_sprite_tiles_ptr.h"

#include "bn_sprite_items_rntba_hand.h"
#include "bn_sprite_items_rntba_arm.h"
#include "bn_sprite_items_rntba_elbow.h"

namespace rntba {

hand::hand() :
    _hand(bn::sprite_items::rntba_hand.create_sprite(_start_x, _start_y, 1))
    , _arm(bn::sprite_items::rntba_arm.create_sprite(_start_x+_arm_x, _start_y+_arm_y))
    , _elbow(bn::sprite_items::rntba_elbow.create_sprite(_start_x+_arm_x+64, _start_y+_arm_y))
{
}

int hand::process()
{
    if (_stop)
        return _mode;

    switch (_mode){
        case 4:
        case 1:
            _mode = 1;
            process_flick();
            break;
        case 2:
            process_retract();
            break;
        case 3:
            return 3;
        case 0:
            process_idle();
            break;
        default:
            break;
    }

    return _mode;
}

int hand::process_idle(){

  // FLICK INITIATED
    if(bn::keypad::a_pressed()){
        _mode = 4;

        _hand.set_x(_flick_x);
        _arm.set_x(_flick_x + _arm_x);
        _elbow.set_x(_flick_x + _arm_x + 64);

        bn::sprite_tiles_ptr tiles = _hand.tiles();
        tiles.set_tiles_ref(bn::sprite_items::rntba_hand.tiles_item(), 0);

        _timer = _flick_wait;

        return _mode;
    }

  // Determine velocity
    int vel = 0;
    if(bn::keypad::up_held()){
        vel -= 1;
    }
    else if (bn::keypad::down_held()){
        vel += 1;
    }

  // Hand was not moved
    if (!vel)
        return 0;

  // Move hand, ensure it stays in bounds
    int y = _hand.y().floor_integer();
    if(y+vel > _y_max){
        _hand.set_y(_y_max);
        _arm.set_y(_y_max + _arm_y);
        _elbow.set_y(_y_max + _arm_y);
    }
    else if (y+vel < _y_min){
        _hand.set_y(_y_min);
        _arm.set_y(_y_min + _arm_y);
        _elbow.set_y(_y_min + _arm_y);
    }
    else{
        _hand.set_y(y+vel);
        _arm.set_y(y+vel+_arm_y);
        _elbow.set_y(y + vel + _arm_y);
    }

    return 0;
}

int hand::process_flick(){
    _timer--;
    if (!_timer){
        _mode = 2;
        _timer = (_start_x - _flick_x) / _step_x;
    }
    return _mode;
}

int hand::process_retract(){
    _timer--;

    int x = _hand.x().floor_integer() + _step_x;
    _hand.set_x(x);
    _arm.set_x(x + _arm_x);
    _elbow.set_x(x + _arm_x + 64);

    if (!_timer){
        _mode = 0;
        bn::sprite_tiles_ptr tiles = _hand.tiles();
        tiles.set_tiles_ref(bn::sprite_items::rntba_hand.tiles_item(), 1);
    }
    return _mode;
}

void hand::thumbsup(){
    bn::sprite_tiles_ptr tiles = _hand.tiles();
    tiles.set_tiles_ref(bn::sprite_items::rntba_hand.tiles_item(), 3);
    _stop = true;
    _hand.set_y(_hand.y().floor_integer() - 7);
}

}
