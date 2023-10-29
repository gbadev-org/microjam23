#ifndef RNTBA_HAND_H
#define RNTBA_HAND_H

#include "bn_sprite_ptr.h"

namespace rntba {

class hand
{
public:
    hand();

    [[nodiscard]] int process(); // 0 if nothing, 1 if the hand flicked

    [[nodiscard]] int hit_top() const{
        return _hand.y().floor_integer() - 32;
    }

    /*[[nodiscard]] int hit_bot() const{
        return _hand.x().floor_integer() + 4;
    }*/

    [[nodiscard]] int hit_len() const{
        return 44;
    }

    [[nodiscard]] int mode() const
    {
        return _mode;
    }

    void thumbsup();

private:
    int process_idle();
    int process_flick(); // Post-flick, stationary for a bit
    int process_retract(); // Retracting after flick
    const int _start_x = 120 - 32; // Move to right side of screen, offset from sprite center (I do not like how BN uses sprite center as base for coords >:(
    const int _start_y = 32; // Already at center, so offset it down to make up for the ~32px of dead space in the hand sprite
    const int _arm_x = 53; // Roughly the xoffset to match the arm up with the hand
    const int _arm_y = 11; // Roughly the yoffset to match the arm up with the hand
    const int _y_min = -48; // In BN coords, the highest the hand is allowed to go (aaaalll the way up to that roof, baby)
    const int _y_max = 80; // In BN coords, the lowest the hand is allowed to go (aaallll the way down to that floor, baby)
    const int _flick_x = 16; // Hand jumps to this position when it flicks
    const int _flick_wait = 30; // How long the hand remains stationary after flicking, before beginning to retract
    const int _step_x = 3; // How much the hand moves each frame as it retracts back
    bn::sprite_ptr _hand;
    bn::sprite_ptr _arm;
    bn::sprite_ptr _elbow;
    int _mode = 0; // 0: idle , 1: flick/pause , 2: retracting , 3: disable , 4:Flick
    int _timer;
    bool _retracting = false;
    bool _stop = false;
};

}

#endif
