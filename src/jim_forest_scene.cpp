#include "jim_forest_scene.h"

#include "bn_regular_bg_items_jim_map_back.h"
#include "bn_affine_bg_items_jim_map_fore.h"
#include "bn_sprite_items_jim_heart.h"

#include "jim_ghost.h"

namespace jim
{
    forest_scene::forest_scene(int difficulty, bn::random* rand):Scene(256, 256, difficulty, rand),_bg(bn::regular_bg_items::jim_map_back.create_bg(256,256)), _fore(bn::affine_bg_items::jim_map_fore.create_bg(256,256))
    {
        
    }

    void forest_scene::init_scene()
    {
        get_camera().set_position(start_pos_x, start_pos_y);
        
        _player = new player(start_pos_x, start_pos_y, bn::affine_bg_items::jim_map_fore.map_item());

        _bg.set_camera(get_camera());
        _fore.set_camera(get_camera());
        
        _bg.set_priority(2);
        _bg.set_z_order(32767);
        _fore.set_priority(1);
        _fore.set_z_order(-32765);

        addEntity(_player);
        int ghost_count = get_difficulty() * 3 + random->get_int(1, 3);

        for(int i = 0; i < ghost_count; i++)
        {
            //try to keep random position outside of player. So easy here...
            int pos_x = random->get_fixed(16, 498).integer();
            int pos_y = random->get_fixed(16, 498).integer();
            addEntity(new jim::ghost(pos_x, pos_y, _player, get_difficulty(), random));
        }

        for(auto e : get_scene_entities())
        {
            e->get_sprite().set_camera(get_camera());
            e->get_sprite().set_bg_priority(2);
            e->get_sprite().set_z_order(-32766);
        }

        
        for(int i = 0; i < _player->get_health(); i++)
        {
            _hearts.push_back(bn::sprite_items::jim_heart.create_sprite(104 - (i * 20), -64));
            _hearts[i].set_bg_priority(1);
            _hearts[i].set_z_order(-32766);
        }

    }
    
    void forest_scene::update()
    {
        //THERE HAS TO BE A BETTER WAY BUT IM OUT OF TIME>>>>>>>>>>>
        for(auto e : _hearts)
        {
            e.set_visible(false);
        }
        for(int i = 0; i < _player->get_health(); i++)
        {
            _hearts[i].set_visible(true);
        }

        if(_player->is_dead())
        {
            _end_scene = true;
        }

        collision_timer++;

        if (collision_timer > 4)
        {
            check_entity_collisions();
            collision_timer = 0;
        }
        for (Entity* e : get_scene_entities())
        {  
            e->update_entity();
        }

        bn::fixed_point position = _player->get_sprite().position();

        if(position.y() < 88)
        {
            position.set_y(88);
        }
        else if(position.y() > 432)
        {
            position.set_y(432);
        }

        get_camera().set_position(position);

        update_draw_zone();
        

    }

}
