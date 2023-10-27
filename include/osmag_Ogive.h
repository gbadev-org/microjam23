#ifndef OSMAG_OGIVE_H
#define OSMAG_OGIVE_H

#include "osmag_Physique.h"

namespace osmag{
class Ogive{

public:
   bn::sprite_ptr sprite;
private:
    enum class Etat{
        REPOS,
        VOL
    };
    Etat etat = Etat::REPOS;

    bn::top_left_fixed_rect boite_collision;



    Physique* phys;
    bn::fixed t;


public:
    Ogive();
    void init(Physique* _phys);
    void update();
    void lancer();
    const bn::top_left_fixed_rect& get_boite_collision() const;


};


}
#endif // OGIVE_H
