#ifndef WWTF_WWTF_GAME_H
#define WWTF_WWTF_GAME_H

#include "bn_regular_bg_ptr.h"
#include "bn_sprite_ptr.h"
#include "bn_sprite_animate_actions.h"

#include "bn_sprite_items_wwtf_cloud.h"
#include "bn_sprite_items_wwtf_werewolf_transformation.h"

#include "bn_sound_handle.h"
#include "bn_sound_items.h"

#include "mj/mj_game.h"

namespace wwtf
{

class wwtf_game : public mj::game
{

public:
    wwtf_game(int completed_games, const mj::game_data& data);

    [[nodiscard]] bn::string<16> title() const final
    {
        return "Transform!";
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
    bn::sprite_ptr _werewolftransformation = bn::sprite_items::wwtf_werewolf_transformation.create_sprite(0,10);
    bn::sprite_animate_action<16> _animationaction = bn::create_sprite_animate_action_forever(_werewolftransformation, 3, bn::sprite_items::wwtf_werewolf_transformation.tiles_item(), 0, 1, 2, 3);

    bn::sprite_ptr _cloudsprite1 = bn::sprite_items::wwtf_cloud.create_sprite(-30,-50);
    bn::sprite_ptr _cloudsprite2 = bn::sprite_items::wwtf_cloud.create_sprite(-25,-30);
    bn::sprite_ptr _cloudsprite3 = bn::sprite_items::wwtf_cloud.create_sprite(30,-55);
    bn::sprite_ptr _cloudsprite4 = bn::sprite_items::wwtf_cloud.create_sprite(20,-35);
    bn::regular_bg_ptr _bg;
    bn::vector<bn::sprite_ptr, 8> _win_text_sprites;
    bn::vector<bn::sprite_ptr, 8> _lose_text_sprites;
    bn::vector<bn::sprite_ptr, 8> _lose_text_sprites_moon;
    bn::string_view _you_lose_label = bn::string_view("You lose!");
    bn::string_view _you_lose_label_moon = bn::string_view("Wait for the moon!");
    bn::string_view _you_win_label = bn::string_view("Release the BEAST!");

    bn::sound_handle _strugglesoundhandle = bn::sound_items::wwtf_struggle.play(0.5);

    int _total_frames;
    int _elapsed_frames = 0;            // number of frames elapsed since game start
    int _game_time_offset = 0;          // stores the randomised offset from the starting value
    int _show_result_frames = 120;       // number of frames to linger on the result before finshing
    bool _victory = false;
    bool _defeat = false;
    bool _cloudsmoving = false;         // weather the clouds have begun/finished their movement yet
    bool _victoryallowed = false;       // weather victory is allowed (clouds have revealed the moon)
};

}

#endif
