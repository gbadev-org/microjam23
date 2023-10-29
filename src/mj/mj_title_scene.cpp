#include "mj/mj_title_scene.h"

#include "bn_blending.h"
#include "bn_fixed_point.h"
#include "bn_keypad.h"
#include "bn_sprite_double_size_mode.h"
#include "bn_string.h"

#include "mj/mj_core.h"
#include "mj/mj_scene_type.h"
#include "mj/mj_title_backdrop_1.h"
#include "mj/mj_title_backdrop_2.h"

#include "bn_music_items.h"
#include "bn_sound_items.h"
#include "bn_sprite_items_mj_small_pumpkin.h"
#include "bn_sprite_items_mj_title.h"
#include "bn_sprite_items_mj_title_a.h"
#include "bn_sprite_items_mj_title_micro.h"
#include "bn_sprite_items_mj_title_pumpkin.h"
#include "bn_sprite_items_mj_title_23.h"

namespace mj
{

namespace
{
    constexpr int fade_frames = title_backdrop::fade_frames;
    constexpr int title_frames = 128;
}

title_scene::title_scene(core& core) :
    _cursor_sprite(bn::sprite_items::mj_small_pumpkin.create_sprite(0, 0, 12)),
    _affine_mat(bn::sprite_affine_mat_ptr::create()),
    _affine_mat_hbe(bn::sprite_affine_mat_attributes_hbe_ptr::create(_affine_mat, _affine_mat_attributes))
{
    sram_data& sram_data = core.sram_data();
    int old_intro_index = sram_data.intro_index();
    int new_intro_index = old_intro_index + 1;

    switch(old_intro_index)
    {

    case 0:
        _backdrop.reset(new title_backdrop_1());
        break;

    default:
        _backdrop.reset(new title_backdrop_2());
        new_intro_index = 0;
        break;
    }

    sram_data.set_intro_index(new_intro_index);
    sram_data.write();

    bn::bg_palettes::set_fade(bn::color(), 1);
    _bgs_fade_action.emplace(fade_frames, 0);

    _create_title_sprites();
    bn::blending::set_transparency_alpha(0);
    bn::sprite_palettes::set_fade(bn::color(), 1);
    _sprites_fade_action.emplace(fade_frames, 0);

    bn::string<20> high_score_text("HIGH SCORE: ");
    high_score_text.append(bn::to_string<8>(core.sram_data().high_scores()[0]));

    bn::sprite_text_generator& text_generator = core.small_text_generator();
    text_generator.set_center_alignment();
    text_generator.generate(-22, 12 - (160 / 2), high_score_text, _high_score_sprites);
    text_generator.set_left_alignment();
    text_generator.generate(-14, 32, "PLAY", _play_sprites);
    text_generator.generate(-14, 32 + 12, "CREDITS", _credits_sprites);
    _cursor_sprite.set_position(_play_sprites[0].position() - bn::fixed_point(28, 0));

    _set_menu_visible(false);
}

title_scene::~title_scene()
{
    bn::bg_palettes::set_fade_intensity(0);
    bn::sprite_palettes::set_fade_intensity(0);
}

bn::optional<scene_type> title_scene::update()
{
    bn::optional<scene_type> result;
    _update_bgs();

    if(_sprites_fade_action)
    {
        _sprites_fade_action->update();

        if(_music_volume_action)
        {
            _music_volume_action->update();
        }

        if(_sprites_fade_action->done())
        {
            _sprites_fade_action.reset();
            _music_volume_action.reset();
            result = _next_scene;
        }
    }
    else if(_title_sprites_1[0].affine_mat())
    {
        _update_title_sprites(true);
    }
    else if(_title_sprites_2[0].affine_mat())
    {
        _update_title_sprites(false);
    }
    else
    {
        _update_menu();
    }

    return result;
}

void title_scene::_update_bgs()
{
    if(_bgs_fade_action)
    {
        _bgs_fade_action->update();

        if(_bgs_fade_action->done())
        {
            _bgs_fade_action.reset();
        }
    }

    if(bn::optional<bn::fixed> fade_intensity = _backdrop->update())
    {
        if(! _bgs_fade_action && ! _next_scene)
        {
            _bgs_fade_action.emplace(fade_frames, *fade_intensity);
        }
    }
}

void title_scene::_create_title_sprites()
{
    bn::fixed x = -59;
    bn::fixed y = -32;
    _title_sprites_1.push_back(bn::sprite_items::mj_title.create_sprite(x, y));
    x += 34;
    _title_sprites_1.push_back(bn::sprite_items::mj_title.create_sprite(x, y, 1));
    x += 36;
    _title_sprites_1.push_back(bn::sprite_items::mj_title_a.create_sprite(x, y));

    x -= 24;
    y += 32;
    _title_sprites_1.push_back(bn::sprite_items::mj_title.create_sprite(x, y, 2));
    x += 30;
    _title_sprites_1.push_back(bn::sprite_items::mj_title_a.create_sprite(x, y, 1));
    x += 33;
    _title_sprites_1.push_back(bn::sprite_items::mj_title.create_sprite(x, y, 3));
    x += 28;
    _title_sprites_2.push_back(bn::sprite_items::mj_title_23.create_sprite(x, y));
    x += 20;
    _title_sprites_2.push_back(bn::sprite_items::mj_title_23.create_sprite(x, y, 1));

    x -= 198;
    y -= 5;
    _title_sprites_2.push_back(bn::sprite_items::mj_title_micro.create_sprite(x, y));
    x += 17;
    _title_sprites_2.push_back(bn::sprite_items::mj_title_micro.create_sprite(x, y, 1));
    x += 16;
    _title_sprites_2.push_back(bn::sprite_items::mj_title_micro.create_sprite(x, y, 2));
    x += 16;
    _title_sprites_2.push_back(bn::sprite_items::mj_title_micro.create_sprite(x, y, 3));
    x += 15;
    _title_sprites_2.push_back(bn::sprite_items::mj_title_micro.create_sprite(x, y, 4));

    x += 100;
    y -= 34;
    _title_sprites_2.push_back(bn::sprite_items::mj_title_pumpkin.create_sprite(x, y));

    for(bn::sprite_ptr& title_sprite : _title_sprites_1)
    {
        title_sprite.set_blending_enabled(true);
        title_sprite.set_affine_mat(_affine_mat);
        title_sprite.set_double_size_mode(bn::sprite_double_size_mode::ENABLED);
        title_sprite.set_z_order(-1);
    }

    for(bn::sprite_ptr& title_sprite : _title_sprites_2)
    {
        title_sprite.set_blending_enabled(true);
        title_sprite.set_affine_mat(_affine_mat);
        title_sprite.set_double_size_mode(bn::sprite_double_size_mode::ENABLED);
        title_sprite.set_visible(false);
    }
}

void title_scene::_update_title_sprites(bool update_first)
{
    bn::fixed transparency_alpha_inc = bn::fixed(1) / title_frames;
    bn::fixed transparency_alpha = bn::blending::transparency_alpha() + transparency_alpha_inc;
    bn::fixed intensity_alpha;

    if(transparency_alpha < 1)
    {
        bn::fixed rotation_angle = (1 - transparency_alpha) * 360;
        bn::fixed rotation_angle_inc = (1 - transparency_alpha) * 8;
        bn::fixed scale = 1;

        if(! update_first)
        {
            scale += (1 - transparency_alpha) / 2;
        }

        if(transparency_alpha < 0.75)
        {
            intensity_alpha = 1;
        }
        else
        {
            intensity_alpha = (1 - transparency_alpha) * 4;
        }

        for(int index = 0; index < 100; ++index)
        {
            bn::affine_mat_attributes& attributes = _affine_mat_attributes[index];
            attributes.set_scale(scale);
            attributes.set_rotation_angle(rotation_angle);
            rotation_angle -= rotation_angle_inc;

            if(rotation_angle < 0)
            {
                rotation_angle += 360;
            }
        }

        _affine_mat_hbe.reload_attributes_ref();
    }
    else
    {
        if(update_first)
        {
            transparency_alpha = 0;

            for(bn::sprite_ptr& title_sprite : _title_sprites_1)
            {
                title_sprite.set_blending_enabled(false);
                title_sprite.remove_affine_mat();
            }

            for(bn::sprite_ptr& title_sprite : _title_sprites_2)
            {
                title_sprite.set_visible(true);
            }
        }
        else
        {
            transparency_alpha = 1;

            for(bn::sprite_ptr& title_sprite : _title_sprites_2)
            {
                title_sprite.set_blending_enabled(false);
                title_sprite.remove_affine_mat();
            }

            _affine_mat_hbe.set_visible(false);
            _set_menu_visible(true);
        }
    }

    bn::blending::set_transparency_alpha(transparency_alpha);
    bn::blending::set_intensity_alpha(intensity_alpha);
}

void title_scene::_set_menu_visible(bool visible)
{
    _cursor_sprite.set_visible(visible);

    for(bn::sprite_ptr& sprite : _high_score_sprites)
    {
        sprite.set_visible(visible);
    }

    for(bn::sprite_ptr& sprite : _play_sprites)
    {
        sprite.set_visible(visible);
    }

    for(bn::sprite_ptr& sprite : _credits_sprites)
    {
        sprite.set_visible(visible);
    }
}

void title_scene::_update_menu()
{
    bn::fixed play_y = _play_sprites[0].y();
    bn::fixed credits_y = _credits_sprites[0].y();
    bn::fixed cursor_y = _cursor_sprite.y();

    if(bn::keypad::up_pressed() || bn::keypad::down_pressed())
    {
        if(cursor_y == play_y)
        {
            _cursor_sprite.set_y(credits_y);
        }
        else
        {
            _cursor_sprite.set_y(play_y);
        }

        bn::sound_items::mj_pause_cursor.play();
    }
    else if(bn::keypad::a_pressed())
    {
        if(cursor_y == play_y)
        {
            _next_scene = scene_type::GAME;
        }
        else
        {
            _next_scene = scene_type::CREDITS;
        }

        if(! _bgs_fade_action)
        {
            _bgs_fade_action.emplace(fade_frames, 1);
        }

        if(bn::music::playing())
        {
            _music_volume_action.emplace(fade_frames, 0);
        }

        _sprites_fade_action.emplace(fade_frames, 1);
        _cursor_sprite.set_tiles(bn::sprite_items::mj_small_pumpkin.tiles_item(), 13);
        bn::sound_items::mj_pause_begin.play();
    }
}

}
