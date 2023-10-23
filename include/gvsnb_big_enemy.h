#ifndef GVSNB_BIG_ENEMY_H
#define GVSNB_BIG_ENEMY_H

#include "bn_sprite_ptr.h"
#include "bn_vector.h"

namespace gvsnb
{

class enemy;
struct enemy_gfx;

class big_enemy
{

public:
    explicit big_enemy(const enemy_gfx& gfx);

    void setup(const enemy& enemy, const enemy_gfx& gfx);

    [[nodiscard]] bool update(bn::fixed tempo);

private:
    bn::vector<bn::sprite_ptr, 6> _sprites;
    int _counter;
};

}

#endif
