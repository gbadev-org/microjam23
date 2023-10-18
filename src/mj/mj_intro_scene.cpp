#include "mj/mj_intro_scene.h"

#include "bn_colors.h"
#include "bn_fixed_point.h"
#include "bn_keypad.h"
#include "bn_string.h"
#include "bn_version.h"

#include "mj/mj_core.h"
#include "mj/mj_scene_type.h"

#include "bn_regular_bg_items_mj_intro_backdrop.h"
#include "bn_regular_bg_items_mj_intro_butano.h"

namespace mj
{

namespace
{
    [[nodiscard]] bn::bg_palettes_fade_to_action _create_bg_fade_action()
    {
        bn::bg_palettes::set_fade(bn::colors::maroon, 1);
        return bn::bg_palettes_fade_to_action(64, 0);
    }

    [[nodiscard]] bn::sprite_palettes_fade_to_action _create_sprite_fade_action()
    {
        bn::sprite_palettes::set_fade(bn::colors::maroon, 1);
        return bn::sprite_palettes_fade_to_action(64, 0);
    }
}

intro_scene::intro_scene(core& core) :
    _bg_fade_action(_create_bg_fade_action()),
    _sprite_fade_action(_create_sprite_fade_action()),
    _backdrop(bn::regular_bg_items::mj_intro_backdrop.create_bg((256 - 240) /  2, (256 - 160) / 2)),
    _butano(bn::regular_bg_items::mj_intro_butano.create_bg(0, 0))
{
    bn::string<32> middle_text("BUTANO ");
    middle_text.append(bn::to_string<4>(bn::version::major()));
    middle_text.append('.');
    middle_text.append(bn::to_string<4>(bn::version::minor()));
    middle_text.append('.');
    middle_text.append(bn::to_string<4>(bn::version::patch()));

    bn::sprite_text_generator& text_generator = core.text_generator();
    text_generator.set_center_alignment();
    text_generator.generate(0, -16, "Made with", _text_sprites);
    text_generator.generate(0, 0, middle_text, _text_sprites);
    text_generator.generate(0, 16, "github.com/GValiente/butano", _text_sprites);
    text_generator.set_left_alignment();
}

bn::optional<scene_type> intro_scene::update()
{
    bn::optional<scene_type> result;
    _update_butano = ! _update_butano;

    if(_update_butano)
    {
        _butano.set_position(_butano.position() + bn::fixed_point(1, -1));
    }

    if(! _bg_fade_action.done())
    {
        _bg_fade_action.update();
        _sprite_fade_action.update();
    }
    else
    {
        if(_counter > 0)
        {
            if(bn::keypad::a_pressed())
            {
                _counter = 0;
            }
            else
            {
                --_counter;
            }

            if(! _counter)
            {
                bn::bg_palettes::set_fade(bn::colors::black, 0);
                _bg_fade_action = bn::bg_palettes_fade_to_action(64, 1);

                bn::sprite_palettes::set_fade(bn::colors::black, 0);
                _sprite_fade_action = bn::sprite_palettes_fade_to_action(64, 1);
            }
        }
        else
        {
            bn::bg_palettes::set_fade(bn::colors::black, 0);
            bn::sprite_palettes::set_fade(bn::colors::black, 0);
            result = scene_type::TITLE;
        }
    }

    return result;
}

}
