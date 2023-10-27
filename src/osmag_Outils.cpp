#include "osmag_Outils.h"

bool osmag::Outils::sont_en_collision(const Cercle &cercle, const bn::top_left_rect &rectangle){
    // --- Vérification rapide : si le centre du cercle est dans le rectangle, il y a collision.
    if(rectangle.contains(bn::point(cercle.x.floor_integer(), cercle.y.floor_integer()))){
        return true;
    }

    // --- Sinon, il faut un calcul plus poussé.

    // Trouver le point le plus proche sur le rectangle au centre du cercle
    int x_plus_proche = bn::max(rectangle.x(), bn::min(cercle.x.floor_integer(), (cercle.x.floor_integer() + rectangle.width()) ) );
    int y_plus_proche = bn::max(rectangle.y(), bn::min(cercle.y.floor_integer(), rectangle.y() + rectangle.height()));

    // Calculer la différence en x et y entre le point le plus proche et le centre du cercle
    bn::fixed dist_x = x_plus_proche - cercle.x;
    bn::fixed dist_y = y_plus_proche - cercle.y;

    // Calculer le carré de la distance entre le point le plus proche et le centre du cercle
    bn::fixed distance_carree = (dist_x * dist_x) + (dist_y * dist_y);

    // Vérifier si la distance au carré est inférieure ou égale au carré du rayon du cercle
    return distance_carree <= cercle.rayon * cercle.rayon;
}
