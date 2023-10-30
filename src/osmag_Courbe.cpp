#include "osmag_Courbe.h"

#include "bn_sprite_items_osmag_pixel.h"

namespace osmag{

Courbe::Courbe(){
}

void Courbe::init(Physique* _phys){
    this->phys = _phys;

    for(int i = 0 ; i < NB_POINTS ; i++){
        sprites.push_back(bn::sprite_items::osmag_pixel.create_sprite(0, 0));
        sprites.back().set_bg_priority(1);
        sprites.back().set_blending_enabled(true);
    }
    update();
}

void Courbe::update(){
    bn::fixed t = phys->DELTA_TEMPS * ECART_TEMPS; // ou 0

    for(int i = 0 ; i < NB_POINTS ; i++){
        sprites[i].set_position(phys->calculer_position_projectile(t));
        t += phys->DELTA_TEMPS * ECART_TEMPS;
    }
}

void Courbe::clear(){
    sprites.clear();
}

void Courbe::clear_partiel(int index_depart){
    for(int i = index_depart ; i < NB_POINTS ; i++){
        sprites[i].set_visible(false);
    }
}




}
