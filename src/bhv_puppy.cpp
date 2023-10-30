#include "bn_sprite_builder.h"

#include "bhv_puppy.h"

#include "bn_sprite_items_bhv_pumppy.h"

namespace bhv
{
	bhv_puppy::bhv_puppy()
	{
		_pos = bn::fixed_point(0, 0);
		bn::sprite_builder builder(bn::sprite_items::bhv_pumppy);
		builder.set_bg_priority(1);
		builder.set_z_order(20);
		builder.set_position(_pos);
		_spr = builder.release_build();
		_anim_idle = bn::create_sprite_animate_action_forever(*_spr.get(), 6, bn::sprite_items::bhv_pumppy.tiles_item(), 0, 1, 2, 3);
		_anim_sing = bn::create_sprite_animate_action_once(*_spr.get(), 6, bn::sprite_items::bhv_pumppy.tiles_item(), 6, 7, 4);
		_anim_win = bn::create_sprite_animate_action_forever(*_spr.get(), 6, bn::sprite_items::bhv_pumppy.tiles_item(), 6, 7, 4);

		set_anim_state(BHV_ANIM_IDLE);
	}

	void bhv_puppy::update_anim()
	{
		if (_anim_state == BHV_ANIM_SING && _anim_sing.has_value())
		{
			if (_anim_sing.get()->done())
			{
				set_anim_state(BHV_ANIM_IDLE);
			}
			_anim_sing.get()->update();
		}
		else if (_anim_state == BHV_ANIM_WIN && _anim_win.has_value())
		{
			_anim_win.get()->update();
		}
		else if (_anim_idle.has_value())
		{
			_anim_idle.get()->update();
		}
	}

	void bhv_puppy::set_wait_updates(int frames)
	{
		if (_anim_idle.has_value())
		{
			_anim_idle.get()->set_wait_updates(frames);
		}
		if (_anim_sing.has_value())
		{
			_anim_sing.get()->set_wait_updates(frames);
		}
		if (_anim_win.has_value())
		{
			_anim_win.get()->set_wait_updates(frames);
		}
	}

	void bhv_puppy::set_anim_state(anim_state state)
	{
		_anim_state = state;
		if (_anim_idle.has_value())
		{
			_anim_idle.get()->reset();
		}
		if (_anim_sing.has_value())
		{
			_anim_sing.get()->reset();
		}
	}
}
