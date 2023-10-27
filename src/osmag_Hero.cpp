#include "osmag_Hero.h"

#include "bn_sprite_items_osmag_fantome.h"
#include "bn_sprite_items_osmag_cible.h"

namespace osmag{

Hero::Hero(){
}

void Hero::init(Physique* _phys, Ogive* _ogive){
    this->phys = _phys;
    this->ogive = _ogive;

    sprite_perso = bn::sprite_items::osmag_fantome.create_sprite(phys->get_pos0(), 3);
    sprite_perso->set_horizontal_flip(true);
    sprite_perso->set_bg_priority(1);    
    actualiser_sprite_hero();

    hero_palette = sprite_perso->palette();
    hero_palette->set_fade(bn::colors::green, 0.0);

    sprite_cible = bn::sprite_items::osmag_cible.create_sprite(0,0);
    sprite_cible->set_bg_priority(1);    
    positionner_viseur();
}

void Hero::positionner_viseur(){
    if(!sprite_cible.has_value()){
        return;
    }

    bn::fixed distanceFixe = 32;
    bn::fixed decalage_x = sprite_perso->x() +  distanceFixe * bn::degrees_lut_cos(phys->get_angle());
    bn::fixed decalage_y = sprite_perso->y() - distanceFixe * bn::degrees_lut_sin(phys->get_angle());

    sprite_cible->set_position(decalage_x, decalage_y);

}




void Hero::update(){

    bool angle_est_modifie = false;

    if(bn::keypad::a_held() && !tir_est_lance){

        peut_charger_canon = phys->augmenter_v0();
        if(!peut_charger_canon){
            tir_est_lance = true;
            ogive->lancer();
            sprite_cible.reset();
            modifier_couleur_hero(0);
        }
        else{
            modifier_couleur_hero(1);
        }
    }
    else if(bn::keypad::a_released() && !tir_est_lance){
        tir_est_lance = true;
        ogive->lancer();
        sprite_cible.reset();
        modifier_couleur_hero(0);
    }

    if(bn::keypad::up_held() && !tir_est_lance){
        retardateur_angle = (retardateur_angle + 1) % 2;
        if(retardateur_angle == 0){
            angle_est_modifie = phys->augmenter_angle();
        }
    }
    else if(bn::keypad::down_held() && !tir_est_lance){
       retardateur_angle = (retardateur_angle + 1) % 2;
       if(retardateur_angle == 0){
            angle_est_modifie = phys->diminuer_angle();
       }

    }

    if(bn::keypad::left_held() && !tir_est_lance){
       phys->reculer_x0();
       sprite_perso->set_x(phys->get_pos0().x());
       positionner_viseur();
    }
    else if(bn::keypad::right_held() && !tir_est_lance){
       phys->avancer_x0();
       sprite_perso->set_x(phys->get_pos0().x());
       positionner_viseur();
    }



    if(angle_est_modifie){
        actualiser_sprite_hero();
        positionner_viseur();


    }


    //sprite.set_rotation_angle(angle);

}

void Hero::modifier_couleur_hero(int choix){
    bn::fixed fade_intensity = hero_palette->fade_intensity();

    switch(choix){
    case -1 : hero_palette->set_fade_intensity(bn::max(fade_intensity - 0.0018, bn::fixed(0))); break;
    case 0 : hero_palette->set_fade_intensity(0.0); break;
    case 1: hero_palette->set_fade_intensity(bn::min(fade_intensity +  0.003, bn::fixed(0.8))); break;
    default: break;
    }
}

bool Hero::get_tir_est_lance(){
    return tir_est_lance;
}

void Hero::clear(){
    sprite_perso.reset();
}

void Hero::actualiser_sprite_hero(){
    if(phys->get_angle() <= 90 && phys->get_angle() > 60){
        sprite_perso->set_item(bn::sprite_items::osmag_fantome, 0);
    }
    else if(phys->get_angle() <= 60 && phys->get_angle() > 30){
        sprite_perso->set_item(bn::sprite_items::osmag_fantome, 1);
    }
    else if(phys->get_angle() <= 30 && phys->get_angle() > 0){
        sprite_perso->set_item(bn::sprite_items::osmag_fantome, 2);
    }
    else if(phys->get_angle() >= 0 && phys->get_angle() > 330){
        sprite_perso->set_item(bn::sprite_items::osmag_fantome, 3);
    }
    else if(phys->get_angle() >= 330 && phys->get_angle() >= 300){
        sprite_perso->set_item(bn::sprite_items::osmag_fantome, 4);
    }
    else if(phys->get_angle() > 300 && phys->get_angle() > 270){
        sprite_perso->set_item(bn::sprite_items::osmag_fantome, 5);
    }
    else if(phys->get_angle() == 270){
        sprite_perso->set_item(bn::sprite_items::osmag_fantome, 6);
    }
}


}
