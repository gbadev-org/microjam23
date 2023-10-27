#include "osmag_Game.h"

#include "mj/mj_game_list.h"


#include "bn_regular_bg_items_osmag_ciel_violet.h"
#include "bn_music_items.h"

namespace{
    constexpr bn::string_view code_credits[] = {"Fralacticus"};
    constexpr bn::string_view graphics_credits[] = {"Fralacticus", "Night Blade", "Ten by Twenty", "GValiente"};
    constexpr bn::string_view music_credits[] = {"Fralacticus", "Isocel"};
}


// MJ_GAME_LIST_ADD(osmag::Game)
MJ_GAME_LIST_ADD_CODE_CREDITS(code_credits)
MJ_GAME_LIST_ADD_GRAPHICS_CREDITS(graphics_credits)
MJ_GAME_LIST_ADD_MUSIC_CREDITS(music_credits)


namespace osmag{
	
	


Game::Game([[maybe_unused]] int completed_games, const mj::game_data& data){
    // Musique
    //bn::music_items::osmag_ectoplasmus.play();
    bn::music_items::osmag_isocel_halloween.play();

    data.random.update(); // DEBUG
    // Initialisation du ciel
    bg_ciel = bn::regular_bg_items::osmag_ciel_violet.create_bg(0, 0);

    // Initialisation de la plateforme
    plateforme.init();

    // Initialisation de la physique
    phys.init(data.random);

    // Initialisation de la pluie
    pluie.init(phys.get_awx(), data.random);

    // Initialisation de l'arbre
    arbre.init(data.random);    

    // Initialisation de l'oeil
    oeil.init(data.random);

    // Initialisation de l'ogive
    ogive.init(&phys);

    // Initialisation du héro fantome
    hero.init(&phys, &ogive);

    // Initialisation du HUD
    hud.init(phys);

    // Initialisation de la courbe de points
    courbe.init(&phys);


    _total_frames = maximum_frames;

    horizontal_deltas_hbe = bn::regular_bg_position_hbe_ptr::create_horizontal(bg_ciel.value(), horizontal_deltas);
    //SON CODE

//    constexpr int frames_diff = maximum_frames - minimum_frames;
//    constexpr int maximum_speed_completed_games = 30;

//    completed_games = bn::min(completed_games, maximum_speed_completed_games);

//    int frames_reduction = (frames_diff * completed_games) / maximum_speed_completed_games;
//    _total_frames = maximum_frames - frames_reduction;
//    _total_frames -= data.random.get_int(60);
//    _total_frames = bn::clamp(_total_frames, minimum_frames, maximum_frames);


}

void Game::fade_in([[maybe_unused]] const mj::game_data& data)
{
}

mj::game_result Game::play(const mj::game_data& data){
    //  SON CODE
    mj::game_result result;
    result.exit = data.pending_frames == 0;
    //

    if(!_victory && !_defeat){
//        if(bn::keypad::select_pressed()){
//            mj::game_result result;
//            result.exit = true;
//            return result;
//        }
        //effect();

        pluie.update();
        hero.update();
        ogive.update();
        oeil.update();

        if(hero.get_tir_est_lance()){
            courbe.clear();

            // Cas victoire : Touche oeil
            if(ogive.get_boite_collision().intersects(oeil.get_boite_collision())){
                _victory = true;

                oeil.changer_animation_mort();
                cinematique.init(this, CinematiqueFin::Etat::CRASH_ZOOM);
                data.big_text_generator.generate(-113, -49, "You win !", txt_sprites);

            }

            // Cas defaite : Touche Decor || Tombe trop bas
            else if(
                plateforme.en_collision_avec_ogive(ogive) ||
                ogive.get_boite_collision().intersects(arbre.get_boite_collision()) ||
                ogive.get_boite_collision().y() > 80
            ){
                ogive.update();
                _defeat = true;
                cinematique.init(this, CinematiqueFin::Etat::PERDU_01);
                data.big_text_generator.generate(-113, -49, "You lose !", txt_sprites);
            }
        }
        else{
            hud.update(phys);
            courbe.update();
        }
    }
    else{
        if(_show_result_frames){
            if(_victory){
                if(cinematique.update() == true){
                    result.exit = true;
                }
            }
            else{
                cinematique.update();
            }
            --_show_result_frames;
        }
        else{
            result.exit = true;
        }
    }

    return result;
}

void Game::fade_out([[maybe_unused]] const mj::game_data& data){

}




void Game::effect(){

    base_degrees_angle += 4;

    if(base_degrees_angle >= 360)
    {
    base_degrees_angle -= 360;
    }

    bn::fixed degrees_angle = base_degrees_angle;

    for(int index = 0, limit = bn::display::height() / 2; index < limit; ++index){
        degrees_angle += 16;

        if(degrees_angle >= 360)
        {
            degrees_angle -= 360;
        }

        bn::fixed desp = bn::degrees_lut_sin(degrees_angle) * 8;
        horizontal_deltas[(bn::display::height() / 2) + index] = desp;
        horizontal_deltas[(bn::display::height() / 2) - index - 1] = desp;
    }

    horizontal_deltas_hbe->reload_deltas_ref();



}


}
