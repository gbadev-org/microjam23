#include "bn_sprite_builder.h"

#include "bhv_conductor.h"

#include "bn_sprite_items_bhv_conductor_head.h"
#include "bn_sprite_items_bhv_conductor_body.h"
#include "bn_sprite_items_bhv_conductor_arm_l.h"
#include "bn_sprite_items_bhv_conductor_arm_r.h"
#include "bn_sprite_items_bhv_conductor_tail.h"

namespace bhv
{
	bhv_conductor::bhv_conductor()
	{
		_pos = bn::fixed_point(0, 0);
		{
			bn::sprite_builder builder(bn::sprite_items::bhv_conductor_head);
			builder.set_bg_priority(1);
			builder.set_z_order(45);
			builder.set_position(_pos);
			_spr = builder.release_build();
		}
		{
			bn::sprite_builder builder(bn::sprite_items::bhv_conductor_body);
			builder.set_bg_priority(1);
			builder.set_z_order(50);
			builder.set_position(_pos);
			_spr_body = builder.release_build();
		}
		{
			bn::sprite_builder builder(bn::sprite_items::bhv_conductor_arm_l);
			builder.set_bg_priority(1);
			builder.set_z_order(55);
			builder.set_position(_pos);
			_spr_arm_l = builder.release_build();
		}
		{
			bn::sprite_builder builder(bn::sprite_items::bhv_conductor_arm_r);
			builder.set_bg_priority(1);
			builder.set_z_order(40);
			builder.set_position(_pos);
			_spr_arm_r = builder.release_build();
		}
		{
			bn::sprite_builder builder(bn::sprite_items::bhv_conductor_tail);
			builder.set_bg_priority(1);
			builder.set_z_order(55);
			builder.set_position(_pos);
			_spr_tail = builder.release_build();
		}
		_anim_head = bn::create_sprite_animate_action_forever(*_spr.get(), 6, bn::sprite_items::bhv_conductor_head.tiles_item(), 0, 1, 2, 3, 4, 5);
		_anim_body = bn::create_sprite_animate_action_forever(*_spr_body.get(), 6, bn::sprite_items::bhv_conductor_body.tiles_item(), 0, 1, 2, 1, 0, 3);
		_anim_arm_l = bn::create_sprite_animate_action_forever(*_spr_arm_l.get(), 6, bn::sprite_items::bhv_conductor_arm_l.tiles_item(), 0, 1, 2, 1, 0, 3);
		_anim_arm_r = bn::create_sprite_animate_action_forever(*_spr_arm_r.get(), 6, bn::sprite_items::bhv_conductor_arm_r.tiles_item(), 0, 1, 2, 1, 0, 3);
		_anim_tail = bn::create_sprite_animate_action_forever(*_spr_tail.get(), 6, bn::sprite_items::bhv_conductor_tail.tiles_item(), 0, 1, 2);

		set_anim_idle();
	}

	bhv_conductor::~bhv_conductor()
	{
	}

	void bhv_conductor::set_position(bn::fixed_point pos)
	{
		_pos = pos;
		if (_spr.has_value())
		{
			_spr.get()->set_position(pos + bn::fixed_point(1, -15));
		}
		if (_spr_body.has_value())
		{
			_spr_body.get()->set_position(pos + bn::fixed_point(0, 8));
		}
		if (_spr_arm_l.has_value())
		{
			_spr_arm_l.get()->set_position(pos + bn::fixed_point(-22, 0));
		}
		if (_spr_arm_r.has_value())
		{
			_spr_arm_r.get()->set_position(pos + bn::fixed_point(18, 1));
		}
		if (_spr_tail.has_value())
		{
			_spr_tail.get()->set_position(pos + bn::fixed_point(8, 24));
		}
	}

	void bhv_conductor::update_anim()
	{
		if (_anim_head.has_value())
		{
			_anim_head.get()->update();
		}
		if (_anim_body.has_value())
		{
			_anim_body.get()->update();
		}
		if (_anim_arm_l.has_value())
		{
			_anim_arm_l.get()->update();
		}
		if (_anim_arm_r.has_value())
		{
			_anim_arm_r.get()->update();
		}
		if (_anim_tail.has_value())
		{
			_anim_tail.get()->update();
		}
	}

	void bhv_conductor::set_wait_updates(int frames)
	{
		if (_anim_head.has_value())
		{
			_anim_head.get()->set_wait_updates(frames);
		}
		if (_anim_body.has_value())
		{
			_anim_body.get()->set_wait_updates(frames);
		}
		if (_anim_arm_l.has_value())
		{
			_anim_arm_l.get()->set_wait_updates(frames);
		}
		if (_anim_arm_r.has_value())
		{
			_anim_arm_r.get()->set_wait_updates(frames);
		}
		if (_anim_tail.has_value())
		{
			_anim_tail.get()->set_wait_updates(frames);
		}
	}

	void bhv_conductor::set_anim_idle()
	{
		_anim_head = bn::create_sprite_animate_action_forever(*_spr.get(), 6, bn::sprite_items::bhv_conductor_head.tiles_item(), 0, 1, 2, 3, 4, 5);
		_anim_body = bn::create_sprite_animate_action_forever(*_spr_body.get(), 6, bn::sprite_items::bhv_conductor_body.tiles_item(), 0, 1, 2, 1, 0, 3);
		_anim_arm_l = bn::create_sprite_animate_action_forever(*_spr_arm_l.get(), 6, bn::sprite_items::bhv_conductor_arm_l.tiles_item(), 0, 1, 2, 1, 0, 3);
		_anim_arm_r = bn::create_sprite_animate_action_forever(*_spr_arm_r.get(), 6, bn::sprite_items::bhv_conductor_arm_r.tiles_item(), 0, 1, 2, 1, 0, 3);
		_anim_tail = bn::create_sprite_animate_action_forever(*_spr_tail.get(), 6, bn::sprite_items::bhv_conductor_tail.tiles_item(), 0, 1, 2);
	}

	void bhv_conductor::set_anim_lose()
	{
		_anim_head = bn::create_sprite_animate_action_forever(*_spr.get(), 6, bn::sprite_items::bhv_conductor_head.tiles_item(), 10, 11, 12, 11, 10, 12);
		_anim_body = bn::create_sprite_animate_action_forever(*_spr_body.get(), 6, bn::sprite_items::bhv_conductor_body.tiles_item(), 0, 1, 2, 1, 0, 3);
		_anim_arm_l = bn::create_sprite_animate_action_forever(*_spr_arm_l.get(), 6, bn::sprite_items::bhv_conductor_arm_l.tiles_item(), 7, 8, 9, 8, 7, 10);
		_anim_arm_r = bn::create_sprite_animate_action_forever(*_spr_arm_r.get(), 6, bn::sprite_items::bhv_conductor_arm_r.tiles_item(), 0, 1, 2, 1, 0, 3);
		_anim_tail = bn::create_sprite_animate_action_forever(*_spr_tail.get(), 6, bn::sprite_items::bhv_conductor_tail.tiles_item(), 0, 1, 2);
	}

}
