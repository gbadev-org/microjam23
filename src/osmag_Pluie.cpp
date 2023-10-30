#include "osmag_Pluie.h"

#include "bn_sprite_items_osmag_araignee.h"

namespace osmag{


Pluie::Pluie(){

}

void Pluie::init(bn::fixed _acceleration_vent, bn::random& _rand){
    this->acceleration_vent = _acceleration_vent/15; // NOTE on baisse le vent car va trop vite
    this->rand = _rand;
    for(int i = 0 ; i < vecteur_araignees.max_size() ; i++){
        bn::fixed_point pos = calculer_position_initiale_araignee(false);

        int variante = rand.get_int(0, 3);
        vecteur_araignees.push_back(bn::sprite_items::osmag_araignee.create_sprite( pos, variante));
        int angle_alea = rand.get_int(0, 361);
        vecteur_araignees.back().set_rotation_angle(angle_alea);
        vecteur_araignees.back().set_double_size_mode(bn::sprite_double_size_mode::DISABLED);

    }
}

void Pluie::larguer_araignee(){

}

void Pluie::update(){
    for(bn::sprite_ptr& araignee : vecteur_araignees){
        if(araignee.y() - 16 > 80){
            bn::fixed_point pos = calculer_position_initiale_araignee(true);
            int variante = rand.get_int(0, 3);
            araignee.set_item(bn::sprite_items::osmag_araignee, variante);
            araignee.set_position(pos);

            int angle_alea = rand.get_int(0, 361);
            araignee.set_rotation_angle(angle_alea);

        }
    }

    for(bn::sprite_ptr& araignee : vecteur_araignees){
        araignee.set_position(araignee.position() + bn::fixed_point(acceleration_vent, 1));

        bn::fixed rotation_angle = araignee.rotation_angle() + 6;

        if(rotation_angle >= 360)
        {
            rotation_angle -= 360;
        }

        araignee.set_rotation_angle(rotation_angle);
    }
}

bn::fixed_point Pluie::calculer_position_initiale_araignee(bool est_update){
    bn::fixed x_aleatoire;
    bn::fixed y_aleatoire;
    //BN_LOG(acceleration_vent);

    if(est_update){
        y_aleatoire = rand.get_fixed(-135, -90);
    }
    else{
        y_aleatoire = rand.get_fixed(-135, 90);
    }

    if(acceleration_vent < 0){
        x_aleatoire = rand.get_fixed(-160 - acceleration_vent*55 , 160 - acceleration_vent*55 );
    }
    else if(acceleration_vent > 0){
        x_aleatoire = rand.get_fixed(-160 + acceleration_vent, 160 + acceleration_vent);
    }
    else{
        x_aleatoire = rand.get_fixed(-160, 160);

    }
    //BN_LOG("araignee : ", x_aleatoire, ", ", y_aleatoire);
    return bn::fixed_point(x_aleatoire, y_aleatoire);

}

void Pluie::clear(){
    vecteur_araignees.clear();
}

}

