#include "mj/mj_game_title.h"

#include "bn_string.h"
#include "bn_sprite_affine_mat_ptr.h"

#include "mj/mj_core.h"

namespace mj
{

namespace
{
    constexpr int scale_frames = 12;
    constexpr int max_wait_frames = 50;
}

void game_title::show(const bn::istring& title, core& core)
{
    BN_BASIC_ASSERT(! title.empty(), "Game title is empty");

    clear();

    bn::sprite_text_generator& text_generator = core.big_text_generator();
    text_generator.set_bg_priority(0);
    text_generator.set_center_alignment();
    text_generator.set_one_sprite_per_character(true);

    text_generator.generate(0, -16, title, _sprites);

    bn::sprite_affine_mat_ptr affine_mat = bn::sprite_affine_mat_ptr::create();
    affine_mat.set_scale(2);

    for(bn::sprite_ptr& sprite : _sprites)
    {
        sprite.set_affine_mat(affine_mat);
        _sprite_xs.push_back(sprite.x());
    }

    text_generator.set_bg_priority(3);
    text_generator.set_left_alignment();
    text_generator.set_one_sprite_per_character(false);
}

void game_title::clear()
{
    _sprites.clear();
    _sprite_xs.clear();
    _counter = 0;
}

void game_title::update(int total_frames)
{
    if(! _sprites.empty())
    {
        if(_counter)
        {
            --_counter;

            if(! _counter)
            {
                _sprites.clear();
            }
        }
        else
        {
            bn::sprite_affine_mat_ptr affine_mat = *_sprites[0].affine_mat();
            bn::fixed scale = affine_mat.horizontal_scale() - (bn::fixed(1) / scale_frames);

            if(scale > 1)
            {
                affine_mat.set_scale(scale);

                for(int index = 0, limit = _sprites.size(); index < limit; ++index)
                {
                    bn::sprite_ptr& title_sprite = _sprites[index];
                    title_sprite.set_x(_sprite_xs[index].unsafe_multiplication(scale));
                }
            }
            else
            {
                affine_mat.set_scale(1);
                _counter = bn::min(total_frames / 4, max_wait_frames);

                for(int index = 0, limit = _sprites.size(); index < limit; ++index)
                {
                    bn::sprite_ptr& title_sprite = _sprites[index];
                    title_sprite.set_x(_sprite_xs[index]);
                    title_sprite.remove_affine_mat();
                }
            }
        }
    }
}

}
