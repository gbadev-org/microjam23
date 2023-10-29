#ifndef JIM_PLAYER_H
#define JIM_PLAYER_H

#include "jim_entity.h"
#include "bn_sprite_palette_ptr.h"
#include "bn_sprite_animate_actions.h"

namespace jim
{
    class player:public Entity
    {
        private:
            bool _dead;
            bool _is_invulnerable;
            int _invuln_time;
            int _health;
            
            bn::sprite_animate_action<6> _player_animation;
            bn::affine_bg_map_item collision_map;
            bn::sprite_palette_ptr base_palette;


        public:
            player(bn::affine_bg_map_item map);
            player(int _x, int _y, bn::affine_bg_map_item map);
            void update_entity() override;
            void entity_collision(Entity_Type type) override;
            bool is_dead();
            int get_health();
            void set_health(int);
            void do_contact_damage();
            bn::sprite_palette_ptr get_base_palette();
    };
}
#endif
