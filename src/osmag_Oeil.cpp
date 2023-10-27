#include "osmag_Oeil.h"

// Sprites
#include "bn_sprite_items_osmag_oeil.h"



namespace osmag{

Oeil::Oeil(){

}

void Oeil::init(bn::random& rand){
    int x0 = rand.get_int(X_MIN, X_MAX + 1);
    int y0 = rand.get_int(Y_MIN, Y_MAX + 1);

    sprite = bn::sprite_items::osmag_oeil.create_sprite(x0, y0);
    animation_action = bn::create_sprite_animate_action_forever(sprite.value(), 10, bn::sprite_items::osmag_oeil.tiles_item(), 0, 1);
    mouvement_action = bn::sprite_move_loop_action(sprite.value(), 42, x0, y0-4); //36

    boite_collision = bn::top_left_fixed_rect(sprite->x() - 16/2 +4, sprite->y() -16/2 +2 , 9, 12);
}

void Oeil::update(){
    animation_action->update();
    mouvement_action->update();
    boite_collision.set_position(sprite->x() -16/2 + 4, sprite->y() -16/2 + 2);
}

const bn::top_left_fixed_rect& Oeil::get_boite_collision() const{
    return boite_collision;
}

void Oeil::log(){

    BN_LOG("-------------------");
    BN_LOG("Sprite -> x:", sprite->x(), ", y: ", sprite->y());
    BN_LOG("Boite -> x:", boite_collision.x(), ", y: ", boite_collision.y());
    BN_LOG("Boite -> left:", boite_collision.left(), ", top: ", boite_collision.top());
}

void Oeil::changer_animation_mort(){
    animation_action = bn::create_sprite_animate_action_forever(sprite.value(), 10, bn::sprite_items::osmag_oeil.tiles_item(), 2, 3);

}

}
