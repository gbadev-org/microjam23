#include "osmag_Arbre.h"

// Sprites
#include "bn_sprite_items_osmag_arbre_tete.h"
#include "bn_sprite_items_osmag_arbre_tronc.h"
#include "bn_sprite_items_osmag_arbre_racines.h"

namespace osmag{

Arbre::Arbre(){
}

void Arbre::init(bn::random& rand){
    // Tirage aléatoire de la taille de l'arbre
    // 0 : juste la tete
    // >= 1 : nombre de sections de tronc
    taille = rand.get_int(TAILLE_MIN, TAILLE_MAX + 1);
    a_tete = rand.get_int(0, 2) == 1 ? true : false;// 0 : pas de tete  | 1:a tete


    // Création des sprites de l'arbre
    // Racines
    sprites.push_back(bn::sprite_items::osmag_arbre_racines.create_sprite(X0, Y0));
    // Tronc
    for(int i = 0 ; i < taille ; i++){
        int y_tronc = (Y0 - 8) - (i * 8);
        sprites.push_back(bn::sprite_items::osmag_arbre_tronc.create_sprite(X0, y_tronc));
    }
    // Tête
    int y_tete = 0;
    if(a_tete){
        y_tete = (Y0 - 12) - ((taille+1) * 8);
        sprites.push_back(bn::sprite_items::osmag_arbre_tete.create_sprite(X0, y_tete));
    }


    // Elevation de tous les sprites en priorité 2
    for(bn::sprite_ptr& spt : sprites){
        spt.set_bg_priority(2);
    }


    // Création de la boite de collision
    int x_gauche = X0 - 16/2;

    int y_haut;
    if(a_tete){
       y_haut = y_tete - 32/2;
    }
    else{
       y_haut = sprites.back().y().integer() - 8/2;
    }



    int largeur = 16;
    int hauteur = 8 + (taille * 8) + 32;
    boite_collision = bn::top_left_rect(x_gauche+1, y_haut+1, largeur-1, hauteur);
}

const bn::top_left_fixed_rect& Arbre::get_boite_collision() const{
    return boite_collision;
}

void Arbre::clear(){
    sprites.clear();
}

}
