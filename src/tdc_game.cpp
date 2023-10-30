#include "tdc_game.h"

#include "bn_keypad.h"

#include "mj/mj_game_list.h"

#include "bn_regular_bg_items_tdc_dont_press.h"
#include "bn_regular_bg_items_tdc_temple_1.h"
#include "bn_regular_bg_items_tdc_frame.h"
#include "bn_regular_bg_items_tdc_you_lose.h"
#include "bn_sound_items.h"

namespace
{
    constexpr bn::string_view code_credits[] = { "quadiez" };
    constexpr bn::string_view graphics_credits[] = { "quadiez" };
}

// MJ_GAME_LIST_ADD(tdc::tdc_game)
MJ_GAME_LIST_ADD_CODE_CREDITS(code_credits)
MJ_GAME_LIST_ADD_GRAPHICS_CREDITS(graphics_credits)
// MJ_GAME_LIST_ADD_MUSIC_CREDITS(music_credits)
// MJ_GAME_LIST_ADD_SFX_CREDITS(sfx_credits)

namespace tdc
{

tdc_game::tdc_game(int completed_games, const mj::game_data& data) :
    _bg(bn::regular_bg_items::tdc_temple_1.create_bg(-100, 0)),
    _bg2(bn::regular_bg_items::tdc_dont_press.create_bg(0, 20)),
    _frame(bn::regular_bg_items::tdc_frame.create_bg(0, 0)),
    _tempo(recommended_music_tempo(completed_games, data)),
    _total_frames(play_jingle(mj::game_jingle_type::METRONOME_12BEAT, completed_games, data)),
    _show_result_frames((210 / _tempo).right_shift_integer())
{
} 

void tdc_game::fade_in([[maybe_unused]] const mj::game_data& data)
{
}

mj::game_result tdc_game::play(const mj::game_data& data)
{
    mj::game_result result;
    result.exit = data.pending_frames == 0;

    if(_victory)
    {
        if(bn::keypad::any_pressed())
        {
            _bg2.set_item(bn::regular_bg_items::tdc_you_lose);
            // bn::sound_items::tdc_lose.play_with_priority(10);
            bn::sound_items::ghost_capture_moan_5.play(1, _tempo, 0);
            result.remove_title = true;
            _victory = false;
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

void tdc_game::fade_out([[maybe_unused]] const mj::game_data& data)
{
}

}
