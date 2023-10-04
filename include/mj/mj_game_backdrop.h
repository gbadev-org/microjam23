#ifndef MJ_GAME_BACKDROP_H
#define MJ_GAME_BACKDROP_H

#include "bn_bg_palettes_transparent_color_hbe_ptr.h"
#include "bn_color.h"
#include "bn_sprite_ptr.h"
#include "bn_vector.h"

namespace mj
{

class core;

class game_backdrop
{

public:
    game_backdrop();

    ~game_backdrop();

    void fade_out();

    void fade_in();

    void update(core& core);

private:
    bn::color _transparent_colors[160];
    bn::vector<bn::sprite_ptr, 8> _sprites;
    bn::fixed _fade_intensity = 0;
    int _counter = 0;
    bn::bg_palettes_transparent_color_hbe_ptr _transparent_color_hbe;
    bool _fade_inc = false;
    bool _fade_dec = false;
};

}

#endif
