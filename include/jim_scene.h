#ifndef JIM_SCENE_H
#define JIM_SCENE_H

#include "bn_vector.h"
#include "jim_entity.h"
#include "bn_camera_ptr.h"
#include "bn_random.h"

namespace jim
{
    class Scene
    {
        private:
            bn::vector<jim::Entity*, 256> scene_entities;
            bn::camera_ptr camera;
            bn::fixed_rect* draw_zone;
            
            int _difficulty;
        public:
            int start_pos_x;
            int start_pos_y;
            bool _end_scene;
            int collision_timer;
            bn::random* random;

            Scene(int x, int y, int d, bn::random*);
            int get_difficulty();
            virtual ~Scene();
            virtual void init_scene();
            virtual void update();
            virtual void addEntity(jim::Entity* entity);
            virtual void removeEntity(int id);
            bn::camera_ptr get_camera();
            bn::vector<Entity*, 256> get_scene_entities();
            void update_draw_zone();
            void check_entities_drawable();
            void check_entity_collisions();

    };
}

#endif
