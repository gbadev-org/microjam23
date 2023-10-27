#ifndef OSMAG_CERCLE_H
#define OSMAG_CERCLE_H

#include "osmag_butano.h"

namespace osmag{

struct Cercle{
    bn::fixed x;
    bn::fixed y;
    bn::fixed rayon;

    bool est_en_collision(const bn::top_left_rect& r) {
        // Trouver le point le plus proche sur le rectangle au centre du cercle
        int x_n = bn::max( r.x(), bn::min(x.integer(), (r.x() + r.width()) ) );
        int y_n = bn::max(r.y(), bn::min(y.integer(), r.y() + r.height()));


        // Calculer la distance entre ce point et le centre du cercle
        bn::fixed dist_x = x_n - x;
        bn::fixed dist_y = y_n - y;
        bn::fixed distance = bn::sqrt((dist_x * dist_x) + (dist_y * dist_y));

        // Vérifier si la distance est inférieure ou égale au rayon du cercle
        return distance <= rayon;
    }

    bool est_en_collision_fixed(const bn::top_left_fixed_rect& r) {
        // Trouver le point le plus proche sur le rectangle au centre du cercle
        bn::fixed x_n = bn::max( r.x(), bn::min(x, (r.x() + r.width()) ) );
        bn::fixed y_n = bn::max(r.y(), bn::min(y, r.y() + r.height()));


        // Calculer la distance entre ce point et le centre du cercle
        bn::fixed dist_x = x_n - x;
        bn::fixed dist_y = y_n - y;
        bn::fixed distance = bn::sqrt((dist_x * dist_x) + (dist_y * dist_y));

        // Vérifier si la distance est inférieure ou égale au rayon du cercle
        return distance <= rayon;
    }
};

}

#endif // OSMAG_CERCLE_H
