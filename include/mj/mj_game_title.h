#ifndef MJ_GAME_TITLE_H
#define MJ_GAME_TITLE_H

#include "bn_sprite_ptr.h"
#include "bn_vector.h"

namespace mj
{

class core;

class game_title
{

public:
    void show(const bn::istring& title, core& core);

    void clear();

    void update(int total_frames);

private:
    bn::vector<bn::sprite_ptr, 24> _sprites;
    bn::vector<bn::fixed, 16> _sprite_xs;
    int _counter = 0;
};

}

#endif
