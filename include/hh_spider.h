#ifndef HH_SPIDER_H
#define HH_SPIDER_H

#include "hh_entity.h"
#include "bn_sprite_ptr.h"
#include "bn_sprite_animate_actions.h"


namespace hh {
class spider : public entity {
public: 
    spider(bn::fixed x, bn::fixed y, bn::fixed tempo);
    virtual void update(const mj::game_data& data);
    virtual void lights_on(bn::random &rand);
    void point_at(bn::fixed_point pos);
    virtual void disable_movement();

private:
    bn::sprite_ptr _sprite;
    bn::sprite_animate_action<2> _anim;
};

}

#endif
