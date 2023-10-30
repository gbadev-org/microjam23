#include "fez_game.h"

#include "bn_keypad.h"
#include "bn_math.h"
#include "bn_random.h"
#include "bn_display.h"
#include "bn_regular_bg_ptr.h"
#include "bn_sprite_animate_actions.h"
#include "bn_sound_actions.h"
#include "bn_sound_items.h"
#include "bn_sound.h"

#include "mj/mj_game_list.h"

#include "bn_regular_bg_items_fez_bg_stars.h"
#include "bn_regular_bg_items_fez_bg_speed.h"

#include "bn_sprite_items_fez_bat.h"
#include "bn_sprite_items_fez_moon.h"
#include "bn_sprite_items_fez_umbrella.h"
#include "bn_sprite_items_fez_stake.h"
#include "bn_sprite_items_fez_garlic.h"
#include "bn_sprite_items_fez_cross.h"

namespace
{
	constexpr bn::string_view code_credits[] = {"Fe26"};
    constexpr bn::string_view graphics_credits[] = {"Fe26"};
    constexpr bn::string_view music_credits[] = {"Fe26"};
	constexpr bn::string_view sfx_credits[] = {"Fe26"};
}

MJ_GAME_LIST_ADD(fez::game)
MJ_GAME_LIST_ADD_CODE_CREDITS(code_credits)
MJ_GAME_LIST_ADD_GRAPHICS_CREDITS(graphics_credits)
MJ_GAME_LIST_ADD_MUSIC_CREDITS(music_credits)
MJ_GAME_LIST_ADD_SFX_CREDITS(sfx_credits)

namespace fez
{	
	int rw = 240;
	int rh = 160;
	int cx = 120;
	int cy = 80;

	float xx = 0.0f;
	float yy = -rh + 32.0f;

	float g = 0.1f;

	float vx = 0.0f;
	float vy = 0.0f;
	float mpy = 0.0f;

	int oN = 4;
	int   oX[4] = {-256,    -256,    -256,    -256};
	float oY[4] = {-256.0f, -256.0f, -256.0f, -256.0f};
	float oS[4] = {0, 0, 0};

	bool end = false;

	int t = 0;

    game::game(int completed_games, const mj::game_data& data) :
        mj::game("fez"),
		_bg(bn::regular_bg_items::fez_bg_stars.create_bg((256 - 240) / 2, (256 - 160) / 2)), // (256 - 240) / 2, (256 - 160) / 2
		_bg_speed(bn::regular_bg_items::fez_bg_speed.create_bg((256 - 240) / 2, (256 - 160) / 2)), // Speed bg
		_moon0(bn::sprite_items::fez_moon.create_sprite(-31, -31)), // Moon
		_moon1(bn::sprite_items::fez_moon.create_sprite( 31,  31)), 
		_moon2(bn::sprite_items::fez_moon.create_sprite(-31,  31)),
		_moon3(bn::sprite_items::fez_moon.create_sprite( 31, -31)),
		_bat(bn::sprite_items::fez_bat.create_sprite(-256, -256)),  // Bat
		_umbrella(bn::sprite_items::fez_umbrella.create_sprite(-256, -256)),  // Umbrella
		_stake(bn::sprite_items::fez_stake.create_sprite(-256, -256)),  // Stake
		_garlic(bn::sprite_items::fez_garlic.create_sprite(-256, -256)),  // Garlic
		_cross(bn::sprite_items::fez_cross.create_sprite(-256, -256))  // Cross
	{
		constexpr int frames_diff = 480 - 350;//maximum_frames - minimum_frames;
		constexpr int maximum_speed_completed_games = 30;

		completed_games = bn::min(completed_games, maximum_speed_completed_games);

		int frames_reduction = (frames_diff * completed_games) / maximum_speed_completed_games;
		_total_frames = 480 - frames_reduction;
		_total_frames -= data.random.get_int(60);
		_total_frames = bn::clamp(_total_frames, 350, 480);
		
		switch (recommended_difficulty_level(completed_games, data)) {
			default:
			case mj::difficulty_level::EASY:
				oN = 2;
				break;
			case mj::difficulty_level::NORMAL:
				oN = 3;
				break;
			case mj::difficulty_level::HARD:
				oN = 4;
				break;
		}	
		
		// Sprite Stuff
		
		_moon1.set_rotation_angle(180);
		_moon2.set_rotation_angle(90);
		_moon3.set_rotation_angle(270);
		
		_moon0.set_z_order(1);
		_moon1.set_z_order(1);
		_moon2.set_z_order(1);
		_moon3.set_z_order(1);
		
		_bg_speed.put_above();
		
		// Variables
		
		xx = 0.0f;
		yy = -cy + 32.0f;
		vx = 0.0f;
		vy = 0.0f;
		mpy = 0.0f;
		
		for(int i = 0; i < 4; i++) {
			oX[i] = -256;
			oY[i] = -256;
		}
		
		for(int i = 0; i < oN; i++) {
			oX[i] = data.random.get_int(-cx+16, cx-16);
			oY[i] = data.random.get_int(-26, cy+256);
		}
		
		oS[0] = 3.2f;
		oS[1] = 4.0f;
		oS[2] = 3.0f;
		oS[3] = 3.4f;
		
		_umbrella.set_x(oX[0]);
		_stake.set_x(oX[1]);
		_garlic.set_x(oX[2]);
		
		end = false;
		t = 0;
		
		// Music
		bn::sound_items::fez_music.play(0.8);
	}

	void game::fade_in([[maybe_unused]] const mj::game_data& data)
	{
	}

	mj::game_result game::play(const mj::game_data& data) // Main thing
	{
		mj::game_result result;
		result.exit = data.pending_frames == 0;
						
		// Movement
		
		if(!_victory && !_defeat) {
			if(bn::keypad::left_held() && xx > -cx+26) {
				xx -= 1.8;	
				if(vy > 3.6f) vy -= 0.3;
			} 
			if(bn::keypad::right_held() && xx < cx-26) {
				xx += 1.8;	
				if(vy > 3.6f) vy -= 0.3;
			}	
		}
		
		if(vy < 9) vy += g;
		vx *= 0.98;

		// Animation
		
		if(t < 42) _bat.set_tiles(bn::sprite_items::fez_bat.tiles_item().create_tiles(t/6));
		else if(t > 68) {
			if(vy > 6.5) _bat.set_tiles(bn::sprite_items::fez_bat.tiles_item().create_tiles(6));
			else if(vy > 5) _bat.set_tiles(bn::sprite_items::fez_bat.tiles_item().create_tiles(5));
			else if(vy > 4) _bat.set_tiles(bn::sprite_items::fez_bat.tiles_item().create_tiles(4));
			else _bat.set_tiles(bn::sprite_items::fez_bat.tiles_item().create_tiles(3));
		}

		// Set position
		
		_bat.set_x(xx);
		if(!_victory) _bat.set_y(yy+vy);
		else {
			_bat.set_y(_bat.y() + 3);
			_bat.set_tiles(bn::sprite_items::fez_bat.tiles_item().create_tiles(7));
		}
		
		// End events
		
		if(_defeat) {
			bn::fixed hs = _bat.horizontal_scale();
            bn::fixed vs = _bat.vertical_scale();
			
			if(hs-0.05 > 0) {
				_bat.set_horizontal_scale(hs-0.02);
				_bat.set_vertical_scale(vs-0.02);
			} else {
				_bat.set_x(-512);
				_bat.set_y(-512);
			}
		}
		
		// Obstacles
		
		for(int i = 0; i < oN; i++) {
			// Movement
			oY[i] -= vy - oS[i];
			if(oY[i] < -cy-16 && mpy < rh*10) {
				oY[i] += cy*2 + 128;
				oX[i] = data.random.get_int(-cx+16, cx-16);
			}
			
			if(_victory && bn::sqrt( (oX[i] - xx)*(oX[i] - xx) + (oY[i] - _bat.y())*(oY[i] - _bat.y()) ) < 32) {
				if(oX[i] > xx) oX[i] += 8;
				else oX[i] -= 8;
			}
			
			// Check collisions
			if(bn::sqrt( (oX[i] - xx)*(oX[i] - xx) + (oY[i] - yy)*(oY[i] - yy) ) < 15 && !_defeat && !_victory) {
				result.remove_title = true;
				_defeat = true;
				bn::sound::stop_all();
				bn::sound_items::fez_hurt.play_with_priority(3);
			}
			
		}
		
		_umbrella.set_x(oX[0]);
		_umbrella.set_y(oY[0]);
		_umbrella.set_rotation_angle(t%360);
		
		_stake.set_x(oX[1]);
		_stake.set_y(oY[1]);
		_stake.set_rotation_angle((t+32)%360);
		
		_garlic.set_x(oX[2]);
		_garlic.set_y(oY[2]);
		_garlic.set_rotation_angle((t+128)%360);
		
		_cross.set_x(oX[3]);
		_cross.set_y(oY[3]);
		_cross.set_rotation_angle((t+240)%360);
		
		// Moon
		mpy += vy;
		_moon0.set_y(-31-mpy/18);
		_moon1.set_y(31-mpy/18);
		_moon2.set_y(31-mpy/18);
		_moon3.set_y(-31-mpy/18);
		
		// End
		if(mpy > rh*11 && !_defeat && !_victory) {
			_victory = true;
			bn::sound_items::fez_win.play(1);
		}
		
		// BG
		_bg_speed.set_y(_bg_speed.y() - vy);
		
		// Time
		t++;

		if(_victory || _defeat) // End game
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

		return result;
		 // Sorry for the ugly code, this is my first time developing for the GBA :)
	}

	void game::fade_out([[maybe_unused]] const mj::game_data& data)
	{
	}

}
