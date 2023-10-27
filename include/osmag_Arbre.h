#ifndef OSMAG_ARBRE_H
#define OSMAG_ARBRE_H

#include "osmag_butano.h"

namespace osmag{

class Arbre{
private:
    // Variables privées
    static constexpr const int TAILLE_MIN = 0;
    static constexpr const int TAILLE_MAX = 1;

    static constexpr const int X0 = -34;  // 0
    static constexpr const int Y0 = 58 - 40;

    bn::vector<bn::sprite_ptr, 16> sprites;
    int taille;
    bool a_tete;

    bn::top_left_fixed_rect boite_collision;

public:
    // Constructeur
    Arbre();

    // Méthodes publiques
    void init(bn::random& rand);
    const bn::top_left_fixed_rect& get_boite_collision() const;
    void clear();
};

}

#endif // OSMAG_ARBRE_H
