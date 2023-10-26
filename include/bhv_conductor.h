#ifndef BHV_CONDUCTOR_H
#define BHV_CONDUCTOR_H

#include "bn_optional.h"
#include "bn_sprite_ptr.h"
#include "bn_sprite_animate_actions.h"
#include "bhv_entity.h"

namespace bhv
{
	class bhv_conductor : public bhv_entity
	{
	public:
		bhv_conductor();
		~bhv_conductor();
		void set_position(bn::fixed_point pos) override;
		void update_anim() override;
		void set_wait_updates(int frames) override;
		void set_anim_idle();
		void set_anim_lose();

	private:
		bn::optional<bn::sprite_ptr> _spr_body;
		bn::optional<bn::sprite_ptr> _spr_arm_l;
		bn::optional<bn::sprite_ptr> _spr_arm_r;
		bn::optional<bn::sprite_ptr> _spr_tail;
		bn::optional<bn::sprite_animate_action<6>> _anim_head;
		bn::optional<bn::sprite_animate_action<6>> _anim_body;
		bn::optional<bn::sprite_animate_action<6>> _anim_arm_l;
		bn::optional<bn::sprite_animate_action<6>> _anim_arm_r;
		bn::optional<bn::sprite_animate_action<3>> _anim_tail;
	};
}

#endif
