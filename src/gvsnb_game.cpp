#include "gvsnb_game.h"

#include "bn_music.h"

#include "mj/mj_game_list.h"

#include "bn_music_items.h"
#include "bn_regular_bg_items_gvsnb_building.h"
#include "bn_sound_items.h"

namespace
{
    constexpr bn::string_view code_credits[] = { "GValiente" };
    constexpr bn::string_view graphics_credits[] = { "GValiente", "bevouliin.com" };
    constexpr bn::string_view music_credits[] = { "Nikku4211" };
    constexpr bn::string_view sfx_credits[] = { "Pixel-boy at Sparklin Labs" };
}

// MJ_GAME_LIST_ADD(gvsnb::game)
MJ_GAME_LIST_ADD_CODE_CREDITS(code_credits)
MJ_GAME_LIST_ADD_GRAPHICS_CREDITS(graphics_credits)
MJ_GAME_LIST_ADD_MUSIC_CREDITS(music_credits)
MJ_GAME_LIST_ADD_SFX_CREDITS(sfx_credits)

namespace gvsnb
{

game::game(int completed_games, const mj::game_data& data) :
    _building(bn::regular_bg_items::gvsnb_building.create_bg((256 - 240) / 2, (256 - 160) / 2)),
    _big_enemy(_enemy_gfx),
    _hand(_candy_gfx),
    _tempo(recommended_music_tempo(completed_games, data)),
    _total_frames(recommended_total_frames(game::maximum_frames, completed_games, data))
{
    play_music(bn::music_items::gvsnb_creppy, completed_games, data);
    bn::music::set_position(7);
}

void game::fade_in([[maybe_unused]] const mj::game_data& data)
{
    _update(false, data.random);
}

mj::game_result game::play(const mj::game_data& data)
{
    mj::game_result result;
    result.exit = data.pending_frames == 0;
    _update(true, data.random);

    if(_defeat_frames > 0)
    {
        _defeat_frames -= _tempo;

        if(_defeat_frames <= 0)
        {
            result.exit = true;
        }
    }

    return result;
}

void game::fade_out([[maybe_unused]] const mj::game_data& data)
{
    _update(false, data.random);
}

void game::_add_enemy(bn::random& random)
{
    bn::fixed tempo = _tempo;
    int difficulty_level = random.get_fixed(1, (tempo * 3) - 0.6).right_shift_integer();
    int door_level = random.get_fixed(1, tempo.unsafe_multiplication(bn::fixed(2.75))).right_shift_integer();
    door_level = bn::min(door_level, 3);

    bool right = random.get_int(2);

    if(_previous_enemy_difficulty_level[0] == _previous_enemy_difficulty_level[1] &&
        _previous_enemy_difficulty_level[1] == difficulty_level)
    {
        difficulty_level = difficulty_level == 1 ? 2 : difficulty_level - 1;
    }

    if(_previous_enemy_door_level[0] == _previous_enemy_door_level[1] &&
        _previous_enemy_door_level[1] == door_level)
    {
        door_level = door_level == 1 ? 2 : door_level - 1;
    }

    if(_previous_enemy_right[0] == _previous_enemy_right[1] && _previous_enemy_right[1] == right)
    {
        right = ! right;
    }

    _enemies.emplace_back(difficulty_level, door_level, right, tempo, _enemy_gfx, random);
    _new_enemy_frames += random.get_fixed(15, 25);

    _previous_enemy_difficulty_level[0] = _previous_enemy_difficulty_level[1];
    _previous_enemy_difficulty_level[1] = uint8_t(difficulty_level);

    _previous_enemy_door_level[0] = _previous_enemy_door_level[1];
    _previous_enemy_door_level[1] = uint8_t(door_level);

    _previous_enemy_right[0] = _previous_enemy_right[1];
    _previous_enemy_right[1] = right;
}

void game::_update(bool playing, bn::random& random)
{
    bn::fixed tempo = _tempo;
    _sky.update(tempo);

    if(playing)
    {
        if(_new_enemy_frames > 0 && ! _enemies.full())
        {
            _new_enemy_frames -= tempo;
        }

        if(_new_enemy_frames <= 0)
        {
            _add_enemy(random);
        }
    }

    if(_hand.update(tempo, playing && _victory))
    {
        candy new_candy = _hand.create_candy(_candy_gfx);

        if(! _candies.full())
        {
            _candies.push_back(bn::move(new_candy));
        }
    }

    bool enemy_hit = false;

    for(auto it = _candies.begin(), end = _candies.end(); it != end; )
    {
        candy& candy = *it;

        if(candy.update(tempo, _enemies, enemy_hit))
        {
            ++it;
        }
        else
        {
            it = _candies.erase(it);
        }
    }

    if(_hit_sound_cooldown_frames > 0)
    {
        _hit_sound_cooldown_frames -= tempo;
    }
    else if(enemy_hit)
    {
        bn::sound_items::gvsnb_hit.play(0.5, tempo, 0);
        _hit_sound_cooldown_frames = 16;
    }

    for(auto it = _enemies.begin(), end = _enemies.end(); it != end; )
    {
        enemy& enemy = *it;

        if(enemy.update(tempo, random))
        {
            enemy.draw(_enemy_gfx);
            ++it;
        }
        else
        {
            if(enemy.alive())
            {
                _big_enemy.setup(enemy, _enemy_gfx);
            }

            it = _enemies.erase(it);
        }
    }

    if(_victory && ! _big_enemy.update(tempo))
    {
        _victory = false;
        _defeat_frames = 2.5 * 60;

        if(bn::music::playing())
        {
            bn::music::set_position(3);
        }
    }
}

}
