#include "awa_Game_01.h"

#include "bn_keypad.h"
#include "bn_math.h"

#include "mj/mj_game_list.h"

#include "bn_regular_bg_items_awa_background.h"
#include "bn_sprite_items_awa_apple.h"
#include "bn_sprite_items_awa_player.h"
#include "bn_sprite_items_awa_o.h"
#include "bn_sprite_items_awa_x.h"

namespace
{
    constexpr bn::string_view code_credits[] = { "installationwizard" };
    constexpr bn::string_view graphics_credits[] = { "FunfettiPhantom", "installationwizard" };
}

// MJ_GAME_LIST_ADD(awa::Game01)
MJ_GAME_LIST_ADD_CODE_CREDITS(code_credits)
MJ_GAME_LIST_ADD_GRAPHICS_CREDITS(graphics_credits)

namespace awa{
    //this class header...
    Game01::Game01(int completed_games, const mj::game_data& data) :
     _bg(bn::regular_bg_items::awa_background.create_bg((256 - 240) / 2, (256 - 160) / 2)),
     _apple_sprite_0(bn::sprite_items::awa_apple.create_sprite(0,0)),
     _apple_sprite_1(bn::sprite_items::awa_apple.create_sprite(20,20)),
     _apple_sprite_2(bn::sprite_items::awa_apple.create_sprite(-20,-20)),
     _player_sprite(bn::sprite_items::awa_player.create_sprite(40,40)),
     _vic_sprite(bn::sprite_items::awa_o.create_sprite(0,0)),
     _def_sprite(bn::sprite_items::awa_x.create_sprite(0,0)),
     _apple_anim_0(bn::create_sprite_animate_action_forever(_apple_sprite_0,10,bn::sprite_items::awa_apple.tiles_item(), 0, 1, 2, 3)),
     _apple_anim_1(bn::create_sprite_animate_action_forever(_apple_sprite_1,10,bn::sprite_items::awa_apple.tiles_item(), 0, 1, 2, 3)),
     _apple_anim_2(bn::create_sprite_animate_action_forever(_apple_sprite_2,10,bn::sprite_items::awa_apple.tiles_item(), 0, 1, 2, 3)),
     _apple_moveby_0(bn::sprite_move_by_action(_apple_sprite_0,1,1)),
     _apple_moveby_1(bn::sprite_move_by_action(_apple_sprite_1,0,1)),
     _apple_moveby_2(bn::sprite_move_by_action(_apple_sprite_2,-2,-1)){
        _total_frames = play_jingle(mj::game_jingle_type::EXELOTL02A, completed_games, data);
        _tempo = recommended_music_tempo(completed_games, data);
        _show_result_frames = (60 / _tempo).right_shift_integer();

        _bg.set_priority(1);
        _vic_sprite.set_bg_priority(2);
        _def_sprite.set_bg_priority(2);
        _apple_sprite_0.set_bg_priority(1);
        _apple_sprite_1.set_bg_priority(1);
        _apple_sprite_2.set_bg_priority(1);
        _player_sprite.set_bg_priority(1);
    }

    void Game01::fade_in([[maybe_unused]] const mj::game_data& data){
        return;
    }

    void Game01::fade_out([[maybe_unused]] const mj::game_data& data){
        return;
    }

    mj::game_result Game01::play(const mj::game_data& data){
        mj::game_result result;
        result.exit = data.pending_frames == 0;

        _updates += _tempo - 1;
        _play_impl(result);

        if(! result.exit && _updates >= 1)
        {
            _updates -= 1;
            _play_impl(result);
        }

        return result;
    }

    /* Function for determining whether passed position is in the bounds of the playing field
     * @param x_pos The x position of the point
     * @param y_pos The y position of the point
     * @return True if the point is in bounds, False if out-of-bounds
     */ 
    bool Game01::bounds_check(bn::fixed x_pos, bn::fixed y_pos){
        if(estimate_hypoteneuse(bn::abs(x_pos),bn::abs(y_pos))<=_BARREL_RADIUS)
            return true;
        else
            return false;
    }

    /* Function for determining whether passed position is in the bounds of the playing field
     * @param player The position of the player character
     * @param apple The position of the apple to be checked
     * @return True if the player is colliding with the apple, False if not
     */ 
    bool Game01::is_colliding_apple(bn::fixed_point player, bn::fixed_point apple){
        bn::fixed x = player.x() - apple.x();
        bn::fixed y = player.y() - apple.y();

        if(estimate_hypoteneuse(x,y) <= _APPLE_RADIUS)
            return true;
        else
            return false;
    }

    /* Function for approximating the hypoteneuse of a right triangle with integer precision
     * @param a The first leg of the right triangle
     * @param b The second leg of the right triangle
     * @return An integer approximation of the hypoteneuse of the right triangle
     */ 
    int Game01::estimate_hypoteneuse(bn::fixed a, bn::fixed b){
        return bn::sqrt(a*a+b*b).ceil_integer();
    }

    /* Function for rotating a vector by approximately 120 degrees
     * @param direction The direction vector to rotate
     * @return The rotated vector
     */ 
    bn::fixed_point Game01::rotate_vector(bn::fixed_point direction){
        bn::fixed_point rotated = bn::fixed_point(0,0);
        rotated.set_x(-0.5*direction.x()-0.866*direction.y());
        rotated.set_y(.866*direction.x()-.5*direction.y());
                
        return rotated;
    }

    //Function for updating looping sprite animations
    void Game01::update_animations(){
        _apple_anim_0.update();
        _apple_anim_1.update();
        _apple_anim_2.update();
    }

    /* Function for updating positions of apples as they move around the play field
     * Determines whether they need their direction vectors updated or if they can simply be moved
     * updates direction vector if needed then moves apples
     * @param apple The apple to update
     * @param apple_moveby the corresponding action defining the movement of the apple
     */ 
    void Game01::update_apple_position(bn::sprite_ptr& apple, bn::sprite_move_by_action& apple_moveby){
        bn::fixed_point vec = apple_moveby.delta_position();
        bn::fixed_point apple_position = apple.position();
        if(!bounds_check(apple_position.x()+vec.x(),apple_position.y()+vec.y())){
            vec = rotate_vector(vec);
            apple_moveby = bn::sprite_move_by_action(apple,vec.x().floor_integer(),vec.y().floor_integer());
        }
        apple_moveby.update();
    }

    void Game01::_play_impl(mj::game_result& result)
    {
        if(! _victory && ! _defeat){
            //determining if the player hit an apple or not
            if(bn::keypad::a_pressed()){
                bn::fixed_point player_pos = _player_sprite.position();

                if(is_colliding_apple(player_pos,_apple_sprite_0.position())
                    ||is_colliding_apple(player_pos,_apple_sprite_1.position())
                    ||is_colliding_apple(player_pos,_apple_sprite_2.position())){
                    _vic_sprite.set_bg_priority(1);
                    result.remove_title = true;
                    _victory = true;
                }else{
                    _def_sprite.set_bg_priority(1);
                    result.remove_title = true;
                    _defeat = true;
                }
            }else{
                update_animations();
                update_apple_position(_apple_sprite_0,_apple_moveby_0);
                update_apple_position(_apple_sprite_1,_apple_moveby_1);
                update_apple_position(_apple_sprite_2,_apple_moveby_2);

                //this cluster controls player movement
                if(bn::keypad::left_held()&&bounds_check(_player_sprite.x()-1,_player_sprite.y())){
                    _player_sprite.set_x(_player_sprite.x() - 1);
                }else if(bn::keypad::right_held()&&bounds_check(_player_sprite.x()+1,_player_sprite.y())){
                    _player_sprite.set_x(_player_sprite.x() + 1);
                }
                if(bn::keypad::up_held()&&bounds_check(_player_sprite.x(),_player_sprite.y()-1)){
                    _player_sprite.set_y(_player_sprite.y() - 1);
                }else if(bn::keypad::down_held()&&bounds_check(_player_sprite.x(),_player_sprite.y()+1)){
                    _player_sprite.set_y(_player_sprite.y() + 1);
                }
            }
        }else{
            if(_show_result_frames){
                --_show_result_frames;
            }else{
                result.exit = true;
            }
        }
    }
}
