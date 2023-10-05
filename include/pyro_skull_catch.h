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

class skull{
public:
	skull(fixed_point p, fixed_point v);
	
	void update();
	
	void toss(fixed_point p, fixed_point v){
		pos = p;
		vel = v;
		active = true;
	}

private:
	bool active;
	fixed_point pos;
	fixed_point vel;
	fixed angle;
	fixed angle_vel;
};

class skelebro{
public:
	skelebro(fixed_point p, bool hflip);
	
	void update();
	
	const int maxFlipTimer = 30;

private:
	int frame;
	sprite_ptr sprite_body;
	sprite_ptr sprite_head;
	int flip_timer;
	bool has_head;
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
	skelebro _skelebros[4];
};

}

#endif
