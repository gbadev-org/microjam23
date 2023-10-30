#ifndef OSMAG_CINEMATIQUEFIN_H
#define OSMAG_CINEMATIQUEFIN_H

#include "osmag_butano.h"

namespace osmag{

class Game;


class CinematiqueFin{
public:
    enum class Etat{
        CRASH_ZOOM,
        OS_BOUGE,
        OEIL_DECHIRE,
        PERDU_01,
        NB_ETATS
    };
private:
    // Variables de gestion
    int ecoulement = 0;
    Etat etat;
    bool cinematique_finit = false;
    Game * game;

    // Etats
    void changer_etat(Etat nom_etat);
    void etat_crash_zoom();
    void etat_os_bouge();
    void etat_oeil_dechire();
    void etat_perdu_01();

    // Utilitaires
    bn::optional<bn::rect_window_boundaries_hbe_ptr> light_window_hbe;
    bn::pair<bn::fixed, bn::fixed> window_boundaries[160];
    void fill_window_boundaries();

    // Variables communes
    bn::camera_ptr camera = bn::camera_ptr::create(0, 0);
    bn::vector<bn::sprite_ptr, 16> txt_sprites;

    // Variables Crash zoom
    bn::optional<bn::camera_move_to_action> cam_action;
    bn::optional<bn::sprite_scale_to_action> scale_action;
    bn::optional<bn::sprite_scale_to_action> scale_action2;
    bn::optional<bn::regular_bg_ptr> bandes;

    // Variables Os bouge
    bn::optional<bn::sprite_shear_to_action> oeil_shear_action;
    bn::optional<bn::sprite_palette_ptr> oeil_palette;
    bn::optional<bn::sprite_palette_fade_to_action> oeil_fade_action;

    // Variables Perdu 01
    // Gigoter ooeil
    bn::optional<bn::sprite_affine_mat_ptr> affine_mat;
    bn::optional<bn::sprite_affine_mat_scale_loop_action> action_oeil_gigote;


    bn::optional<bn::regular_bg_ptr> light_bg;



public:
    void init(Game* in_game, Etat etat_initial);
    bool update();
};

}

#endif // OSMAG_CINEMATIQUEFIN_H
