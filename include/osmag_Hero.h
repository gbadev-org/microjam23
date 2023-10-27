#ifndef OSMAG_HERO_H
#define OSMAG_HERO_H

#include "osmag_Ogive.h"
#include "osmag_Physique.h"

namespace osmag{



class Hero{

public:
    bn::optional<bn::sprite_ptr> sprite_perso;
private:

    bn::optional<bn::sprite_ptr> sprite_cible;
    bn::optional<bn::sprite_palette_ptr> hero_palette;


    bool peut_charger_canon = true;
    bool tir_est_lance= false;

    Ogive* ogive;
    Physique * phys;

    int retardateur_angle = 0;


    void actualiser_sprite_hero();

public:    
    Hero();
    void init(Physique* _phys, Ogive* _ogive);
    void update();
    void positionner_viseur();
    void modifier_couleur_hero(int choix);
    bool get_tir_est_lance();
    void clear();

};

}

#endif // OSMAG_HERO_H
