#ifndef OSMAG_COURBE_H
#define OSMAG_COURBE_H

#include "osmag_Physique.h"

namespace osmag{

class Courbe{
public:
  static constexpr const int NB_POINTS = 32;
  static constexpr const int ECART_TEMPS = 8;

private:
    bn::vector<bn::sprite_ptr, NB_POINTS> sprites;
    Physique* phys;    

public:    
    Courbe();
    void init(Physique* _phys);
    void update();
    void clear();
    void clear_partiel(int index_depart);


};

}

#endif // OSMAG_COURBE_H
