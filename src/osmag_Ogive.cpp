#include "osmag_Ogive.h"
#include "bn_sprite_items_osmag_ogive_fantome.h"

namespace osmag{

Ogive::Ogive()
    : sprite(bn::sprite_items::osmag_ogive_fantome.create_sprite(-80, 64))    
{
    sprite.set_visible(false);
    sprite.set_bg_priority(1);
}

void Ogive::init(Physique* _phys){
    this->phys = _phys;

    sprite.set_position(phys->get_pos0());


    // DEBUG Positionnement de la boite de collision
    boite_collision.set_position(phys->get_pos0().x() - 5, phys->get_pos0().y() - 5);
    boite_collision.set_dimensions(11, 11);


}


void Ogive::update(){
    if(etat == Etat::VOL){

        auto nouvelles_proprietes = phys->calculer_position_et_orientation_projectile(t);

//        bn::fixed_point nouvelle_pos = calculer_position_projectile(t, phys);
        sprite.set_position(nouvelles_proprietes.first);
        sprite.set_rotation_angle(nouvelles_proprietes.second);


        // DEBUG Mise à jour de la position de la boite de collision
        boite_collision.set_position(sprite.x() - 5, sprite.y() - 5);



        t += phys->DELTA_TEMPS;

    }
}

void Ogive::lancer(){
    if(etat == Etat::VOL){
        return;
    }


//    this->angle = angle;
//    this->v0 = vitesse ;// bn::fixed(1.45);//
    t = 0;


    etat = Etat::VOL;
    sprite.set_visible(true);
    //sprite_boite_collision->set_visible(true); // DEBUG decommenter
}



const bn::top_left_fixed_rect& Ogive::get_boite_collision() const{
    return boite_collision;
}




}
