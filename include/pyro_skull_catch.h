#ifndef TMG_TEST_GAME_H
#define TMG_TEST_GAME_H

#include "bn_regular_bg_ptr.h"
#include "bn_sprite_ptr.h"
#include "bn_fixed_point.h"
#include "bn_fixed.h"

#include "mj/mj_game.h"

namespace pyro_sc
{

using namespace bn; //Style guide, shmyle guide!

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

private:
	fixed_point pos;
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
	
	const int maxFlipTimer = 30;

private:
	fixed_point pos;
	sprite_ptr sprite_body;
	int timer;
	Skull skull;
	bool throwing;
};

class skullCatch : public mj::game
{

public:
	skullCatch(int completed_games, const mj::game_data& data);
	
	[[nodiscard]] string<16> title() const final
	{
		return "Get a head!";
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
	regular_bg_ptr _bg;
	int _total_frames;
	int _show_result_frames = 60;
	bool _victory = false;
	bool _defeat = false;
	int _player_x = 0;
	int _player_anim_timer = 0;
	int _player_anim_frame = 0;
	sprite_ptr _player_sprite;
	Skelebro _skelebros[4];
};

}

#endif
