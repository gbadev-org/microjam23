#include "jim_survival.h"
#include "mj/mj_game_list.h"

namespace
{
    constexpr bn::string_view code_credits[] = { "Jimmi_Jam" };
    constexpr bn::string_view graphics_credits[] = {"KDs_Arts", "Gustavo Vituri", "Jimmi_Jam"};
}

// MJ_GAME_LIST_ADD(jim::survival)
MJ_GAME_LIST_ADD_CODE_CREDITS(code_credits)
MJ_GAME_LIST_ADD_GRAPHICS_CREDITS(graphics_credits)

namespace jim
{
    survival::survival(int completed_games, const mj::game_data &data):scene(int(recommended_difficulty_level(completed_games, data)) + 1, &data.random),_total_frames(play_jingle(mj::game_jingle_type::TOTSNUK10, completed_games, data))
    {
        scene.init_scene();
        
    }

    void survival::fade_in([[maybe_unused]] const mj::game_data &data)
    {
        
    }

    mj::game_result survival::play(const mj::game_data &data)
    {
        
        mj::game_result result;
        result.exit = data.pending_frames == 0;
        _victory = data.pending_frames == 1;
        _defeat = scene._end_scene == true || scene._end_scene;
        if(!_victory && ! _defeat)
        {
           scene.update();
        }
        else
        {
            if (_show_result_frames)
            {
                --_show_result_frames;
            }
            else
            {
                result.exit = true;
            }
        }

        return result;
    }

    void survival::fade_out([[maybe_unused]] const mj::game_data &data)
    {
    }
}
