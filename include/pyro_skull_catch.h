#ifndef PYRO_SKULL_CATCH_H
#define PYRO_SKULL_CATCH_H

#include "bn_regular_bg_ptr.h"
#include "bn_sprite_ptr.h"
#include "bn_fixed_point.h"

#include "mj/mj_game.h"

namespace pyro_sc
{

using namespace bn; //Style guide, shmyle guide!

const fixed floorY = fixed(50);

class Skull{
public:
	Skull(fixed_point p);
	
	void update(const mj::game_data& data);
	
	void toss(fixed_point v){
		vel = v;
		active = true;
	}
	
	fixed_point offset;
	bool active;
	bool cracked;
	bool missed;
	bool caught;
	fixed_point pos;

private:
	fixed_point vel;
	fixed angle;
	fixed angle_vel;
	sprite_ptr sprite;
};

class Skelebro{
public:
	Skelebro(fixed_point p, bool hflip);
	
	void update(const mj::game_data& data);
	
	void throwSkull();
	
    bool failed() const
	{
		return skull.cracked;
	}
	
	fixed maxFlipTimer;
	fixed flipTimer;
	int timer;
	Skull skull;

private:
	fixed_point pos;
	sprite_ptr sprite_body;
	bool throwing;
};

class skullCatch : public mj::game
{

public:
	skullCatch(int completed_games, const mj::game_data& data);
	
	[[nodiscard]] string<16> title() const final
	{
		switch (_target)
		{
		case 0:
			//This shouldn't be possible, free win it happens though.
            return "Do Whatever";
		
		case 1:
            return "Get A Head!";
		
		case 2:
            return "Double Up!";
		
		case 3:
            return "Catch 3 Skulls!";
		
		case 4:
            return "Catch 4 Skulls!";
		
		default:
			// This game is impossible if this ever happens, so it shouldn't.
            return "SOMETHING IS WRONG";
		}
	}
	
	[[nodiscard]] int total_frames() const final
	{
		return _total_frames;
	}
	
	void fade_in(const mj::game_data& data) final;
	
    [[nodiscard]] mj::game_result play(const mj::game_data& data) final;

    [[nodiscard]] bool victory() const final;
	
	void fade_out(const mj::game_data& data) final;

private:
	regular_bg_ptr _bg;
	int _total_frames;
    int _show_result_frames = 60;
	fixed _player_x = 0;
	fixed _player_catch_x_offset = 0;
	fixed _player_catch_y = fixed(23);
	int _player_anim_timer = 0;
	int _player_footstep_timer = 0;
	int _player_anim_frame = 0;
	Skelebro _skelebros[4];
	sprite_ptr _player_sprite;
	int _target = 0;
	int _catch_count = 0;
};

}

#endif
