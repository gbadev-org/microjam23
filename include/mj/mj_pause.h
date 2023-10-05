#ifndef MJ_PAUSE_H
#define MJ_PAUSE_H

#include "bn_sprite_ptr.h"
#include "bn_vector.h"

namespace mj
{

class core;

class pause
{

public:
    explicit pause(core& core);

    ~pause();

    [[nodiscard]] bool paused() const
    {
        return _sprites[0].visible();
    }

    [[nodiscard]] bool update(bool& exit);

private:
    bn::vector<bn::sprite_ptr, 16> _sprites;
    bn::fixed _sound_master_volume;
    int _back_to_game_delay_frames = 0;
    bool _exit_selected = false;
};

}

#endif
