#ifndef URI_MURDER_GAME_H
#define URI_MURDER_GAME_H

#include "bn_regular_bg_ptr.h"
#include "bn_affine_bg_ptr.h"
#include "bn_sprite_ptr.h"
#include "bn_affine_bg_actions.h"

#include "bn_affine_bg_items_uri_police_station_bg.h"
#include "bn_regular_bg_items_uri_empty_bg.h"
#include "bn_sprite_items_uri_char_1.h"
#include "bn_sprite_items_uri_char_2.h"
#include "bn_sprite_items_uri_char_3.h"
#include "bn_sprite_items_uri_char_4.h"
#include "bn_sprite_items_uri_arrow_sprite.h"

#include "mj/mj_game.h"

namespace uri
{

class murder_game : public mj::game
{

public:
    murder_game(int completed_games, const mj::game_data& data);

    [[nodiscard]] bn::string<16> title() const final
    {
        return "Find the killer!";
    }

    [[nodiscard]] int total_frames() const final
    {
        return _total_frames;
    }

    void fade_in(const mj::game_data& data) final;

    [[nodiscard]] mj::game_result play(const mj::game_data& data) final;

    [[nodiscard]] bool victory() const final
    {
        return _victory;
    }

    void fade_out(const mj::game_data& data) final;

private:
    bn::regular_bg_ptr _bg = bn::regular_bg_items::uri_empty_bg.create_bg(0,0);
    bn::affine_bg_ptr _front_bg = bn::affine_bg_items::uri_police_station_bg.create_bg(0,0);
    bn::affine_bg_scale_to_action zoom_action = bn::affine_bg_scale_to_action(_front_bg, 60, 2);
    bn::affine_bg_move_to_action move_action = bn::affine_bg_move_to_action(_front_bg, 60, 0, 32);


    bn::sprite_item _char_1 = bn::sprite_items::uri_char_1;
    bn::sprite_item _char_2 = bn::sprite_items::uri_char_2;
    bn::sprite_item _char_3 = bn::sprite_items::uri_char_3;
    bn::sprite_item _char_4 = bn::sprite_items::uri_char_4;

    bn::sprite_ptr _char_1_top = _char_1.create_sprite(-90, -32, 2);
    bn::sprite_ptr _char_1_bottom = _char_1.create_sprite(-90, 32, 3);
    bn::sprite_ptr _char_2_top = _char_2.create_sprite(-30, -32, 2);
    bn::sprite_ptr _char_2_bottom = _char_2.create_sprite(-30, 32, 3);
    bn::sprite_ptr _char_3_top = _char_3.create_sprite(30, -32, 2);
    bn::sprite_ptr _char_3_bottom = _char_3.create_sprite(30, 32, 3);
    bn::sprite_ptr _char_4_top = _char_4.create_sprite(90, -32, 2);
    bn::sprite_ptr _char_4_bottom = _char_4.create_sprite(90, 32, 3);

    int _killer = 0;
    bool _is_setup = false;
    bool _is_game_over = false;

    bn::sprite_ptr _select_arrow = bn::sprite_items::uri_arrow_sprite.create_sprite(-200, 0);
    int _position = 0;
    int _total_frames = 0;
    int _show_result_frames = 60;
    bool _victory = false;
    bool _defeat = false;
};

}

#endif
