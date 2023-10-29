#include "hh_mystic_mansion.h"

#include "mj/mj_game_list.h"

#include "bn_regular_bg_items_hh_black_bg.h"
#include "bn_regular_bg_items_hh_gymnasium.h"
#include "bn_regular_bg_items_hh_basement.h"
#include "bn_sprite_items_hh_peepantsometer.h"
#include "bn_sprite_items_hh_peebar.h"
#include "bn_sprite_items_hh_lightbulb.h"
#include "bn_affine_bg_items_hh_radiance.h"
#include "bn_sprite_items_hh_boo.h"
#include "bn_sprite_items_hh_fat_guy_overalls.h"
#include "bn_sprite_items_hh_fat_guy_devil.h"
#include "bn_sprite_items_hh_fat_guy_vampire.h"
#include "bn_rect_window.h"
#include "bn_blending.h"
#include "bn_sound.h"
#include "bn_sound_items.h"

namespace
{
    constexpr bn::string_view code_credits[] = { "Jeremy Elkayam" };
    constexpr bn::string_view graphics_credits[] = { "Jeremy Elkayam", "Michael Elkayam" };
    constexpr bn::string_view sfx_credits[] = { "Michael Elkayam", "3Maze", "Soundspark LLC", "Studio 23" };
}

// MJ_GAME_LIST_ADD(hh::mystic_mansion)
MJ_GAME_LIST_ADD_CODE_CREDITS(code_credits)
MJ_GAME_LIST_ADD_GRAPHICS_CREDITS(graphics_credits)
// MJ_GAME_LIST_ADD_MUSIC_CREDITS(music_credits)
MJ_GAME_LIST_ADD_SFX_CREDITS(sfx_credits)

namespace hh
{

mystic_mansion::mystic_mansion(int completed_games, const mj::game_data& data) :
    _blackbg(bn::regular_bg_items::hh_black_bg.create_bg((256 - 240) / 2, (256 - 160) / 2)),
    _room(bn::regular_bg_items::hh_gymnasium.create_bg((256 - 240) / 2, (256 - 160) / 2)),
    _tempo(recommended_music_tempo(completed_games, data)),
    _total_frames((bn::fixed(360) / _tempo).round_integer()),
    _game_end_frame((_total_frames * bn::fixed(0.3)).round_integer()),
    _lights_on_end_frame((_total_frames * bn::fixed(0.25)).round_integer()),
    _lightbulb_appear_frame((_total_frames * bn::fixed(0.4)).round_integer()),
    _player(0,0, _tempo),
    _peepantsometer(bn::sprite_items::hh_peepantsometer.create_sprite(-90,30)),
    _fat_guy(bn::sprite_items::hh_fat_guy_devil.create_sprite(data.random.get_int(-110, 90),data.random.get_int(-60, 70))),
    _speech_bubble(bn::sprite_items::hh_boo.create_sprite(_fat_guy.x() + 24,_fat_guy.y() - 24)),
    _ambient_sound_timer(90),
    _explosion(_blackbg, 10),
    _difficulty_level(recommended_difficulty_level(completed_games, data))
{
    if(data.random.get_int(2)){
        _room.set_item(bn::regular_bg_items::hh_basement);
    }
    uint8_t costume = data.random.get_int(3);
    if(costume == 0){
        _fat_guy.set_item(bn::sprite_items::hh_fat_guy_overalls);
    }if(costume == 1){
        _fat_guy.set_item(bn::sprite_items::hh_fat_guy_vampire);
    }
    _fat_guy.set_visible(false);
    _speech_bubble.set_visible(false);

    bn::rect_window::internal().set_bottom_right(0,0);
    bn::rect_window::internal().set_top_left(0,0);
    bn::rect_window::internal().set_show_all();
    bn::window::outside().set_show_bg(_room, false);
 
    spawn_enemy(data, data.random.get_int(0, 3));

    if(_difficulty_level == mj::difficulty_level::NORMAL || _difficulty_level == mj::difficulty_level::HARD){
        
        bool enemy_type = data.random.get_int(2);
        if(_spider){
            if(enemy_type) {
                spawn_enemy(data, 1);
            }else{
                spawn_enemy(data, 2);
            }
        }else if(_ghost){
            if(enemy_type) {
                spawn_enemy(data, 0);
            }else{
                spawn_enemy(data, 1);
            }
        }else if (_bat){
            if(enemy_type) {
                spawn_enemy(data, 0);
            }else{
                spawn_enemy(data, 2);
            }
        }
        
    }
    if(_difficulty_level == mj::difficulty_level::HARD){
        if(!_ghost){
            spawn_enemy(data, 2);
        }else if(!_spider){
            spawn_enemy(data, 0);
        }else if(!_bat){
            spawn_enemy(data, 1);
        }
    }

    _ambient_sounds.emplace_back(bn::sound_items::hh_ominous_0);
    _ambient_sounds.emplace_back(bn::sound_items::hh_ominous_1);
    _ambient_sounds.emplace_back(bn::sound_items::hh_ominous_2);
    _ambient_sounds.emplace_back(bn::sound_items::hh_ominous_3);
    _ambient_sounds.emplace_back(bn::sound_items::hh_ominous_4);
    _ambient_sounds.emplace_back(bn::sound_items::hh_ominous_5);
    _ambient_sounds.emplace_back(bn::sound_items::hh_ominous_6);
    _ambient_sounds.emplace_back(bn::sound_items::hh_ominous_7);
    _ambient_sounds.emplace_back(bn::sound_items::hh_ominous_8);


    _peepantsometer.set_scale(2);
    _peepantsometer.set_visible(false);
}

void mystic_mansion::spawn_enemy(const mj::game_data& data, uint8_t enemy_type){

    int8_t xquad = data.random.get_int(2) == 1 ? 1 : -1;
    int8_t yquad = data.random.get_int(2) == 1 ? 1 : -1;
    bn::fixed xcor;
    bn::fixed ycor;
    xcor = data.random.get_fixed(50, 110) * xquad;
    ycor = yquad == 1 ? data.random.get_fixed(20, 60) : 
                                  data.random.get_fixed(-75, -20);
    uint8_t direction = data.random.get_int(8);

    bn::fixed speedup_factor = 1.0;
    switch(_difficulty_level){
        case mj::difficulty_level::EASY:
            break;
        case mj::difficulty_level::NORMAL:
            speedup_factor = data.random.get_fixed(1.0, 1.1);
            break;
        case mj::difficulty_level::HARD:
            speedup_factor = data.random.get_fixed(1.1, 1.3);
            break;
        default:
            break;
    }    
    if(enemy_type == 0){
        BN_ASSERT(!_spider, "to spawn a spider you can't have one already");

        _spider.emplace(xcor, ycor, _tempo * speedup_factor);
    }else if(enemy_type == 1){
        BN_ASSERT(!_bat, "to spawn a bat you can't have one already");

        //custom spawn generator to make sure he starts far away from you
        xcor = data.random.get_fixed(80, 110) * xquad;
        ycor = yquad == 1 ? data.random.get_fixed(50, 60) : 
                                    data.random.get_fixed(-75, -45);

        _bat.emplace(xcor, ycor,direction, _tempo * speedup_factor, data.random);
    }else if(enemy_type == 2){
        BN_ASSERT(!_ghost, "to spawn a ghost you can't have one already");

        //this block of code makes it so that the enemy is pointing at the player
        // ycor = _player.pos().y();
        // xcor = _player.pos().x();

        // if(direction == 7 || direction == 0 || direction == 1) xcor += 50;
        // if(direction == 1 || direction == 2 || direction == 3) ycor += 50;
        // if(direction == 3 || direction == 4 || direction == 5) xcor -= 50;
        // if(direction == 5 || direction == 6 || direction == 7) ycor -= 50;

        // //random variance
        // ycor += data.random.get_fixed(-10, 10);
        // xcor += data.random.get_fixed(-10, 10);        
        
        //ghosts now only move diagonally so it is harder
        if(direction % 2 == 0) direction = (direction + 1) % 8;


        //the random int just means a 1/4 chance of being true
        _ghost.emplace(xcor, ycor,direction, _tempo * speedup_factor, !data.random.get_int(4));
    }else{
        BN_ERROR("buddy you fucked up");
    }    
}

void mystic_mansion::fade_in([[maybe_unused]] const mj::game_data& data)
{
    // if there is going to be a fade in sound, please remove this...
    bn::sound_items::hh_pipe_organ.play(0.5);
}

mj::game_result mystic_mansion::play(const mj::game_data& data)
{
    mj::game_result result;

    if(! _defeat)
    {

        _player.take_button_input();
        if(_spider){
            _spider->point_at(_player.pos());
            _spider->update(data);

        }
        if(_bat) _bat->update(data);
        if(_ghost) _ghost->update(data);
        if(_lightbulb) _lightbulb->update();

        if((_spider && _player.hitbox().intersects(_spider->hitbox())) || 
            (_bat && _player.hitbox().intersects(_bat->hitbox())) || 
            (_ghost && _player.hitbox().intersects(_ghost->hitbox()))){
            _defeat = true;
            result.remove_title = true;
            _player.disable_movement();
            bn::sound_items::hh_waves.play(1);
        }

        if(data.pending_frames == _lightbulb_appear_frame){
            _lightbulb.emplace(_lightbulb_appear_frame - _game_end_frame);
        }

        if(data.pending_frames >= _lightbulb_appear_frame){
            if(_ambient_sound_timer){
                --_ambient_sound_timer;
            }else{
                _ambient_sounds.at(data.random.get_int(_ambient_sounds.size())).play(1);
                _ambient_sound_timer = data.random.get_int(60, 120);
            }
        }

        if(data.pending_frames == _game_end_frame){
            //you won
            _victory = true;

            for(entity *e : all_entities()){
                e->disable_movement();
            }
            bool intersection;
            do{
                intersection = false;
                for(entity *e : all_entities()){
                    bn::fixed_rect hitbox = e->hitbox();
                    hitbox.set_height(hitbox.height() + 30);
                    hitbox.set_width(hitbox.width() + 20);
                    intersection = intersection || hitbox.contains(_fat_guy.position())
                                                || hitbox.contains(_speech_bubble.position());
                }                
                if(intersection){
                    _fat_guy.set_position(data.random.get_int(-110, 90),data.random.get_int(-60, 70));
                    _speech_bubble.set_position(_fat_guy.x() + 24,_fat_guy.y() - 24);
                }
            }while(intersection);
        }
        if(data.pending_frames < _game_end_frame && data.pending_frames >= _lights_on_end_frame){
            bn::fixed window_scale_factor = bn::fixed(_game_end_frame - data.pending_frames) / bn::fixed(_game_end_frame - _lights_on_end_frame);
            bn::rect_window iw = bn::rect_window::internal();
            iw.set_left(-120 * window_scale_factor);
            iw.set_right(120 * window_scale_factor);
            iw.set_top( - 80 * window_scale_factor);
            iw.set_bottom(80 * window_scale_factor);

            //todo: turn this into a loop thru all entities

            for(entity *e : all_entities()){
                if(e->hitbox().intersects(iw.boundaries())){
                    //show your body when you are illuminated by the light ... 
                    e->lights_on(data.random);
                }
            }

            if(iw.boundaries().contains(_speech_bubble.position())){
                _speech_bubble.set_visible(true);
            }
            if(iw.boundaries().contains(_fat_guy.position())){
                _fat_guy.set_visible(true);
            }
        }
    }
    else
    {
        //only happens if you lose
        if(_show_result_frames)
        {
            if(!_peepantsometer.visible()){
                _peepantsometer.set_visible(true);
                if(_player.pos().x() <= _peepantsometer.x() + 30 && 
                   _player.pos().x() >= _peepantsometer.x() - 30){
                    _peepantsometer.set_x(-_peepantsometer.x());
                }
            }
            --_show_result_frames;
            if(_show_result_frames % 2 == 0){
                //rotate eyeballs every other frame
                _player.rotate_eyes();
            }

            //fill the peepantsometer every third frame
            if(!_pee_bars.full() && _show_result_frames % 3 == 0){
                bn::fixed xcor;
                bn::fixed ycor;
                if(_pee_bars.size() < 6){
                    xcor = _peepantsometer.x() - 12;
                    ycor = _peepantsometer.y() + 28 - (8 * _pee_bars.size());
                }else if(_pee_bars.size() < 12){
                    xcor = _peepantsometer.x() + 12;
                    ycor = _peepantsometer.y() + 28 - (8 * (_pee_bars.size() - 6));
                }else{
                    xcor = _peepantsometer.x();
                    ycor = _peepantsometer.y() + 28 - (8 * (_pee_bars.size() - 8));
                }
                _pee_bars.emplace_back(bn::sprite_items::hh_peebar.create_sprite(xcor, ycor));
                _pee_bars.back().set_scale(2);
            }
            _explosion.update();


        }
        else
        {
            result.exit = true;
        }
    }

    _player.update(data);


    return result;
}

void mystic_mansion::fade_out([[maybe_unused]] const mj::game_data& data)
{
    if(_lightbulb){
        _lightbulb->hide_radiance();
    }
}

bn::vector<entity*, 4> mystic_mansion::all_entities(){
    bn::vector<entity*, 4> result;
    result.emplace_back(&_player);
    if(_spider) result.emplace_back(_spider.get());
    if(_bat) result.emplace_back(_bat.get());
    if(_ghost) result.emplace_back(_ghost.get());

    return result;
}

explosion::explosion(bn::regular_bg_ptr &bg, uint8_t fpc) : 
    _blackbg(bg),
    _fade(bn::bg_palette_fade_to_action(bg.palette(), fpc, 1)){
        //can probably do this with palette rotation actually

    bn::bg_palette_ptr palette = _blackbg.palette();
    palette.set_rotate_count(-1);
    
    reset_fade();
}

void explosion::update(){
    _fade.update();
    if(_fade.done()){
        reset_fade();
    }
}

//this fades between the 16 colors in the bg palette as it simultaneously rotates 
//between them.. basically giving us an atari explosion effect
void explosion::reset_fade(){
    bn::bg_palette_ptr palette = _blackbg.palette();
    uint8_t next_rotation = (palette.rotate_count() + 1) % palette.colors_count();
    uint8_t next_fade = palette.colors_count() - (next_rotation + 1);
    palette.set_fade_intensity(0);
    palette.set_fade_color(palette.colors().at(next_fade));
    palette.set_rotate_count(next_rotation);

    _fade.reset();
}

lightbulb::lightbulb(uint8_t descent_frames) : 
    _bulb(bn::sprite_items::hh_lightbulb.create_sprite(0, -80 - 32)),
    _radiance(bn::affine_bg_items::hh_radiance.create_bg(0, -80 + 48)),
    _descent_frames(descent_frames),
    _total_descent_frames(descent_frames)
    // _swing_time(120),
    // _timer(_swing_time)
     {
    _radiance.set_visible(false);
    _radiance.set_blending_enabled(true);
    _radiance.set_priority(0);
    _radiance.set_wrapping_enabled(false);
    _radiance.set_scale(0.6);

    _radiance_scale = bn::affine_bg_scale_loop_action(_radiance, 30, 0.8);
    bn::blending::set_transparency_alpha(0.3);
    bn::sound::stop_all();
}

void lightbulb::update(){

    if(_descent_frames){
        bn::fixed dist = bn::fixed(64) * bn::fixed(_descent_frames) / bn::fixed(_total_descent_frames);
        _bulb.set_y(-80 -32 + (64 - dist));

        --_descent_frames;

        if(_descent_frames == 0){
            bn::sound_items::hh_switch_on.play(1);
            bn::sound_items::hh_ambient.play(0.25);
        }

    }else{
        _bulb.set_item(bn::sprite_items::hh_lightbulb, 1);
        _radiance.set_visible(true);
    }
    if(_radiance.visible()){
        _radiance_scale->update();
    }
}

}
