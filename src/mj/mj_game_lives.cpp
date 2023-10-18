#include "mj/mj_game_lives.h"

#include "bn_sprite_affine_mat_ptr.h"

#include "bn_sprite_items_mj_small_pumpkin.h"

namespace mj
{

namespace
{
    bn::fixed sprite_y = 20 - 80;

    [[nodiscard]] bn::fixed sprite_x(int index)
    {
        return 120 - (index * 22);
    }
}

void game_lives::show(bool victory, bool defeat)
{
    _show = true;
    _stop = false;
    _hide = false;
    _victory = victory;
    _defeat = defeat;

    _sprite_animations.clear();
}

void game_lives::look_down()
{
    for(bn::isprite_animate_action& sprite_animation : _sprite_animations)
    {
        bn::sprite_ptr sprite = sprite_animation.sprite();
        sprite.set_tiles(bn::sprite_items::mj_small_pumpkin.tiles_item(), 7);
    }
}

void game_lives::hide()
{
    _show = false;
    _stop = false;
    _hide = true;
    _victory = false;
    _defeat = false;

    bn::sprite_affine_mat_ptr affine_mat = bn::sprite_affine_mat_ptr::create();

    for(bn::isprite_animate_action& sprite_animation : _sprite_animations)
    {
        bn::sprite_ptr sprite = sprite_animation.sprite();
        sprite.set_affine_mat(affine_mat);
    }
}

void game_lives::update()
{
    if(_show)
    {
        _update_show();
    }
    else if(_victory)
    {
        _update_victory();
    }
    else if(_defeat)
    {
        _update_defeat();
    }
    else if(_hide)
    {
        _update_hide();
    }
}

void game_lives::_update_show()
{
    int old_lives = _lives;

    if(_defeat)
    {
        ++old_lives;
    }

    if(old_lives)
    {
        if(_sprite_animations.empty())
        {
            for(int index = 0; index < old_lives; ++index)
            {
                bn::fixed x = sprite_x(old_lives - index);
                bn::sprite_ptr sprite = bn::sprite_items::mj_small_pumpkin.create_sprite(x, - 8 - 80);
                sprite.set_bg_priority(0);

                _sprite_animations.push_back(
                            bn::create_sprite_animate_action_forever(
                                bn::move(sprite), 4, bn::sprite_items::mj_small_pumpkin.tiles_item(), 7, 6, 7, 8));
            }
        }

        for(bn::isprite_animate_action& sprite_animation : _sprite_animations)
        {
            bn::sprite_ptr sprite = sprite_animation.sprite();
            sprite.set_y(sprite.y() + 2);
            sprite_animation.update();
        }

        if(_sprite_animations[0].sprite().y() == sprite_y)
        {
            _show = false;
        }
    }
    else
    {
        _show = false;
    }
}

void game_lives::_update_victory()
{
    for(bn::isprite_animate_action& sprite_animation : _sprite_animations)
    {
        sprite_animation.update();
    }

    if(_stop)
    {
        if(_sprite_animations.empty() || _sprite_animations[0].current_index() == 3)
        {
            _victory = false;
            _stop = false;
        }
    }
}

void game_lives::_update_defeat()
{
    if(int num_sprites = _sprite_animations.size())
    {
        bn::isprite_animate_action& first_sprite_animation = _sprite_animations[0];
        bn::sprite_ptr first_sprite = first_sprite_animation.sprite();
        bn::fixed vertical_scale = first_sprite.vertical_scale() - (bn::fixed(1) / 96);

        if(vertical_scale > 0)
        {
            first_sprite_animation.update();
            first_sprite.set_y(sprite_y + ((1 - vertical_scale) * 7));
            first_sprite.set_vertical_scale(vertical_scale);
        }
        else
        {
            _defeat = false;
        }

        bn::fixed desp_x;
        bn::fixed desp_y;

        if(_defeat)
        {
            switch(first_sprite_animation.current_index())
            {

            case 0:
                desp_x = -1;
                desp_y = 1;
                break;

            case 1:
                desp_x = 1;
                break;

            case 2:
                desp_y = -1;
                break;

            default:
                desp_x = 1;
                break;
            }
        }

        for(int index = 1; index < num_sprites; ++index)
        {
            bn::sprite_ptr sprite = _sprite_animations[index].sprite();
            sprite.set_position(sprite_x(index) + desp_x, sprite_y + desp_y);
            sprite.set_tiles(bn::sprite_items::mj_small_pumpkin.tiles_item(), 4);
        }

        if(! _defeat)
        {
            _sprite_animations.erase(_sprite_animations.begin());
        }
    }
    else
    {
        _defeat = false;
    }
}

void game_lives::_update_hide()
{
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
                _hide = false;
            }
        }
        else
        {
            _hide = false;
        }
    }
    else
    {
        _hide = false;
    }
}

}
