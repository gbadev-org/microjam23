#ifndef OSMAG_PLATEFORME_H
#define OSMAG_PLATEFORME_H

#include "osmag_Ogive.h"

namespace osmag{

class Plateforme{
private:

    bn::top_left_fixed_rect boite_collision;
    bn::top_left_fixed_rect boite_collision_droite;

public:
    bn::optional<bn::regular_bg_ptr> bg;
    Plateforme();
    void init();    
    const bn::top_left_fixed_rect& get_boite_collision() const;
    bool en_collision_avec_ogive(Ogive& ogive);

};
}

#endif // OSMAG_PLATEFORME_H
