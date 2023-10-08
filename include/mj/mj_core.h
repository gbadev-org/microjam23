#ifndef MJ_CORE_H
#define MJ_CORE_H

#include "bn_random.h"
#include "bn_sprite_text_generator.h"

namespace mj
{

class core
{

public:
    core();

    [[nodiscard]] bn::sprite_text_generator& text_generator()
    {
        return _text_generator;
    }

    [[nodiscard]] bn::sprite_text_generator& small_text_generator()
    {
        return _small_text_generator;
    }

    [[nodiscard]] bn::sprite_text_generator& big_text_generator()
    {
        return _big_text_generator;
    }

    [[nodiscard]] bn::random& random()
    {
        return _random;
    }

    void update();

private:
    bn::sprite_text_generator _text_generator;
    bn::sprite_text_generator _small_text_generator;
    bn::sprite_text_generator _big_text_generator;
    bn::random _random;
    bool _reset_ready = false;

    void _update_keypad();
};

}

#endif
