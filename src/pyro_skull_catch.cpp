#include "pyro_skull_catch.h"

#include "bn_math.h"
#include "bn_keypad.h"
#include "bn_sprite_tiles_ptr.h"

#include "mj/mj_game_list.h"

#include "bn_regular_bg_items_pyro_skullcatch_stage.h"
#include "bn_sprite_items_pyro_skeletonbody.h"
#include "bn_sprite_items_pyro_skeletonhead.h"

#include "bn_sound_items.h"

namespace
{
	constexpr bn::string_view code_credits[] = { "PyroPyro" };
	constexpr bn::string_view graphics_credits[] = { "PyroPyro" };
	constexpr bn::string_view sfx_credits[] = { "PyroPyro" };
}

// MJ_GAME_LIST_ADD(pyro_sc::skullCatch)
MJ_GAME_LIST_ADD_CODE_CREDITS(code_credits)
MJ_GAME_LIST_ADD_GRAPHICS_CREDITS(graphics_credits)
MJ_GAME_LIST_ADD_SFX_CREDITS(sfx_credits)

namespace pyro_sc
{

namespace
{
    void approach(fixed &val, fixed target, fixed vel)
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
}

Skull::Skull(fixed_point p):
	sprite(sprite_items::pyro_skeletonhead.create_sprite(p.x(),p.y()))
{
	active = false;
	pos = p;
	vel = fixed_point(0,0);
	angle = 0;
	angle_vel = 1;
	cracked = false;
	missed = false;
	caught = false;
	sprite.set_bg_priority(1);
}

void Skull::update(const mj::game_data& data)
{
	if(active)
	{
		fixed velX = vel.x();
		approach(velX, fixed(0), fixed(0.01));
		vel.set_x(velX);
		vel.set_y(vel.y() + fixed(0.1));
		pos = pos + vel;
		sprite.set_position(pos);
		if(pos.y() >= floorY && !cracked)
		{
			sound_items::pyro_skullcatch_drop.play(1, data.random.get_fixed(0.9, 1.1), clamp(pos.x()/100,fixed(-1),fixed(1)));
			vel.set_y(vel.y() * fixed(-0.5));
			cracked = true;
			sprite.set_tiles(sprite_items::pyro_skeletonhead.tiles_item().create_tiles(1));
		}
		//angle += angle_vel;
		//while(angle > fixed(360))
		//{
		//	angle -= fixed(360);
		//}
		//sprite.set_rotation_angle(angle);
	}
	else
	{
		sprite.set_position(pos + offset);
	}
	
}

Skelebro::Skelebro(fixed_point p, bool hflip) :
    skull(fixed_point(p.x(),p.y()-15)),
    sprite_body(sprite_items::pyro_skeletonbody.create_sprite(p.x(),p.y()))
{
	flipTimer = maxFlipTimer;
	sprite_body.set_horizontal_flip(hflip);
	throwing = false;
	pos = p;
	timer = -1;
	
	sprite_body.set_bg_priority(3);
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
			sound_items::pyro_skullcatch_throw.play(1, data.random.get_fixed(0.9, 1.1), pos.x()/100);
		}
	}
	else
	{
		if (timer > 0)
		{
			timer--;
			if (timer <= 0)
			{
				throwSkull();
			}
		}
	}
	
	flipTimer -= fixed(1);
	if(flipTimer <= fixed(0))
	{
		flipTimer += maxFlipTimer;
		sprite_body.set_horizontal_flip(!sprite_body.horizontal_flip());
		if (!skull.caught)
		{
			if(sprite_body.horizontal_flip())
			{
				skull.offset.set_x(1);
			}
			else
			{
				skull.offset.set_x(-1);
			}
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
	
	skull.update(data);
}

skullCatch::skullCatch(int completed_games, const mj::game_data& data) :
	_bg(regular_bg_items::pyro_skullcatch_stage.create_bg((256 - 240) / 2, (256 - 160) / 2)),
	_total_frames(play_jingle(mj::game_jingle_type::TOTSNUK17, completed_games, data)),
	_skelebros{
		Skelebro(fixed_point(-92,23),false),
		Skelebro(fixed_point(-41,19),true),
		Skelebro(fixed_point(41,19),false),
		Skelebro(fixed_point(92,23),true)
	},
	_player_sprite(sprite_items::pyro_skeletonbody.create_sprite(0,38))
{
	int actual_total_frames = _total_frames + 48;
	
	fixed beatLength = fixed(actual_total_frames)/16;
	
	_skelebros[0].maxFlipTimer = beatLength;
	_skelebros[0].flipTimer = beatLength - 24;
	_skelebros[0].timer = beatLength.integer()*3;
	
	_skelebros[1].maxFlipTimer = beatLength;
	_skelebros[1].flipTimer = beatLength - 24;
	_skelebros[1].timer = beatLength.integer()*5;
	
	_skelebros[2].maxFlipTimer = beatLength;
	_skelebros[2].flipTimer = beatLength - 24;
	_skelebros[2].timer = beatLength.integer()*7;
	
	_skelebros[3].maxFlipTimer = beatLength;
	_skelebros[3].flipTimer = beatLength - 24;
	_skelebros[3].timer = beatLength.integer()*9;
	
	_player_sprite.set_bg_priority(2);
	
	_player_x = fixed(0);
	_player_catch_x_offset = fixed(0);
	_player_catch_y = fixed(23);
	_target = max(1,min(4, (completed_games+10)/10));
	_catch_count = 0;
	_player_footstep_timer = 1;
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
	
	fixed speed = fixed(3);
	bool dash = keypad::a_held() || keypad::b_held();
	
	if (dash)
	{
		speed = fixed(5);
	}
	
	// Move the player
	if(keypad::left_held())
	{
		_player_x-=speed;
		_player_anim_timer--;
		_player_footstep_timer--;
	}
	else if(keypad::right_held())
	{
		_player_x+=speed;
		_player_anim_timer--;
		_player_footstep_timer--;
	}
	else
	{
		_player_anim_timer = 3;
	}
	
	//Detect a catch
	for (int i = 0; i <= 3; i++)
	{
		if (_skelebros[i].skull.active && !_skelebros[i].skull.missed)
		{
			if (_skelebros[i].skull.pos.y() > _player_catch_y)
			{
				fixed player_catch_x = _player_x + _player_catch_x_offset;
				if (abs((player_catch_x) - _skelebros[i].skull.pos.x()) <= fixed(10))
				{
					_skelebros[i].skull.caught = true;
					_skelebros[i].skull.active = false;
					_skelebros[i].skull.offset.set_x(_skelebros[i].skull.pos.x() - _player_x);
					_skelebros[i].skull.offset.set_y(_player_catch_y);
					_player_catch_y -= fixed(8);
					_player_catch_x_offset = _skelebros[i].skull.offset.x();
					sound_items::pyro_skullcatch_catch.play(1, fixed(1) + (fixed(0.1)*_catch_count), clamp(((player_catch_x)/100),fixed(-1),fixed(1)));
					_catch_count++;
				}
				else
				{
					_skelebros[i].skull.missed = true;
				}
			}
		}
		
		if (_skelebros[i].skull.caught)
		{
			_skelebros[i].skull.pos.set_x(_player_x);
		}
	}
	
	// Update Player sprite position
	if(_player_anim_timer <= 0)
	{
		_player_sprite.set_horizontal_flip(!_player_sprite.horizontal_flip());
		_player_anim_timer = 5;
	}
	if (_player_footstep_timer <= 0)
	{
		_player_footstep_timer = 12;
		if (dash)
		{
			sound_items::pyro_skullcatch_footstep.play(0.5, 1.5, clamp(((_player_x)/100),fixed(-1),fixed(1)));
		}
		else
		{
			sound_items::pyro_skullcatch_footstep.play(0.5, 1, clamp(((_player_x)/100),fixed(-1),fixed(1)));
		}
	}
	_player_sprite.set_x(_player_x.integer());
	
	return result;
}

bool skullCatch::victory() const
{
    if(_skelebros[0].failed() || _skelebros[1].failed() || _skelebros[2].failed() || _skelebros[3].failed())
    {
        return _catch_count >= _target;
    }

    return true;
}

void skullCatch::fade_out([[maybe_unused]] const mj::game_data& data)
{
}

}
