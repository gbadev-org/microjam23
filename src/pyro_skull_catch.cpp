#include "pyro_skull_catch.h"

// I don't know which ones I do and don't need anymore, here's all the ones the sprites example uses.
#include "bn_core.h"
#include "bn_math.h"
#include "bn_keypad.h"
#include "bn_display.h"
#include "bn_blending.h"
#include "bn_bg_palettes.h"
#include "bn_regular_bg_ptr.h"
#include "bn_sprites_mosaic.h"
#include "bn_sprite_actions.h"
#include "bn_sprite_builder.h"
#include "bn_sprite_text_generator.h"
#include "bn_sprite_animate_actions.h"
#include "bn_sprite_first_attributes.h"
#include "bn_sprite_third_attributes.h"
#include "bn_sprite_position_hbe_ptr.h"
#include "bn_sprite_first_attributes_hbe_ptr.h"
#include "bn_sprite_third_attributes_hbe_ptr.h"
#include "bn_sprite_affine_second_attributes.h"
#include "bn_sprite_regular_second_attributes.h"
#include "bn_sprite_affine_second_attributes_hbe_ptr.h"
#include "bn_sprite_regular_second_attributes_hbe_ptr.h"
#include "bn_fixed.h"

#include "mj/mj_game_list.h"

#include "bn_regular_bg_items_pyro_skullcatch_stage.h"
#include "bn_sprite_items_pyro_skeletonbody.h"
#include "bn_sprite_items_pyro_skeletonhead.h"

namespace
{
	constexpr bn::string_view code_credits[] = { "PyroPyro" };
	constexpr bn::string_view graphics_credits[] = { "PyroPyro" };
}

MJ_GAME_LIST_ADD(pyro_sc::skullCatch)
MJ_GAME_LIST_ADD_CODE_CREDITS(code_credits)
MJ_GAME_LIST_ADD_GRAPHICS_CREDITS(graphics_credits)

namespace pyro_sc
{

void approach(fixed val, fixed target, fixed vel)
{
	if(val > target)
	{
		val -= vel;
		if(val < target)
		{
			val = target;
		}
	}
	else if(val < target)
	{
		val += vel;
		if(val > target)
		{
			val = target;
		}
	}
}

Skull::Skull(fixed_point p):
	sprite(sprite_items::pyro_skeletonhead.create_sprite(p.x(),p.y()))
{
	active = false;
	pos = p;
	vel = fixed_point(0,0);
	angle = 0;
	angle_vel = 1;
}

void Skull::update(const mj::game_data& data)
{
	if(active)
	{
		fixed velX = vel.x();
		approach(velX, fixed(0), fixed(0.1));
		vel.set_x(velX);
		vel.set_y(vel.y() + fixed(0.1));
		pos = pos + vel;
		sprite.set_position(pos);
		angle += angle_vel;
		while(angle > fixed(360))
		{
			angle -= fixed(360);
		}
		sprite.set_rotation_angle(angle);
	}
	else
	{
		sprite.set_position(pos + offset);
	}
	
}

Skelebro::Skelebro(fixed_point p, bool hflip) :
	sprite_body(sprite_items::pyro_skeletonbody.create_sprite(p.x(),p.y())),
	skull(Skull(fixed_point(p.x(),p.y()-15)))
{
	flipTimer = maxFlipTimer;
	sprite_body.set_horizontal_flip(hflip);
	throwing = false;
	pos = p;
}

void Skelebro::update(const mj::game_data& data)
{
	if(throwing)
	{
		timer--;
		if(timer == 0)
		{
			sprite_body.set_tiles(sprite_items::pyro_skeletonbody.tiles_item().create_tiles(2));
			fixed velX = data.random.get_fixed(fixed(-0.5),fixed(2));
			if(pos.x() > 0)
			{
				velX = -velX;
			}
			skull.toss(fixed_point(velX,fixed(-4)));
		}
	}
	else
	{
		flipTimer -= fixed(1);
		if(flipTimer <= fixed(0))
		{
			flipTimer += maxFlipTimer;
			sprite_body.set_horizontal_flip(!sprite_body.horizontal_flip());
			if(sprite_body.horizontal_flip())
			{
				skull.offset.set_x(1);
			}
			else
			{
				skull.offset.set_x(-1);
			}
		}
		if(flipTimer >= (maxFlipTimer/2))
		{
			skull.offset.set_y(0);
		}
		else
		{
			skull.offset.set_y(1);
		}
	}
	skull.update(data);
}

void Skelebro::throwSkull()
{
	if(!throwing)
	{
		throwing = true;
		timer = maxFlipTimer.integer();
		sprite_body.set_tiles(sprite_items::pyro_skeletonbody.tiles_item().create_tiles(1));
	}
}

skullCatch::skullCatch(int completed_games, const mj::game_data& data) :
	_bg(regular_bg_items::pyro_skullcatch_stage.create_bg((256 - 240) / 2, (256 - 160) / 2)),
	_total_frames(play_jingle(mj::game_jingle_type::METRONOME_16BEAT, completed_games, data)),
	_player_sprite(sprite_items::pyro_skeletonbody.create_sprite(0,38)),
	_skelebros{
		Skelebro(fixed_point(-92,23),false),
		Skelebro(fixed_point(-41,19),true),
		Skelebro(fixed_point(41,19),false),
		Skelebro(fixed_point(92,23),true)
	}
{
	constexpr int frames_diff = maximum_frames - minimum_frames;
	constexpr int maximum_speed_completed_games = 30;
	
	completed_games = min(completed_games, maximum_speed_completed_games);
	
	int frames_reduction = (frames_diff * completed_games) / maximum_speed_completed_games;
	
	int actual_total_frames = _total_frames + 48;
	
	fixed beatLength = fixed(actual_total_frames)/16;
	
	_skelebros[0].maxFlipTimer = beatLength;
	_skelebros[0].flipTimer = beatLength - 24;
	_skelebros[1].maxFlipTimer = beatLength;
	_skelebros[1].flipTimer = beatLength - 24;
	_skelebros[2].maxFlipTimer = beatLength;
	_skelebros[2].flipTimer = beatLength - 24;
	_skelebros[3].maxFlipTimer = beatLength;
	_skelebros[3].flipTimer = beatLength - 24;
}

void skullCatch::fade_in([[maybe_unused]] const mj::game_data& data)
{
}

mj::game_result skullCatch::play(const mj::game_data& data)
{
	mj::game_result result;
	result.exit = data.pending_frames == 0;
	
	_skelebros[0].update(data);
	_skelebros[1].update(data);
	_skelebros[2].update(data);
	_skelebros[3].update(data);
	
	if(keypad::a_pressed())
	{
		_skelebros[0].throwSkull();
	}
	if(keypad::b_pressed())
	{
		_skelebros[2].throwSkull();
	}
	
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
