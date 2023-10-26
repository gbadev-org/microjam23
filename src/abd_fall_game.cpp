#include "abd_fall_game.h"

#include "bn_keypad.h"
#include "bn_sprite_ptr.h"
#include "bn_sprite_animate_actions.h"

#include "mj/mj_game_list.h"

#include "bn_sound_items.h"

#include "bn_regular_bg_items_abd_bg.h"
#include "bn_sprite_items_mj_small_pumpkin.h"
#include "bn_sprite_items_abd_player_lose.h"
#include "bn_sprite_items_abd_bowl.h"
#include "bn_sprite_items_abd_candy.h"

namespace
{
    constexpr bn::string_view code_credits[] = { "Ahmad Alabandi" };
    constexpr bn::string_view graphics_credits[] = { "Ahmad Alabandi" };
    constexpr bn::string_view sfx_credits[] = { "JDWasabi" };
}

// MJ_GAME_LIST_ADD(abd::abd_fall_game)
MJ_GAME_LIST_ADD_CODE_CREDITS(code_credits)
MJ_GAME_LIST_ADD_GRAPHICS_CREDITS(graphics_credits)
// MJ_GAME_LIST_ADD_MUSIC_CREDITS(music_credits)
MJ_GAME_LIST_ADD_SFX_CREDITS(sfx_credits)

namespace abd
{

// abd::abd_player player;

abd_fall_game::abd_fall_game(int completed_games, const mj::game_data& data) :
    _bg(bn::regular_bg_items::abd_bg.create_bg((256 - 240) / 2, (256 - 160) / 2)),
    _playerBowlSprite(bn::sprite_items::abd_bowl.create_sprite(0,43)),
    _candySprite(bn::sprite_items::abd_candy.create_sprite(0,-150)),
    _total_frames(play_jingle(mj::game_jingle_type::TOTSNUK05, completed_games, data))
{
    constexpr int frames_diff = maximum_frames - minimum_frames;
    constexpr int maximum_speed_completed_games = 30;

    completed_games = bn::min(completed_games, maximum_speed_completed_games);

    int frames_reduction = (frames_diff * completed_games) / maximum_speed_completed_games;
    _total_frames = maximum_frames - frames_reduction;
    _total_frames -= data.random.get_int(60);
    _total_frames = bn::clamp(_total_frames, minimum_frames, maximum_frames);

    gameSpeed = 1 + completed_games*1.0/maximum_speed_completed_games;
    if(gameSpeed > 2){
        gameSpeed = 2;
    }

}

void abd_fall_game::fade_in([[maybe_unused]] const mj::game_data& data)
{
   _playerSprite.set_position(playerX,playerY);
   setUpCandy(data);
}

mj::game_result abd_fall_game::play(const mj::game_data& data)
{
    mj::game_result result;
    result.exit = data.pending_frames == 0;

    if(! _victory && ! _defeat)
    {
        if(bn::keypad::right_held()){
            if(playerX < 80){
                playerX += playerMoveSpeed * gameSpeed;
                bowlangle += 1.0 * gameSpeed * candiesCollected *1.0/ 2.0;
                if(bn::keypad::right_pressed()){
                    _playerSprite.set_horizontal_flip(true);
                    _playerSpriteMoveAction = bn::create_sprite_animate_action_forever(
                        _playerSprite, 4, bn::sprite_items::mj_small_pumpkin.tiles_item(), 4, 3, 4, 5);
                }
            }
            else{
                playerX = 80;
            }
        }
        else if(bn::keypad::left_held()){
            if(playerX > -80){
                playerX -= playerMoveSpeed * gameSpeed;
                bowlangle -= 1.0 * gameSpeed * candiesCollected *1.0/ 2.0;
                if(bn::keypad::left_pressed()){
                    _playerSprite.set_horizontal_flip(false);
                    _playerSpriteMoveAction = bn::create_sprite_animate_action_forever(
                        _playerSprite, 4, bn::sprite_items::mj_small_pumpkin.tiles_item(), 4, 3, 4, 5);
                }
            }
            else{
                playerX = -80;
            }
        }
        else{
            if(bowlangle > 1){
                bowlangle -= 0.1;
            }
            else if(bowlangle < -1){
                bowlangle += 0.1;
            }
            else{
                bowlangle = 0;
            }

            _playerSpriteMoveAction = bn::create_sprite_animate_action_forever(
                _playerSprite, 8, bn::sprite_items::mj_small_pumpkin.tiles_item(), 6,7);
            
        }
        
        _playerSprite.set_position(playerX,playerY);
        _playerBowlSprite.set_position(playerX,playerY-7);
        bowlangle = bn::clamp(bowlangle.integer(),-40,40);
        _playerBowlSprite.set_rotation_angle(bowlangle >= 0 ? bowlangle : 360 + bowlangle);

        candyY += candyFallSpeed * gameSpeed;
        _candySprite.set_position(candyX,candyY);
        bn::fixed tempRotation = _candySprite.rotation_angle() + candyRotateDirection;
        _candySprite.set_rotation_angle(tempRotation > 360 ? tempRotation-360 : tempRotation);

        if(candyY < playerY + 6 && candyY > playerY - 16 && candyY < 70){
            if(candyX >= playerX - (playerCatchRange/2) && candyX <= playerX + (playerCatchRange/2)){ // candy collect
                candiesCollected++;
                if(candiesCollected >= 1){
                    if(candiesCollected >= 4){
                        _playerBowlSprite.set_tiles(bn::sprite_items::abd_bowl.tiles_item().create_tiles(2));
                    }
                    else{
                        _playerBowlSprite.set_tiles(bn::sprite_items::abd_bowl.tiles_item().create_tiles(1));
                    }
                }
                bn::sound_items::abd_collectcandy.play(1);
                setUpCandy(data);
            }
        }
        else if(candyY >= 70){ // player lose
            _playerSpriteMoveAction = bn::create_sprite_animate_action_forever(
                            _playerSprite, 10, bn::sprite_items::abd_player_lose.tiles_item(), 0, 1, 2);
            _playerBowlSprite.set_scale(0.001);
            bn::sound_items::abd_lose.play(1);
            result.remove_title = true;
            _defeat = true;
        }
        else if(data.pending_frames <= 2){ // player win
            _playerSpriteMoveAction = bn::create_sprite_animate_action_forever(
                            _playerSprite, 2, bn::sprite_items::mj_small_pumpkin.tiles_item(), 0, 3, 6, 9);
            bn::sound_items::abd_win.play(1);
            result.remove_title = true;
            _victory = true;
        }
        
    }
    else
    {
        if(_show_result_frames)
        {
            --_show_result_frames;
        }
        else
        {
            result.exit = true;
        }
    }

    _playerSpriteMoveAction.update();

    return result;
}

void abd_fall_game::fade_out([[maybe_unused]] const mj::game_data& data)
{
}

}
