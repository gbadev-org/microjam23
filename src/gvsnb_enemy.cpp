#include "gvsnb_enemy.h"

#include "bn_math.h"
#include "bn_memory.h"
#include "bn_random.h"

#include "gvsnb_utils.h"

#include "bn_sprite_items_gvsnb_kid.h"

namespace gvsnb
{

namespace
{
    constexpr int walk_tiles_index = 0;
    constexpr int wait_tiles_index = 6;
    constexpr int jump_tiles_index = 7;
    constexpr int hurt_tiles_index = 9;
    constexpr int update_walk_tiles_frames = 3;

    constexpr bn::fixed hit_vertical_scale_dec = bn::fixed(1) / 24;

    constexpr int last_jump_final_y = -128;

    [[nodiscard]] bn::fixed _multiplier(uint8_t difficulty_level)
    {
        return 1 + (bn::fixed(difficulty_level) / 8);
    }
}

enemy_gfx::enemy_gfx()
{
    for(int index = 0; index < tiles.max_size(); ++index)
    {
        tiles.push_back(bn::sprite_items::gvsnb_kid.tiles_item().create_tiles(index));
    }

    // red
    const bn::sprite_palette_item& palette_item = bn::sprite_items::gvsnb_kid.palette_item();
    palettes.push_back(palette_item.create_palette());

    bn::array<bn::color, 16> colors;
    bn::sprite_palette_item colors_item(colors, palette_item.bpp());
    const bn::span<const bn::color>& item_colors_ref = palette_item.colors_ref();
    bn::memory::copy(item_colors_ref[0], 16, colors[0]);

    // blue
    for(int index = 12; index < 16; ++index)
    {
        bn::color& color = colors[index];
        color = bn::color(color.blue(), color.green(), color.red());
    }

    palettes.push_back(colors_item.create_palette());

    // green
    for(int index = 12; index < 16; ++index)
    {
        bn::color& color = colors[index];
        color = bn::color(color.red(), color.blue(), color.green());
    }

    palettes.push_back(colors_item.create_palette());

    // black
    for(int index = 12; index < 16; ++index)
    {
        bn::color& color = colors[index];
        color = bn::color(color.red(), color.red(), color.blue());
    }

    palettes.push_back(colors_item.create_palette());
}

enemy::enemy(
        int difficulty_level, int door_level, bool right, bn::fixed tempo, const enemy_gfx& gfx, bn::random& random) :
    _sprite(bn::sprite_ptr::create(0, 0, bn::sprite_items::gvsnb_kid.shape_size(),
                                   gfx.tiles[wait_tiles_index], gfx.palettes[difficulty_level - 1])),
    _x(right ? utils::door_x : -utils::door_x),
    _difficulty_level(uint8_t(difficulty_level)),
    _door_level(uint8_t(door_level))
{
    _sprite.set_horizontal_flip(right);
    _set_z_order(door_level);
    _start_cooldown(tempo, true, false, random);
}

bool enemy::alive() const
{
    return _tiles_index != hurt_tiles_index;
}

bool enemy::candy_hit(bn::fixed x, bn::fixed y)
{
    if(alive() && ! _jump_frames)
    {
        if(bn::abs(x - _x) < 16 && bn::abs(y - _y) < 11)
        {
            _hit_vertical_scale = 1;
            _tiles_index = hurt_tiles_index;
            return true;
        }
    }

    return false;
}

bool enemy::update(bn::fixed tempo, bn::random& random)
{
    if(_hit_vertical_scale > 0)
    {
        _hit_vertical_scale -= hit_vertical_scale_dec.unsafe_multiplication(tempo);

        return _hit_vertical_scale > 0;
    }

    if(_cooldown_frames)
    {
        --_cooldown_frames;
        return true;
    }

    if(_walk_frames)
    {
        --_walk_frames;

        if(_walk_frames)
        {
            _update_walking(tempo);
        }
        else
        {
            _start_cooldown(tempo, false, false, random);
        }

        return true;
    }

    if(_jump_frames)
    {
        --_jump_frames;

        if(_jump_frames)
        {
            _update_jumping();
        }
        else if(_tiles_index == jump_tiles_index)
        {
            if(_door_level == 3)
            {
                return false;
            }

            _start_jumping_down();
        }
        else
        {
            ++_door_level;
            _start_cooldown(tempo, false, true, random);
        }

        return true;
    }

    if(_start_walk)
    {
        _start_walking(tempo, random);
    }
    else
    {
        _start_jumping_up(tempo, random);
    }

    return true;
}

void enemy::draw(const enemy_gfx& gfx)
{
    bn::fixed x = _x;
    bn::fixed y = _y;
    bn::fixed horizontal_scale;
    bn::fixed vertical_scale;
    _sprite.set_tiles(gfx.tiles[_tiles_index]);

    if(_jump_frames && _door_level == 3)
    {
        int diff = last_jump_final_y - utils::y_3;
        horizontal_scale = 1 + ((y - utils::y_3) / diff);
        vertical_scale = horizontal_scale;
        x /= horizontal_scale;
    }
    else
    {
        x = x.unsafe_multiplication(utils::x_mult(y));
        horizontal_scale = utils::scale(y);
        vertical_scale = horizontal_scale;

        if(_hit_vertical_scale > 0)
        {
            vertical_scale = bn::max(vertical_scale.unsafe_multiplication(_hit_vertical_scale), bn::fixed(0.01));
        }
    }

    _sprite.set_position(x, y);
    _sprite.set_scale(horizontal_scale, vertical_scale);
}

void enemy::_set_z_order(int level)
{
    _sprite.set_z_order(utils::enemy_z_order - level);
}

void enemy::_start_cooldown(bn::fixed tempo, bool first, bool after_jump, bn::random& random)
{
    switch(_door_level)
    {

    case 1:
        _y = utils::y_1;
        break;

    case 2:
        _y = utils::y_2;
        break;

    case 3:
    default:
        _y = utils::y_3;
        break;
    }

    _tiles_index = wait_tiles_index;

    bn::fixed cooldown_frames = 24;

    if(! first)
    {
        cooldown_frames += 8;
    }

    bn::fixed divisor = _multiplier(_difficulty_level).unsafe_multiplication(tempo);
    cooldown_frames = cooldown_frames.unsafe_multiplication(random.get_fixed(1, 1.375));
    cooldown_frames = cooldown_frames.unsafe_division(divisor);

    if(! after_jump && _door_level == 3)
    {
        cooldown_frames *= 2;
    }

    _cooldown_frames = cooldown_frames.right_shift_integer();
}

void enemy::_start_walking(bn::fixed tempo, bn::random& random)
{
    bn::fixed x = _x;
    bn::fixed new_x;
    bn::fixed x_diff;

    do
    {
        new_x = random.get_fixed(-utils::door_x, utils::door_x);
        x_diff = new_x - x;
    }
    while(bn::abs(x_diff) < utils::door_x / 3);

    bool left = x_diff < 0;
    _tiles_index = walk_tiles_index;
    _sprite.set_horizontal_flip(left);
    _x_inc = 1 + random.get_fixed(0.25);
    _x_inc = _x_inc.unsafe_multiplication(_multiplier(_difficulty_level));
    _x_inc = _x_inc.unsafe_multiplication(tempo);

    if(left)
    {
        _x_inc = -_x_inc;
    }

    _walk_frames = (x_diff / _x_inc).right_shift_integer();
    _update_tiles_counter = update_walk_tiles_frames;
    _start_walk = false;
}

void enemy::_update_walking(bn::fixed tempo)
{
    _x += _x_inc;

    if(_update_tiles_counter > 0)
    {
        _update_tiles_counter -= tempo;
    }
    else
    {
        _tiles_index = (_tiles_index + 1) % wait_tiles_index;
        _update_tiles_counter += update_walk_tiles_frames;
    }
}

void enemy::_start_jumping_up(bn::fixed tempo, bn::random& random)
{
    auto door_level = int(_door_level);
    bn::fixed jump_frames;
    _tiles_index = jump_tiles_index;

    if(door_level < 3)
    {
        _set_z_order(door_level + 1);

        jump_frames = random.get_fixed(20, 20 + 4);
        jump_frames = jump_frames.unsafe_division(_multiplier(_difficulty_level));
    }
    else
    {
        _sprite.set_z_order(utils::candy_z_order - 1);
        _sprite.set_horizontal_flip(_x > 0);

        jump_frames = 48;
    }

    _total_jump_frames = jump_frames.unsafe_division(tempo).right_shift_integer();
    _jump_frames = _total_jump_frames;
}

void enemy::_start_jumping_down()
{
    _jump_frames = _total_jump_frames;
    _tiles_index = jump_tiles_index + 1;
    _start_walk = true;
}

void enemy::_update_jumping()
{
    bn::fixed sin;
    int y;
    int y_dest;

    switch(_door_level)
    {

    case 1:
        y = utils::y_1;
        y_dest = y - 6;
        break;

    case 2:
        y = utils::y_2;
        y_dest = y - 8;
        break;

    default:
        y = utils::y_3;
        y_dest = last_jump_final_y;
        break;
    }

    if(_tiles_index == jump_tiles_index)
    {
        int lut_angle = 512 - ((_jump_frames * 512) / _total_jump_frames);
        sin = bn::lut_sin(lut_angle);
    }
    else
    {
        y = y_dest;
        y_dest = _door_level == 1 ? utils::y_2 : utils::y_3;

        int lut_angle = (_jump_frames * 512) / _total_jump_frames;
        sin = 1 - bn::lut_sin(lut_angle);
    }

    _y = y + ((y_dest - y) * sin);
}

}
