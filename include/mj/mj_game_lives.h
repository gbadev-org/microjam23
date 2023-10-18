#ifndef MJ_GAME_LIVES_H
#define MJ_GAME_LIVES_H

#include "bn_sprite_animate_actions.h"

namespace mj
{

class game_lives
{

public:
    [[nodiscard]] int left() const
    {
        return _lives;
    }

    void decrease()
    {
        --_lives;
    }

    void show(bool victory, bool defeat);

    void stop()
    {
        _stop = true;
    }

    void look_down();

    void hide();

    void update();

private:
    bn::vector<bn::sprite_animate_action<4>, 16> _sprite_animations;
    int _lives = 4;
    bool _show = false;
    bool _stop = false;
    bool _hide = false;
    bool _victory = false;
    bool _defeat = false;

    void _update_show();

    void _update_victory();

    void _update_defeat();

    void _update_hide();
};

}

#endif
