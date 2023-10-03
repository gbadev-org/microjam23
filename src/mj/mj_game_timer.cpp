#include "mj/mj_game_timer.h"

#include "bn_string.h"

#include "mj/mj_core.h"

namespace mj
{

void game_timer::update(int pending_frames, core& core)
{
    _sprites.clear();

    if(pending_frames)
    {
        bn::sprite_text_generator& text_generator = core.text_generator();
        text_generator.set_bg_priority(1);
        text_generator.set_z_order(-32767);

        int time = pending_frames / 6;
        bn::string<16> time_text = bn::to_string<16>(time / 10) + '.' + bn::to_string<16>(time % 10);
        text_generator.generate(10 - 120, 80 - 16, "Time: " + time_text, _sprites);

        text_generator.set_bg_priority(3);
        text_generator.set_z_order(0);
    }
}

}
