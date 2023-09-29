#include "tmg_test_game.h"

#include "bn_keypad.h"

#include "mj/mj_game_list.h"

#include "bn_regular_bg_items_tmg_press_a.h"
#include "bn_regular_bg_items_tmg_you_lose.h"
#include "bn_regular_bg_items_tmg_you_win.h"

MJ_GAME_LIST_ADD(tmg::test_game)

namespace tmg
{

test_game::test_game(int completed_games, const mj::game_data& data) :
    _bg(bn::regular_bg_items::tmg_press_a.create_bg((256 - 240) / 2, (256 - 160) / 2))
{
    constexpr int frames_diff = maximum_frames - minimum_frames;
    constexpr int maximum_speed_completed_games = 30;

    completed_games = bn::min(completed_games, maximum_speed_completed_games);

    int frames_reduction = (frames_diff * completed_games) / maximum_speed_completed_games;
    _total_frames = maximum_frames - frames_reduction;
    _total_frames -= data.random.get_int(60);
    _total_frames = bn::clamp(_total_frames, minimum_frames, maximum_frames);
}

void test_game::fade_in([[maybe_unused]] const mj::game_data& data)
{
}

mj::game_result test_game::play(const mj::game_data& data)
{
    mj::game_result result;
    result.exit = data.pending_frames == 0;

    if(! _victory && ! _defeat)
    {
        if(bn::keypad::a_pressed())
        {
            _bg.set_item(bn::regular_bg_items::tmg_you_win);
            result.remove_title = true;
            _victory = true;
        }
        else if(! bn::keypad::start_pressed() && bn::keypad::any_pressed())
        {
            _bg.set_item(bn::regular_bg_items::tmg_you_lose);
            result.remove_title = true;
            _defeat = true;
        }
    }
    else
    {
        if(_show_result_frames)
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

void test_game::fade_out([[maybe_unused]] const mj::game_data& data)
{
}

}
