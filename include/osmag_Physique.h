#ifndef OSMAG_PHYSIQUE_H
#define OSMAG_PHYSIQUE_H

#include "osmag_butano.h"

namespace osmag{

class Physique{
private:
    bn::fixed v0;       // vitesse initiale
    int angle;    // angle de tir
    bn::fixed_point pos0;       // pos initiale
    //bn::fixed t;        // temps écoulé
    bn::fixed g;  // accélération verticale de la gravité
    bn::fixed awx;      // accélération horizontale du vent (<0 vers gauche, >0 vers droite)


    bn::fixed x0_dec_min;
    bn::fixed x0_dec_max;

    void generer_position_iniale(bn::random &rand);
    void generer_vent(bn::random& rand);

public:
    static constexpr bn::fixed V0_MIN = 0;
    static constexpr bn::fixed V0_MAX = 300;
    static constexpr const bn::fixed DELTA_TEMPS = bn::fixed(0.0166666667);
    Physique();
    void init(bn::random& rand);

    // Accesseur
    bn::fixed get_v0() const;
    int get_angle() const;
    const bn::fixed_point& get_pos0() const;
    bn::fixed get_g() const;
    bn::fixed get_awx() const;

    // Modifieurs
    bool augmenter_angle();
    bool diminuer_angle();

    bool augmenter_v0();   

    void avancer_x0();
    void reculer_x0();

    // Outils
    bn::pair<bn::fixed_point, bn::fixed> calculer_position_et_orientation_projectile(const bn::fixed t) const;
    bn::fixed_point calculer_position_projectile(const bn::fixed t) const;
};



}
#endif // OSMAG_PHYSIQUE_H
