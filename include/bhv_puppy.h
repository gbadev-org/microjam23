#ifndef BHV_PUPPY_H
#define BHV_PUPPY_H

#include "bn_optional.h"
#include "bn_sprite_animate_actions.h"
#include "bhv_entity.h"

namespace bhv
{
	class bhv_puppy : public bhv_entity
	{
	public:
		bhv_puppy();
		void update_anim() override;
		void set_wait_updates(int frames) override;
		void set_anim_state(anim_state state) override;

	private:
		bn::optional<bn::sprite_animate_action<4>> _anim_idle;
		bn::optional<bn::sprite_animate_action<3>> _anim_sing;
		bn::optional<bn::sprite_animate_action<3>> _anim_win;
	};
}

#endif
