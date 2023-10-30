#ifndef JIM_FOREST_SCENE_H
#define JIM_FOREST_SCENE_H

#include "bn_regular_bg_ptr.h"
#include "bn_affine_bg_ptr.h"

#include "jim_player.h"
#include "jim_scene.h"

namespace jim
{
    class forest_scene:public Scene
    {
        public:
            jim::player* _player;
            bn::regular_bg_ptr _bg;
            bn::affine_bg_ptr _fore;
            bn::vector<bn::sprite_ptr, 3> _hearts;
            forest_scene(int, bn::random*);
            void init_scene() override;
            void update() override;
    };
}
#endif
