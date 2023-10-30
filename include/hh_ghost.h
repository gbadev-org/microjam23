#ifndef HH_GHOST_H
#define HH_GHOST_H

#include "hh_entity.h"
#include "bn_sprite_ptr.h"
#include "bn_sprite_actions.h"
#include "bn_sprite_animate_actions.h"


namespace hh {
class ghost : public entity {
public: 
    ghost(bn::fixed x, bn::fixed y, uint8_t initial_direction, bn::fixed tempo, bool pacman_ghost);
    virtual void update(const mj::game_data& data);
    virtual void lights_on(bn::random &rand);
    virtual void disable_movement();

private:
    bn::sprite_ptr _sprite;
    bn::optional<bn::sprite_horizontal_flip_toggle_action> _flip_anim;
    bn::optional<bn::sprite_animate_action<2>> _anim;
};

}

#endif
