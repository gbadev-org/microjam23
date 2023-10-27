#ifndef OSMAG_OUTILS_H
#define OSMAG_OUTILS_H

#include "osmag_Cercle.h"

namespace osmag{


class Outils{
public:
    static bool sont_en_collision(const Cercle& cercle, const bn::top_left_rect& rectangle);
};

}
#endif // OUTILS_H
