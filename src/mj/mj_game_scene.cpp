#include "mj/mj_game_scene.h"

#include "bn_bg_palettes.h"
#include "bn_colors.h"
#include "bn_dmg_music.h"
#include "bn_dmg_music_item.h"
#include "bn_music.h"
#include "bn_sprite_palettes.h"

#include "mj/mj_build_config.h"
#include "mj/mj_core.h"
#include "mj/mj_game_over_scene.h"
#include "mj/mj_game_result_animation.h"
#include "mj/mj_scene_type.h"

#include "bn_regular_bg_items_mj_big_pumpkin_1.h"
#include "bn_regular_bg_items_mj_big_pumpkin_2.h"
#include "bn_regular_bg_items_mj_big_pumpkin_3.h"
#include "bn_regular_bg_items_mj_big_pumpkin_4.h"
#include "bn_regular_bg_items_mj_big_pumpkin_5.h"
#include "bn_regular_bg_items_mj_big_pumpkin_6.h"
#include "bn_regular_bg_items_mj_big_pumpkin_7.h"
#include "bn_regular_bg_items_mj_big_pumpkin_8.h"
#include "bn_regular_bg_items_mj_big_pumpkin_9.h"
#include "bn_regular_bg_items_mj_big_pumpkin_10.h"
#include "bn_regular_bg_items_mj_big_pumpkin_11.h"
#include "bn_regular_bg_items_mj_big_pumpkin_12.h"

namespace mj
{

namespace
{
    constexpr int fade_in_frames = 32;
    constexpr int fade_out_frames = 64;
    constexpr int volume_dec_frames = 24;
}

game_scene::game_scene(core& core) :
    _core(core),
    _data({ core.text_generator(), core.small_text_generator(), core.big_text_generator(), core.random(), 0 }),
    _pause(core),
    _music_tempo(game::recommended_music_tempo(0, _data)),
    _completed_games(MJ_INITIAL_COMPLETED_GAMES),
    _fade_in_frames(fade_in_frames)
{
    bn::bg_palettes::set_fade(bn::colors::black, 1);
    bn::sprite_palettes::set_fade(bn::colors::black, 1);

    _update_big_pumpkin(&bn::regular_bg_items::mj_big_pumpkin_1);
    _lives.show(false, false);
}

game_scene::~game_scene()
{
    bn::bg_palettes::set_fade_intensity(0);
    bn::sprite_palettes::set_fade_intensity(0);
}

bn::optional<scene_type> game_scene::update()
{
    bn::optional<scene_type> result;
    bool update_again = false;

    if(_fade_in_frames)
    {
        --_fade_in_frames;

        bn::fixed fade_intensity = bn::fixed(_fade_in_frames) / fade_in_frames;
        bn::bg_palettes::set_fade(bn::colors::black, fade_intensity);
        bn::sprite_palettes::set_fade(bn::colors::black, fade_intensity);
    }
    else if(_next_scene)
    {
        --_fade_out_frames;

        if(_fade_out_frames > 0)
        {
            bn::fixed fade_intensity = 1 - (bn::fixed(_fade_out_frames) / fade_out_frames);
            bn::bg_palettes::set_fade(bn::colors::black, fade_intensity);
            bn::sprite_palettes::set_fade(bn::colors::black, fade_intensity);

            if(_game_over_scene)
            {
                _game_over_scene->update_gfx();
            }
        }
        else
        {
            result = _next_scene;
        }
    }
    else if(_game_over_scene)
    {
        _next_scene = _game_over_scene->update();

        if(_next_scene)
        {
            _fade_out_frames = fade_out_frames;
        }
    }
    else
    {
        bool exit = false;

        if(_pause.update(exit))
        {
            if(exit)
            {
                _next_scene = scene_type::TITLE;
                _fade_out_frames = fade_out_frames;
            }
        }
        else
        {
            _updates += _music_tempo - 1;
            update_again = _updates >= 1;

            if(update_again)
            {
                _updates -= 1;
            }

            if(_playing)
            {
                _data.pending_frames = _pending_frames;
                --_pending_frames;

                _update_play();
            }
            else
            {
                _data.pending_frames = 0;

                if(_update_fade(update_again))
                {
                    _game_over_scene.reset(new game_over_scene(_completed_games, _core));
                }
            }

            _title.update(_total_frames);
            _timer.update(_pending_frames, _total_frames);
            _update_volume_dec();
        }
    }

    if(! _pause.paused() && ! _game_over_scene)
    {
        _backdrop.update(_core);

        if(update_again)
        {
            _backdrop.update(_core);
        }
    }

    return result;
}

void game_scene::_update_play()
{
    game& game = _game_manager->game();
    game_result game_result = game.play(_data);
    _playing = ! game_result.exit && _pending_frames;

    if(! _playing || game_result.remove_title)
    {
        _title.clear();
    }

    if(! _playing)
    {
        _completed_games = bn::min(_completed_games + 1, 998);
        _first_game_played = true;
        _victory = game.victory();

        if(bn::music::playing())
        {
            _music_volume_dec = bn::music::volume() / volume_dec_frames;
        }

        if(bn::dmg_music::playing())
        {
            _dmg_music_left_volume_dec = bn::dmg_music::left_volume() / volume_dec_frames;
            _dmg_music_right_volume_dec = bn::dmg_music::right_volume() / volume_dec_frames;
        }

        if(! _victory)
        {
            _lives.decrease();
        }
    }
}

bool game_scene::_update_fade(bool update_again)
{
    bool exit = false;
    _lives.update();

    if(update_again)
    {
        _lives.update();
    }

    if(_result_animation)
    {
        bool active = _result_animation->update();

        if(active && update_again)
        {
            active = _result_animation->update();
        }

        if(! active)
        {
            bool big_pumpkin_visible = true;
            _result_animation.reset();
            _lives.stop();

            if(_lives.left())
            {
                if(_completed_games % game::games_per_speed_inc == 0)
                {
                    _music_tempo = game::recommended_music_tempo(_completed_games, _data);
                    _speed_inc_animation = game_result_animation::create_speed_inc();
                    _lives.look_down();
                    big_pumpkin_visible = false;
                }
                else
                {
                    _next_game_transition.emplace(_completed_games);
                }
            }

            _big_pumpkin->set_visible(big_pumpkin_visible);
        }
    }
    else if(_speed_inc_animation)
    {
        if(! _speed_inc_animation->update())
        {
            _speed_inc_animation.reset();
            _big_pumpkin->set_visible(true);
            _next_game_transition.emplace(_completed_games);
        }
    }
    else if(_next_game_transition)
    {
        bool active = _next_game_transition->update();

        if(active && update_again)
        {
            active = _next_game_transition->update();
        }

        if(! active)
        {
            _next_game_transition.reset();
        }
    }
    else if(_big_pumpkin_counter)
    {
        --_big_pumpkin_counter;
    }
    else
    {
        if(_big_pumpkin_inc)
        {
            if(_big_pumpkin_stage < 13)
            {
                ++_big_pumpkin_stage;
            }
            else
            {
                --_big_pumpkin_stage;
                _big_pumpkin_inc = false;
            }
        }
        else
        {
            if(_big_pumpkin_stage > 1)
            {
                --_big_pumpkin_stage;
            }
            else
            {
                if(_lives.left())
                {
                    ++_big_pumpkin_stage;
                    _big_pumpkin_inc = true;
                }
                else
                {
                    exit = true;
                }
            }
        }

        const bn::regular_bg_item* bg_item = nullptr;
        _big_pumpkin_counter = 2;

        switch(_big_pumpkin_stage)
        {

        case 1:
            bg_item = &bn::regular_bg_items::mj_big_pumpkin_1;
            _big_pumpkin_counter = 0;

            if(! exit)
            {
                if(_first_game_played)
                {
                    _result_animation = game_result_animation::create(_completed_games, _victory);
                }
                else
                {
                    _next_game_transition.emplace(_completed_games);
                }
            }
            break;

        case 2:
            bg_item = &bn::regular_bg_items::mj_big_pumpkin_2;

            if(_big_pumpkin_inc)
            {
                _backdrop.fade_out();
                _lives.hide();
            }
            break;

        case 3:
            bg_item = &bn::regular_bg_items::mj_big_pumpkin_3;
            break;

        case 4:
            bg_item = &bn::regular_bg_items::mj_big_pumpkin_4;

            if(! _big_pumpkin_inc)
            {
                _game_manager.reset();
                _backdrop.fade_in();
                _lives.show(_victory, ! _victory);
                _pending_frames = 0;
                _total_frames = 1;
            }
            break;

        case 5:
            bg_item = &bn::regular_bg_items::mj_big_pumpkin_5;

            if(_big_pumpkin_inc)
            {
                game_manager& game_manager = _game_manager.emplace(_completed_games, _data, _core);
                int total_frames = game_manager.game().total_frames();
                BN_ASSERT(total_frames >= game::minimum_frames && total_frames <= game::maximum_frames,
                          "Invalid game total frames: ", total_frames);

                _pending_frames = total_frames;
                _total_frames = total_frames;
            }
            break;

        case 6:
            bg_item = &bn::regular_bg_items::mj_big_pumpkin_6;

            if(_big_pumpkin_inc)
            {
                _title.show(_game_manager->game().title(), _core);
            }
            break;

        case 7:
            bg_item = &bn::regular_bg_items::mj_big_pumpkin_7;
            break;

        case 8:
            bg_item = &bn::regular_bg_items::mj_big_pumpkin_8;
            break;

        case 9:
            bg_item = &bn::regular_bg_items::mj_big_pumpkin_9;
            break;

        case 10:
            bg_item = &bn::regular_bg_items::mj_big_pumpkin_10;
            break;

        case 11:
            bg_item = &bn::regular_bg_items::mj_big_pumpkin_11;
            break;

        case 12:
            bg_item = &bn::regular_bg_items::mj_big_pumpkin_12;
            break;

        default:
            if(_big_pumpkin_inc)
            {
                _playing = true;
                _big_pumpkin_counter = 0;
            }
            else
            {
                _big_pumpkin_counter = 60;
            }
            break;
        }

        _update_big_pumpkin(bg_item);
    }

    if(! _playing)
    {
        if(_game_manager)
        {
            game& game = _game_manager->game();

            if(_big_pumpkin_inc)
            {
                game.fade_in(_data);
            }
            else
            {
                game.fade_out(_data);

                if(_pending_frames)
                {
                    --_pending_frames;
                }
            }
        }
    }

    return exit;
}

void game_scene::_update_big_pumpkin(const bn::regular_bg_item* bg_item)
{
    if(bg_item)
    {
        if(_big_pumpkin)
        {
            _big_pumpkin->set_item(*bg_item);
        }
        else
        {
            bn::regular_bg_ptr big_pumpkin = bg_item->create_bg(0, (256 - 160) / 2);
            big_pumpkin.set_priority(0);
            _big_pumpkin = bn::move(big_pumpkin);
        }

        _big_pumpkin->set_visible(! _result_animation);
    }
    else
    {
        _big_pumpkin.reset();
    }
}

void game_scene::_update_volume_dec()
{
    if(_music_volume_dec > 0)
    {
        if(bn::music::playing())
        {
            bn::fixed volume = bn::music::volume() - _music_volume_dec;

            if(volume > 0)
            {
                bn::music::set_volume(volume);
            }
            else
            {
                bn::music::stop();
                _music_volume_dec = 0;
            }
        }
        else
        {
            _music_volume_dec = 0;
        }
    }

    if(_dmg_music_left_volume_dec > 0 || _dmg_music_right_volume_dec > 0)
    {
        bn::optional<bn::dmg_music_item> dmg_music_item = bn::dmg_music::playing_item();

        if(dmg_music_item && dmg_music_item->type() == bn::dmg_music_type::GBT_PLAYER)
        {
            bn::fixed left_volume = bn::dmg_music::left_volume() - _dmg_music_left_volume_dec;
            bn::fixed right_volume = bn::dmg_music::right_volume() - _dmg_music_right_volume_dec;

            if(left_volume > 0 && right_volume > 0)
            {
                bn::dmg_music::set_left_volume(left_volume);
                bn::dmg_music::set_right_volume(right_volume);
            }
            else
            {
                bn::dmg_music::stop();
                _dmg_music_left_volume_dec = 0;
                _dmg_music_right_volume_dec = 0;
            }
        }
        else
        {
            _dmg_music_left_volume_dec = 0;
            _dmg_music_right_volume_dec = 0;
        }
    }
}

}
