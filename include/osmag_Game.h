#ifndef OSMAG_GAME_H
#define OSMAG_GAME_H

#include "osmag_Pluie.h"
#include "osmag_Hero.h"
#include "osmag_Ogive.h"
#include "osmag_Arbre.h"
#include "osmag_Oeil.h"
#include "osmag_Hud.h"
#include "osmag_Plateforme.h"
#include "osmag_Physique.h"
#include "osmag_Courbe.h"
#include "osmag_CinematiqueFin.h"

#include "mj/mj_game.h"

namespace osmag{

class Game : public mj::game{

public:
    Game(int completed_games, const mj::game_data& data);

    [[nodiscard]] bn::string<16> title() const final{
        return "Os Magnum";
    }

    [[nodiscard]] int total_frames() const final{
        return _total_frames;
    }

    void fade_in(const mj::game_data& data) final;

    [[nodiscard]] mj::game_result play(const mj::game_data& data) final;

    [[nodiscard]] bool victory() const final{
        return _victory;
    }

    void fade_out(const mj::game_data& data) final;




public:
    bn::optional<bn::regular_bg_ptr> bg_ciel;
    Plateforme plateforme;
    Oeil oeil;
    Hero hero;
    Ogive ogive;
    Hud hud;
    Arbre arbre;
    Pluie pluie;
    void effect();

private:
    Physique phys;

    bn::vector<bn::sprite_ptr, 16> txt_sprites;

    //
    //const mj::game_data& mj_game_data;
    //bn::fixed acceleration_vent;

    // Fonds





    // Elements dynamiques
    // Oeil





    // Arbre




    // Courbe de points
    Courbe courbe;

    CinematiqueFin cinematique;

    // Méthodes privées
    bn::array<bn::fixed, bn::display::height()> horizontal_deltas;
    bn::optional<bn::regular_bg_position_hbe_ptr> horizontal_deltas_hbe;
    bn::fixed base_degrees_angle;





    int _total_frames;
    int _show_result_frames = 260;
    bool _victory = false;
    bool _defeat = false;
};

}

#endif
