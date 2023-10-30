#pragma once

#include "bn_sprite_ptr.h"
#include "bn_fixed.h"

#include "dh_defines.h"

DH_START_NAMESPACE

class hand {
public:
	hand();

	[[nodiscard]] inline bn::fixed x() const { return hand_sprite.x() - 21; }
	[[nodiscard]] inline bn::fixed y() const { return hand_sprite.y() - 21; }

	inline void set_visible(bool v) { hand_sprite.set_visible(v); }

	void add_x(bn::fixed x);
	void add_y(bn::fixed y);

	void update();
	void update_normal();
	void update_press();
	void update_grab();

	void update_movement();

	void start_intro();
	bool update_intro();

	void set_frame(int frame);

	void press();
	bool is_actively_pressing() const;

	void grab();
	bool is_actively_grabbing() const;

	bn::fixed get_push_ratio() const;

private:
	bn::sprite_ptr hand_sprite;

	int time = 0;
	int frame = 0;

	int is_pressing = 0;
	int is_grabbing = 0;

	int move_camera_x = 0;
	int move_camera_y = 0;
};

DH_END_NAMESPACE
