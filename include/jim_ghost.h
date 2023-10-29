#ifndef JIM_GHOST_H
#define JIM_GHOST_H

#include "jim_entity.h"
#include "bn_sprite_animate_actions.h"
#include "bn_random.h"
#include "jim_player.h"

namespace jim
{
    class ghost:public Entity
    {
        private:
            bool enabled;
            jim::player* _player;
            bn::sprite_animate_action<8> _ghost_animation;

        public:
            int difficulty;
            int _path_timer;
            bn::fixed _speed;
            bn::random* _rand;
            bn::fixed vec_x, vec_y;
            ghost();
            ghost(int x, int y, jim::player*, int, bn::random*);
            void update_entity() override;
            void entity_collision(Entity_Type type) override;
            bn::fixed sqr(bn::fixed);
            bn::sprite_animate_action<8> get_animation();
            void set_animation(bn::sprite_animate_action<6>);
    };
}
#endif
