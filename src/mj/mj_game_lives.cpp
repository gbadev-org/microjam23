#include "mj/mj_game_lives.h"

#include "bn_sprite_affine_mat_ptr.h"

#include "bn_sprite_items_mj_small_pumpkin.h"

namespace mj
{

void game_lives::show()
{
    _show = true;
    _sprite_animations.clear();
}

void game_lives::hide()
{
    bn::sprite_affine_mat_ptr affine_mat = bn::sprite_affine_mat_ptr::create();

    for(bn::isprite_animate_action& sprite_animation : _sprite_animations)
    {
        bn::sprite_ptr sprite = sprite_animation.sprite();
        sprite.set_affine_mat(affine_mat);
    }

    _show = false;
}

void game_lives::update()
{
    if(_show)
    {
        if(_lives)
        {
            if(_sprite_animations.empty())
            {
                for(int index = 0; index < _lives; ++index)
                {
                    bn::fixed x = 128 + (index * 22);
                    bn::sprite_ptr sprite = bn::sprite_items::mj_small_pumpkin.create_sprite(x, 20 - 80);
                    sprite.set_bg_priority(0);

                    _sprite_animations.push_back(
                                bn::create_sprite_animate_action_forever(
                                    bn::move(sprite), 4, bn::sprite_items::mj_small_pumpkin.tiles_item(), 4, 3, 4, 5));
                }
            }

            for(bn::isprite_animate_action& sprite_animation : _sprite_animations)
            {
                bn::sprite_ptr sprite = sprite_animation.sprite();
                sprite.set_x(sprite.x() - 2);
                sprite_animation.update();
            }

            if(_sprite_animations[0].sprite().x() == 120 - (_lives * 22))
            {
                for(bn::isprite_animate_action& sprite_animation : _sprite_animations)
                {
                    bn::sprite_ptr sprite = sprite_animation.sprite();
                    sprite.set_tiles(bn::sprite_items::mj_small_pumpkin.tiles_item(), 7);
                }

                _show = false;
            }
        }
        else
        {
            _show = false;
        }
    }

    if(! _sprite_animations.empty())
    {
        if(bn::optional<bn::sprite_affine_mat_ptr> affine_mat = _sprite_animations[0].sprite().affine_mat())
        {
            bn::fixed scale = affine_mat->horizontal_scale() - (bn::fixed(1) / 8);

            if(scale > 0)
            {
                affine_mat->set_scale(scale);
            }
            else
            {
                _sprite_animations.clear();
            }
        }
    }
}

}
