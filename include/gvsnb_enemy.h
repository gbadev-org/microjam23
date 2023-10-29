#ifndef GVSNB_ENEMY_H
#define GVSNB_ENEMY_H

#include "bn_sprite_palette_ptr.h"
#include "bn_sprite_ptr.h"
#include "bn_sprite_tiles_ptr.h"
#include "bn_vector.h"

namespace bn
{
    class random;
}

namespace gvsnb
{

struct enemy_gfx
{
    bn::vector<bn::sprite_tiles_ptr, 10> tiles;
    bn::vector<bn::sprite_palette_ptr, 4> palettes;

    enemy_gfx();
};


class enemy
{

public:
    enemy(int difficulty_level, int door_level, bool right, bn::fixed tempo, const enemy_gfx& gfx, bn::random& random);

    [[nodiscard]] int difficulty_level() const
    {
        return _difficulty_level;
    }

    [[nodiscard]] bool alive() const;

    [[nodiscard]] bool horizontal_flip() const
    {
        return _sprite.horizontal_flip();
    }

    [[nodiscard]] bool candy_hit(bn::fixed x, bn::fixed y);

    [[nodiscard]] bool update(bn::fixed tempo, bn::random& random);

    void draw(const enemy_gfx& gfx);

private:
    bn::sprite_ptr _sprite;
    bn::fixed _x;
    bn::fixed _y;
    bn::fixed _x_inc;
    bn::fixed _update_tiles_counter;
    bn::fixed _hit_vertical_scale;
    int _cooldown_frames;
    int _walk_frames = 0;
    int _jump_frames = 0;
    int _total_jump_frames = 0;
    uint8_t _difficulty_level;
    uint8_t _door_level;
    uint8_t _tiles_index;
    bool _start_walk = true;

    void _set_z_order(int level);

    void _start_cooldown(bn::fixed tempo, bool first, bool after_jump, bn::random& random);

    void _start_walking(bn::fixed tempo, bn::random& random);

    void _update_walking(bn::fixed tempo);

    void _start_jumping_up(bn::fixed tempo, bn::random& random);

    void _start_jumping_down();

    void _update_jumping();
};

}

#endif
