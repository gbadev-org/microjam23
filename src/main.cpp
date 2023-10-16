#include "bn_core.h"

#include "mj/mj_build_config.h"
#include "mj/mj_core.h"
#include "mj/mj_credits_scene.h"
#include "mj/mj_game_scene.h"
#include "mj/mj_intro_scene.h"
#include "mj/mj_scene.h"
#include "mj/mj_scene_type.h"
#include "mj/mj_title_scene.h"

int main()
{
    bn::core::init();

    bn::unique_ptr<mj::core> core_ptr(new mj::core());
    mj::core& core = *core_ptr;

    bn::unique_ptr<mj::scene> scene;

    #if MJ_SKIP_INITIAL_TITLE
        bn::optional<mj::scene_type> next_scene = mj::scene_type::GAME;
    #else
        bn::optional<mj::scene_type> next_scene = mj::scene_type::INTRO;
    #endif

    while(true)
    {
        mj::scene* scene_ptr = scene.get();

        if(scene_ptr)
        {
            next_scene = scene_ptr->update();
        }

        if(const mj::scene_type* next_scene_ptr = next_scene.get())
        {
            if(scene_ptr)
            {
                scene.reset();
            }
            else
            {
                switch(*next_scene_ptr)
                {

                case mj::scene_type::INTRO:
                    scene.reset(new mj::intro_scene(core));
                    break;

                case mj::scene_type::TITLE:
                    scene.reset(new mj::title_scene(core));
                    break;

                case mj::scene_type::GAME:
                    scene.reset(new mj::game_scene(core));
                    break;

                case mj::scene_type::CREDITS:
                    scene.reset(new mj::credits_scene(core));
                    break;

                default:
                    break;
                }
            }
        }

        core.update();
    }
}
