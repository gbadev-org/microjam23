#ifndef OSMAG_HUD_H
#define OSMAG_HUD_H

#include "osmag_Physique.h"

namespace osmag{

class Hud{
private:
    bool debug = false;

    bn::sprite_text_generator plume;
    bn::sprite_text_generator plume_terre;
    bn::vector<bn::sprite_ptr, 48> txt_noms;
    bn::vector<bn::sprite_ptr, 32> txt_valeurs;

    void init_noms();
    bn::point pos_noms(int col, int ligne);
    bn::point pos_valeurs(int col, int ligne);
public:
    Hud();
    void init(const Physique& phys);
    void update(const Physique& phys);
    void clear();
};

}
#endif // HUD_H
