#ifndef PLUIE_H
#define PLUIE_H

#include "osmag_butano.h"

namespace osmag{

class Pluie{
private:
    bn::fixed acceleration_vent;
    bn::vector<bn::sprite_ptr, 18> vecteur_araignees;
    bn::random rand;
public:
    Pluie();
    void init(bn::fixed _acceleration_vent,  bn::random& _rand);
    void larguer_araignee();
    void update();
    bn::fixed_point calculer_position_initiale_araignee(bool est_update);
    void clear();
};

#endif // PLUIE_H


}
