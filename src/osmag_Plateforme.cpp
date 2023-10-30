#include "osmag_Plateforme.h"

#include "bn_regular_bg_items_osmag_plateforme_haute.h"


namespace osmag{

Plateforme::Plateforme(){
}

void Plateforme::init(){
    bg = bn::regular_bg_items::osmag_plateforme_haute.create_bg(0, 0);
    bg->set_priority(2);    
    boite_collision = bn::top_left_fixed_rect(-120, 80-62, 104-1, 62);
    boite_collision_droite = bn::top_left_fixed_rect(120-23, 80-62, 50, 62);
}

const bn::top_left_fixed_rect& Plateforme::get_boite_collision() const{
    return boite_collision;
}

bool Plateforme::en_collision_avec_ogive(Ogive& ogive){
    return
        ogive.get_boite_collision().intersects(boite_collision) ||
        ogive.get_boite_collision().intersects(boite_collision_droite)
    ;
}

}
