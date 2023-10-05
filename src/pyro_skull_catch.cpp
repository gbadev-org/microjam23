#include "pyro_skull_catch.h"

#include "bn_keypad.h"
#include "bn_fixed_point.h"

#include "bn_regular_bg_ptr.h"

#include "mj/mj_game_list.h"

#include "bn_regular_bg_items_pyro_skullcatch_stage.h"
#include "bn_sprite_items_pyro_skeletonbody.h"
#include "bn_sprite_items_pyro_skeletonhead.h"

MJ_GAME_LIST_ADD(pyro_sc::skullCatch)

namespace pyro_sc
{

skull::skull(fixed_point p, fixed_point v)
{
	active = false;
	pos = p;
	vel = v;
	angle = 0;
	angle_vel = 0;
}

void skull::update()
{
	vel.set_y(vel.y() + fixed(0.2));
}

skelebro::skelebro(fixed_point p, bool hflip) :
	sprite_body(sprite_items::pyro_skeletonbody.create_sprite(p.x(),p.y())),
	sprite_head(sprite_items::pyro_skeletonhead.create_sprite(p.x(),p.y()-15))
{
	flip_timer = maxFlipTimer;
	sprite_body.set_horizontal_flip(hflip);
	has_head = true;
}

void skelebro::update()
{
	flip_timer--;
	if(flip_timer <= 0)
	{
		flip_timer = maxFlipTimer;
		sprite_body.set_horizontal_flip(!sprite_body.horizontal_flip());
		if(sprite_body.horizontal_flip())
		{
			sprite_head.set_x(sprite_body.x() + 1);
		}
		else
		{
			sprite_head.set_x(sprite_body.x() - 1);
		}
	}
	if(flip_timer >= (maxFlipTimer/2))
	{
		sprite_head.set_y(sprite_body.y() - 15);
	}
	else
	{
		sprite_head.set_y(sprite_body.y() - 14);
	}
}

skullCatch::skullCatch(int completed_games, const mj::game_data& data) :
	_bg(regular_bg_items::pyro_skullcatch_stage.create_bg((256 - 240) / 2, (256 - 160) / 2)),
	_player_sprite(sprite_items::pyro_skeletonbody.create_sprite(0,38)),
	_skelebros{
		skelebro(fixed_point(-92,23),false),
		skelebro(fixed_point(-41,19),true),
		skelebro(fixed_point(41,19),false),
		skelebro(fixed_point(92,23),true)
	}
{
	constexpr int frames_diff = maximum_frames - minimum_frames;
	constexpr int maximum_speed_completed_games = 30;
	
	completed_games = min(completed_games, maximum_speed_completed_games);
	
	int frames_reduction = (frames_diff * completed_games) / maximum_speed_completed_games;
	_total_frames = maximum_frames - frames_reduction;
	_total_frames -= data.random.get_int(60);
	_total_frames = clamp(_total_frames, minimum_frames, maximum_frames);
	
}

void skullCatch::fade_in([[maybe_unused]] const mj::game_data& data)
{
}

mj::game_result skullCatch::play(const mj::game_data& data)
{
	mj::game_result result;
	result.exit = data.pending_frames == 0;
	
	_skelebros[0].update();
	_skelebros[1].update();
	_skelebros[2].update();
	_skelebros[3].update();
	
	if(keypad::left_held())
	{
		_player_x-=2;
		_player_anim_timer--;
	}
	else if(keypad::right_held())
	{
		_player_x+=2;
		_player_anim_timer--;
	}
	else
	{
		_player_anim_timer = 3;
	}
	
	if(_player_anim_timer <= 0)
	{
		_player_sprite.set_horizontal_flip(!_player_sprite.horizontal_flip());
		_player_anim_timer = 5;
	}
	_player_sprite.set_x(_player_x);
	
	return result;
}

void skullCatch::fade_out([[maybe_unused]] const mj::game_data& data)
{
}

}
