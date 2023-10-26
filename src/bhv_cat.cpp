#include "bn_sprite_builder.h"

#include "bhv_cat.h"

#include "bn_sprite_items_bhv_cat.h"

namespace bhv
{
	bhv_cat::bhv_cat(bn::fixed_point pos)
	{
		_pos = pos;
		bn::sprite_builder builder(bn::sprite_items::bhv_cat);
		builder.set_bg_priority(1);
		builder.set_z_order(20);
		builder.set_position(_pos);
		_spr = builder.release_build();
		_anim_idle = bn::create_sprite_animate_action_forever(*_spr.get(), 6, bn::sprite_items::bhv_cat.tiles_item(), 0, 1, 2, 3, 4, 5);
		_anim_sing = bn::create_sprite_animate_action_once(*_spr.get(), 6, bn::sprite_items::bhv_cat.tiles_item(), 7, 8, 9);
		_anim_win = bn::create_sprite_animate_action_forever(*_spr.get(), 6, bn::sprite_items::bhv_cat.tiles_item(), 7, 8, 9);
		_anim_lose = bn::create_sprite_animate_action_forever(*_spr.get(), 6, bn::sprite_items::bhv_cat.tiles_item(), 10, 11);

		set_anim_state(BHV_ANIM_IDLE);
	}

	void bhv_cat::update_anim()
	{
		if (_anim_state == BHV_ANIM_SING && _anim_sing.has_value())
		{
			if (_anim_sing.get()->done())
			{
				set_anim_state(BHV_ANIM_IDLE);
			}
			_anim_sing.get()->update();
		}
		else if (_anim_state == BHV_ANIM_LOSE && _anim_lose.has_value())
		{
			_anim_lose.get()->update();
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

	void bhv_cat::set_wait_updates(int frames)
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
		if (_anim_lose.has_value())
		{
			_anim_lose.get()->set_wait_updates(frames);
		}
	}

	void bhv_cat::set_anim_state(anim_state state)
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
