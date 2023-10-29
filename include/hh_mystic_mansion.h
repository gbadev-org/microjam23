#ifndef HH_HAUNTED_HOUSE_H
#define HH_HAUNTED_HOUSE_H

#include "bn_regular_bg_ptr.h"
#include "bn_affine_bg_ptr.h"
#include "bn_affine_bg_actions.h"
#include "bn_bg_palette_actions.h"
#include "bn_optional.h"
#include "bn_vector.h"

#include "hh_player.h"
#include "hh_bat.h"
#include "hh_spider.h"
#include "hh_ghost.h"

#include "mj/mj_game.h"


namespace hh
{



class explosion {
public:
    //fpc = frames per color
    explosion(bn::regular_bg_ptr &bg, uint8_t fpc);
    void update();
private:
    bn::regular_bg_ptr &_blackbg;
    bn::bg_palette_fade_to_action _fade;
    void reset_fade();
};

class lightbulb {
public:
    lightbulb(uint8_t descent_frames);
    void update();
    void hide_radiance(){ _radiance.set_visible(false);}
private:
    bn::sprite_ptr _bulb;
    bn::affine_bg_ptr _radiance;
    uint8_t _descent_frames, _total_descent_frames;
    bn::optional<bn::affine_bg_scale_loop_action> _radiance_scale;

    // , _swing_time, _timer;
};

class mystic_mansion : public mj::game
{

public:
    mystic_mansion(int completed_games, const mj::game_data& data);

    [[nodiscard]] bn::string<16> title() const final
    {
        return "Avoid monsters!";
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

    void spawn_enemy();

private:
    bn::regular_bg_ptr _blackbg, _room;
    bn::fixed _tempo;
    int _total_frames, _game_end_frame, _lights_on_end_frame, _lightbulb_appear_frame;
    int _show_result_frames = 120;
    bool _victory = false;
    bool _defeat = false;
    player _player;
    bn::optional<spider> _spider;
    bn::optional<bat> _bat;
    bn::optional<ghost> _ghost;
    bn::optional<lightbulb> _lightbulb;
    bn::sprite_ptr _peepantsometer, _fat_guy, _speech_bubble;
    bn::vector<bn::sprite_ptr, 14> _pee_bars;
    bn::vector<bn::sound_item, 9> _ambient_sounds;
    uint8_t _ambient_sound_timer;
    explosion _explosion;
    mj::difficulty_level _difficulty_level;

    // const uint8_t n = 1, _hard = 2, _very_hard = 5; 
    
    

    void spawn_enemy(const mj::game_data& data, uint8_t enemy_type);

    bn::vector<entity*, 4> all_entities();
};


}

#endif
