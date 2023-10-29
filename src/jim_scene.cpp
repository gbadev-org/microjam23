#include "jim_scene.h"

namespace jim
{

    Scene::Scene(int x, int y, int d, bn::random* rand) : camera(bn::camera_ptr::create(x, y))
    {
        collision_timer = 0;
        start_pos_x = x;
        start_pos_y = y;
        _end_scene = false;
        draw_zone = new bn::fixed_rect(camera.x(), camera.y(), 240, 160);
        _difficulty = d;
        random = rand;
    }
    void Scene::update()
    {
    }

    int Scene::get_difficulty()
    {
        return _difficulty;
    }

    void Scene::init_scene()
    {
    }
    void Scene::check_entities_drawable()
    {
        for(auto e : scene_entities)
        {
            if(e->get_type() == PLAYER)
                continue;
            e->set_drawable(draw_zone->intersects(e->get_hitbox()));
        }
    }
    void Scene::check_entity_collisions()
    {
        bn::fixed_rect lft;
        bn::fixed_rect rgt;
        // change this to only check entities in camera view. add another rectangle.
        for (int i = 0; i < scene_entities.size(); i++)
        {
            if(!scene_entities[i]->is_drawable())
                continue;
            lft = scene_entities[i]->get_hitbox();
            for (int j = 0; j < scene_entities.size(); j++)
            {
                if(!scene_entities[j]->is_drawable()) //skip entities not on screen
                    continue;
                // needs to do this to not collide with itself
                if (j != i)
                {
                    rgt = scene_entities[j]->get_hitbox();
                    if (lft.intersects(rgt))
                    {
                        scene_entities[i]->entity_collision(scene_entities[j]->get_type());
                    }
                }
            }
        }
    }

    void Scene::addEntity(jim::Entity *entity)
    {
        scene_entities.push_back(entity);
    }

    void Scene::removeEntity(int id)
    {
        for (jim::Entity *e : scene_entities)
        {
            if (e->get_id() == id)
            {
                scene_entities.erase(&e);
            }
        }
    }
    bn::vector<Entity *, 256> Scene::get_scene_entities()
    {
        return scene_entities;
    }

    bn::camera_ptr Scene::get_camera()
    {
        return camera;
    }
    
    void Scene::update_draw_zone()
    {
        draw_zone->set_position(camera.position());
        check_entities_drawable();
    }

    Scene::~Scene()
    {
        for (Entity *e : scene_entities)
        {
            delete e;
        }
        delete draw_zone;
        scene_entities.clear();
        //delete random occurs before end of scene
    }
}
