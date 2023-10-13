#ifndef MJ_CREDITS_H
#define MJ_CREDITS_H

#include "bn_bg_palettes_actions.h"
#include "bn_deque.h"
#include "bn_music_actions.h"
#include "bn_sprite_palettes_actions.h"
#include "bn_sprite_ptr.h"
#include "bn_string_view.h"
#include "bn_vector.h"

namespace mj
{

class core;

class credits
{

public:
    static constexpr int speed_up_frames = 8;

    explicit credits(core& core);

    [[nodiscard]] int total_frames() const;

    [[nodiscard]] bool update();

private:
    struct text_entry
    {
        bn::vector<bn::sprite_ptr, 12> sprites;
        bn::fixed y;
    };

    core& _core;
    bn::vector<bn::string_view, 1024> _text_items;
    bn::bg_palettes_fade_to_action _bgs_fade_in_action;
    bn::sprite_palettes_fade_to_action _sprites_fade_in_action;
    bn::deque<text_entry, 16> _text_entries;
    bn::optional<bn::bg_palettes_fade_to_action> _bgs_fade_out_action;
    bn::optional<bn::sprite_palettes_fade_to_action> _sprites_fade_out_action;
    bn::optional<bn::music_volume_to_action> _music_volume_action;
    int _text_index = 0;
    int _text_counter = 1;

    void _generate_text_items();

    void _update_text();

    void _finish();
};

}

#endif
