#include "osmag_CinematiqueFin.h"
#include "osmag_Game.h"

#include "bn_regular_bg_items_osmag_bandes.h"
#include "bn_regular_bg_items_osmag_coffin_light.h"

namespace osmag{

void CinematiqueFin::init(Game* in_game, Etat etat_initial){
    game = in_game;
    etat = etat_initial;
}

bool CinematiqueFin::update(){
    switch(etat){
        case Etat::CRASH_ZOOM: etat_crash_zoom(); break;
        case Etat::OS_BOUGE: etat_os_bouge(); break;
        case Etat::OEIL_DECHIRE: etat_oeil_dechire(); break;
        case Etat::PERDU_01: etat_perdu_01(); break;
        case Etat::NB_ETATS: break;
        default: break;
    }
    game->effect();
    ecoulement += 1;
    return cinematique_finit;
}


void CinematiqueFin::changer_etat(Etat nom_etat){
    etat = nom_etat;
    ecoulement = -1;
}

void CinematiqueFin::etat_crash_zoom(){
    if(ecoulement == 0){
        game->ogive.sprite.set_bg_priority(3);

        game->plateforme.bg.reset();
        game->hud.clear();
        game->arbre.clear();
        game->hero.clear();
        game->pluie.clear();


        game->oeil.sprite->set_camera(camera);
        game->ogive.sprite.set_camera(camera);
        game->bg_ciel->set_camera(camera);

        oeil_palette = game->oeil.sprite->palette();
        oeil_palette->set_fade_color(bn::colors::red);


        bandes = bn::regular_bg_items::osmag_bandes.create_bg(0, 0);
        bandes->set_priority(1);
        bandes->put_above();

        int duree = 20;
        cam_action = bn::camera_move_to_action(camera, duree, game->oeil.sprite->position());
        scale_action = bn::sprite_scale_to_action(game->oeil.sprite.value(), duree, 4.0);
        scale_action2 = bn::sprite_scale_to_action(game->ogive.sprite, duree, 4.0);
    }
    if(!cam_action->done()){
        cam_action->update();
        scale_action->update();
        scale_action2->update();
        game->ogive.update();
        game->oeil.update();
        //oeil_fade_action->update();
    }
    else{
        changer_etat(Etat::OS_BOUGE);

    }
}

void CinematiqueFin::etat_os_bouge(){
    if(ecoulement == 0){
        //game->ogive.update();
        oeil_fade_action =  bn::sprite_palette_fade_to_action(oeil_palette.value(), 42, 0.80);
        //oeil_shear_action = bn::sprite_shear_to_action(game->oeil.sprite.value(), 60, 10);
        //oeil_fade_action->update();

    }
    if(!oeil_fade_action->done()){
        //game->ogive.update();
        game->oeil.update();
        if(ecoulement % 4 == 0){
            game->ogive.update();
        }

        oeil_fade_action->update();
    }
    else{
        changer_etat(Etat::OEIL_DECHIRE);
    }

}

void CinematiqueFin::etat_oeil_dechire(){
    if(ecoulement == 0){
        oeil_shear_action = bn::sprite_shear_to_action(game->oeil.sprite.value(), 90, 6);
    }
    if(!oeil_shear_action->done()){
        oeil_shear_action->update();
        game->ogive.update();
    }
    else{
        //oeil_shear_action.reset();
        //game->oeil.sprite.reset();
        game->oeil.sprite->set_visible(false);
        cinematique_finit = true;
    }

}

void CinematiqueFin::etat_perdu_01(){
    if(ecoulement == 0){
        // Gigotement oeil
        affine_mat = bn::sprite_affine_mat_ptr::create();
        game->oeil.sprite->set_affine_mat(affine_mat.value());
        affine_mat->set_scale(0.95);
        action_oeil_gigote = bn::sprite_affine_mat_scale_loop_action(affine_mat.value(), 30, 1.65);




        light_bg = bn::regular_bg_items::osmag_coffin_light.create_bg(
            game->oeil.sprite->x(),
            game->oeil.sprite->y()
        );
        light_bg->set_priority(1);
        //light_bg->put_above();
        game->oeil.sprite->set_bg_priority(1);
        game->oeil.sprite->put_above();
        game->ogive.sprite.set_bg_priority(2);
        game->hero.sprite_perso->set_bg_priority(2);

        light_bg->set_blending_enabled(true);
        bn::window::outside().set_show_bg(light_bg.value(), false);
        bn::rect_window internal_window = bn::rect_window::internal();
        internal_window.set_top(-80);
        internal_window.set_bottom(80);
        light_window_hbe = bn::rect_window_boundaries_hbe_ptr::create_horizontal(internal_window, window_boundaries);
        //fill_window_boundaries();
    }

        game->oeil.animation_action->update();
        game->pluie.update();
        action_oeil_gigote->update();
        fill_window_boundaries();


}

void CinematiqueFin::fill_window_boundaries(){
    constexpr int scanlines = 160;
    int centerY = game->oeil.sprite->y().ceil_integer();  // Centre de l'écran
    int centerX = game->oeil.sprite->x().ceil_integer();

    bn::fixed amplitude = (bn::fixed(ecoulement) / 60);
    bn::fixed max_radius = 30 * amplitude;

    for(int y = 0; y < scanlines; ++y) {
        bn::fixed dy = y - centerY - 80;
        if(bn::abs(dy) <= max_radius) {
            bn::fixed radius = bn::sqrt(max_radius * max_radius - dy * dy);

            window_boundaries[y].first = centerX - radius;
            window_boundaries[y].second = centerX + radius;
        } else {
            window_boundaries[y].first = 0;
            window_boundaries[y].second = 0;
        }
    }

    light_window_hbe->reload_deltas_ref();

}



}
