#ifndef BHV_ENTITY_H
#define BHV_ENTITY_H

#include "bn_fixed_point.h"
#include "bn_optional.h"
#include "bn_sprite_ptr.h"

namespace bhv
{
	enum anim_state
	{
		BHV_ANIM_IDLE,
		BHV_ANIM_SING,
		BHV_ANIM_LOSE,
		BHV_ANIM_WIN,
	};

	class bhv_entity
	{
	public:
		virtual ~bhv_entity() = default;
		virtual void set_position(bn::fixed_point pos)
		{
			_pos = pos;
			if (_spr.has_value())
			{
				_spr.get()->set_position(pos);
			}
		}
		bn::fixed_point get_position() { return _pos; }
		virtual void update_anim() = 0;
		void set_flip(bool flip)
		{
			if (_spr.has_value())
			{
				_spr.get()->set_horizontal_flip(flip);
			}
		}
		virtual void set_wait_updates(int frames) = 0;
		virtual void set_anim_state(anim_state state)
		{
			_anim_state = state;
		}

	protected:
		bn::fixed_point _pos;
		bn::optional<bn::sprite_ptr> _spr;
		anim_state _anim_state;
	};
}

#endif
