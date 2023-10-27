#ifndef OSMAG_OEIL_H
#define OSMAG_OEIL_H

#include "osmag_butano.h"

namespace osmag{

class Oeil
{

private:
    static constexpr const int X_MIN =  -5;//-120 + 129; //-120 + 132; -120 + 140;    -120 + 145;  -120 + 154;
    static constexpr const int X_MAX =   120-32;
    static constexpr const int Y_MIN =    -80 +  25;     // 30  39 55
    static constexpr const int Y_MAX =     80 - 16;    // 80 -  28 80 - 32



    bn::optional<bn::sprite_move_loop_action> mouvement_action;

    bn::top_left_fixed_rect boite_collision;
public:
    bn::optional<bn::sprite_animate_action<4>> animation_action;
    bn::optional<bn::sprite_ptr> sprite;
    Oeil();
    void init(bn::random& rand);
    void update();
    const bn::top_left_fixed_rect& get_boite_collision() const;
    // DEBUG
    void log();
    //
    void changer_animation_mort();
};

}

#endif // OSMAG_OEIL_H
