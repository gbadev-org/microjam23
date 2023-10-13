#include "mj/mj_core.h"

#include "bn_core.h"
#include "bn_keypad.h"
#include "bn_timers.h"
#include "bn_version.h"

#include "mj/mj_big_sprite_font.h"
#include "mj/mj_build_config.h"
#include "mj/mj_small_sprite_font.h"
#include "mj/mj_sprite_font.h"

namespace mj
{

namespace
{
    constexpr int version(int version_major, int version_minor, int patch)
    {
        return (version_major << 16) + (version_minor << 8) + patch;
    }

    static_assert(version(BN_VERSION_MAJOR, BN_VERSION_MINOR, BN_VERSION_PATCH) >= version(15, 8, 2),
                  "Butano version is too old");
}

core::core() :
    _text_generator(sprite_font),
    _small_text_generator(small_sprite_font),
    _big_text_generator(big_sprite_font)
{
    _sram_data.init();
}

void core::update()
{
    _random.update();

    bn::core::update();

    #if MJ_VBLANK_ASSERT_ENABLED
        bn::fixed last_vblank_usage = bn::core::last_vblank_usage();

        if(last_vblank_usage >= 1)
        {
            constexpr int vdraw_pct_multiplier =
                    (bn::timers::ticks_per_vblank() * 100) / bn::timers::ticks_per_frame();

            int vblank_pct = (last_vblank_usage * 100).right_shift_integer();
            int vdraw_pct = (last_vblank_usage * vdraw_pct_multiplier).right_shift_integer();
            BN_ERROR("High V-Blank usage: ", vblank_pct, "% - ", vdraw_pct, '%');
        }
    #endif

    _update_keypad();
}

void core::_update_keypad()
{
    bool reset_held = bn::keypad::select_held() && bn::keypad::start_held() && bn::keypad::b_held() &&
            bn::keypad::a_held();

    if(_reset_ready)
    {
        if(reset_held)
        {
            bn::core::reset();
        }
    }
    else
    {
        if(! reset_held)
        {
            _reset_ready = true;
        }
    }
}

}
