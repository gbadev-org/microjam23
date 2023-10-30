#ifndef BHV_CAT_H
#define BHV_CAT_H

#include "bn_optional.h"
#include "bn_sprite_animate_actions.h"
#include "bhv_entity.h"

namespace bhv
{
	

	class bhv_cat : public bhv_entity
	{
	public:
		bhv_cat(bn::fixed_point pos);
		void update_anim() override;
		void set_wait_updates(int frames) override;
		void set_anim_state(anim_state state) override;

	private:
		bn::optional<bn::sprite_animate_action<6>> _anim_idle;
		bn::optional<bn::sprite_animate_action<3>> _anim_sing;
		bn::optional<bn::sprite_animate_action<2>> _anim_lose;
		bn::optional<bn::sprite_animate_action<3>> _anim_win;
	};
}

#endif
