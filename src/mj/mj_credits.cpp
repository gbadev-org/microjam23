#include "mj/mj_credits.h"

#include "bn_colors.h"
#include "bn_display.h"
#include "bn_keypad.h"
#include "bn_unordered_set.h"

#include "mj/mj_core.h"
#include "mj/mj_game_list.h"

#include "bn_music_items.h"

namespace mj
{

namespace
{
    constexpr bn::fixed text_y_inc = 0.5;
    constexpr bn::fixed text_y_limit = (bn::display::height() + 32) / 2;
    constexpr int fade_frames = 64;

    [[nodiscard]] bn::bg_palettes_fade_to_action _create_bgs_fade_in_action()
    {
        bn::bg_palettes::set_fade_intensity(1);
        return bn::bg_palettes_fade_to_action(fade_frames, 0);
    }

    [[nodiscard]] bn::sprite_palettes_fade_to_action _create_sprites_fade_in_action()
    {
        bn::sprite_palettes::set_fade_intensity(1);
        return bn::sprite_palettes_fade_to_action(fade_frames, 0);
    }
}

credits::credits(core& core) :
    _core(core),
    _bgs_fade_in_action(_create_bgs_fade_in_action()),
    _sprites_fade_in_action(_create_sprites_fade_in_action())
{
    _generate_text_items();

    bn::music_items::mj_ven_sopena_halloween.play(0.5);
}

int credits::total_frames() const
{
    return (fade_frames * 3) + (text_y_limit * 2).right_shift_integer() +
            (_text_items.size() * (16 / text_y_inc).right_shift_integer()) + 34;
}

bool credits::update()
{
    bool exit = false;

    if(! _bgs_fade_in_action.done())
    {
        _bgs_fade_in_action.update();
        _sprites_fade_in_action.update();
    }
    else if(_bgs_fade_out_action)
    {
        _bgs_fade_out_action->update();
        _sprites_fade_out_action->update();
        _music_volume_action->update();

        if(_bgs_fade_out_action->done())
        {
            _bgs_fade_out_action.reset();
            _sprites_fade_out_action.reset();
            _music_volume_action.reset();
            bn::bg_palettes::set_transparent_color(bn::colors::black);
            bn::music::stop();
            exit = true;
        }
    }
    else if(bn::keypad::start_pressed())
    {
        _finish();
    }

    if(! exit)
    {
        _update_text();
    }

    return ! exit;
}

void credits::_generate_text_items()
{
    _text_items.push_back("GBA Microjam 23");
    _text_items.push_back("");
    _text_items.push_back("");
    _text_items.push_back("");
    _text_items.push_back("Code");
    _text_items.push_back("");

    bn::unordered_set<bn::string_view, 512> text_items_set;
    text_items_set.insert("GValiente");

    for(bn::span<const bn::string_view> game_credits : game_list::get_code_credits())
    {
        for(const bn::string_view& text_item : game_credits)
        {
            text_items_set.insert(text_item);
        }
    }

    for(const bn::string_view& text_item : text_items_set)
    {
        _text_items.push_back(text_item);
    }

    _text_items.push_back("");
    _text_items.push_back("");
    _text_items.push_back("");
    _text_items.push_back("Graphics");
    _text_items.push_back("");

    text_items_set.clear();
    text_items_set.insert("codeinfernogames");
    text_items_set.insert("Pixabay");
    text_items_set.insert("Emcee Flesher");
    text_items_set.insert("bevouliin.com");
    text_items_set.insert("Paul Wortmann");
    text_items_set.insert("CloutierFontes");
    text_items_set.insert("Pixel-boy at Sparklin Labs");
    text_items_set.insert("Evan Bowman");
    text_items_set.insert("Leonard Pabin");
    text_items_set.insert("Screaming Brain Studios");
    text_items_set.insert("GValiente");
    text_items_set.insert("Pickpik");
    text_items_set.insert("Jono");
    text_items_set.insert("vectorportal.com");
    text_items_set.insert("Sun Ladder");

    for(bn::span<const bn::string_view> game_credits : game_list::get_graphics_credits())
    {
        for(const bn::string_view& text_item : game_credits)
        {
            text_items_set.insert(text_item);
        }
    }

    for(const bn::string_view& text_item : text_items_set)
    {
        _text_items.push_back(text_item);
    }

    _text_items.push_back("");
    _text_items.push_back("");
    _text_items.push_back("");
    _text_items.push_back("Music");
    _text_items.push_back("");

    text_items_set.clear();
    text_items_set.insert("exelotl");
    text_items_set.insert("Totsnuk");
    text_items_set.insert("Ven Sopeña");

    for(bn::span<const bn::string_view> game_credits : game_list::get_music_credits())
    {
        for(const bn::string_view& text_item : game_credits)
        {
            text_items_set.insert(text_item);
        }
    }

    for(const bn::string_view& text_item : text_items_set)
    {
        _text_items.push_back(text_item);
    }

    _text_items.push_back("");
    _text_items.push_back("");
    _text_items.push_back("");
    _text_items.push_back("Sound effects");
    _text_items.push_back("");

    text_items_set.clear();
    text_items_set.insert("Tempest");

    for(bn::span<const bn::string_view> game_credits : game_list::get_sfx_credits())
    {
        for(const bn::string_view& text_item : game_credits)
        {
            text_items_set.insert(text_item);
        }
    }

    for(const bn::string_view& text_item : text_items_set)
    {
        _text_items.push_back(text_item);
    }

    _text_items.push_back("");
    _text_items.push_back("");
    _text_items.push_back("");
    _text_items.push_back("Thanks for playing!");
}

void credits::_update_text()
{
    for(int index = 0, limit = bn::keypad::a_held() ? speed_up_frames : 1; index < limit; ++index)
    {
        for(auto it = _text_entries.begin(), end = _text_entries.end(); it != end; )
        {
            text_entry& text = *it;
            bn::fixed text_y = text.y;

            if(text_y <= -text_y_limit)
            {
                _text_entries.erase(it);
                end = _text_entries.end();

                if(_text_entries.empty())
                {
                    _finish();
                }
            }
            else
            {
                text_y -= text_y_inc;
                text.y = text_y;
                ++it;

                for(bn::sprite_ptr& sprite : text.sprites)
                {
                    sprite.set_y(text_y);
                }
            }
        }

        if(! _bgs_fade_out_action)
        {
            --_text_counter;

            if(! _text_counter)
            {
                _text_counter = (16 / text_y_inc).right_shift_integer();

                if(_text_index < _text_items.size())
                {
                    text_entry entry;
                    entry.y = text_y_limit;

                    const bn::string_view& text_item = _text_items[_text_index];
                    bn::sprite_text_generator& text_generator = _core.text_generator();
                    text_generator.set_center_alignment();
                    text_generator.generate(0, text_y_limit, text_item, entry.sprites);
                    text_generator.set_left_alignment();

                    _text_entries.push_back(bn::move(entry));
                    ++_text_index;
                }
            }
        }
    }
}

void credits::_finish()
{
    if(! _bgs_fade_out_action)
    {
        bn::bg_palettes::set_fade_color(bn::colors::black);
        bn::sprite_palettes::set_fade_color(bn::colors::black);
        _bgs_fade_out_action.emplace(fade_frames, 1);
        _sprites_fade_out_action.emplace(fade_frames, 1);
        _music_volume_action.emplace(fade_frames, 0);
    }
}

}
