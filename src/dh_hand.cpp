#include "dh_hand.h"

#include "bn_keypad.h"
#include "bn_math.h"

#include "bn_sprite_double_size_mode.h"
#include "bn_sprite_items_dh_hand.h"

DH_START_NAMESPACE

namespace {
	constexpr int screen_min_x = (240 / -2);
	constexpr int screen_max_x = (240 / 2);
	constexpr int screen_min_y = (160 / -2);
	constexpr int screen_max_y = (160 / 2);

	bn::fixed clamp(bn::fixed value, int min, int max) {
		if(value < min) return min;
		else if(value > max) return max;
		return value;
	}
}

hand::hand():
	hand_sprite(bn::sprite_items::dh_hand.create_sprite(0, 0))
{
	hand_sprite.set_double_size_mode(bn::sprite_double_size_mode::ENABLED);
	hand_sprite.set_visible(false);
	hand_sprite.set_bg_priority(1);
}

void hand::add_x(bn::fixed x) {
	auto v = clamp(hand_sprite.x() + x, screen_min_x, screen_max_x);
	hand_sprite.set_x(v);
	move_camera_x = v == screen_min_x ? -1 : (v == screen_max_x ? 1 : 0);
}

void hand::add_y(bn::fixed y) {
	auto v = clamp(hand_sprite.y() + y, screen_min_y, screen_max_y);
	hand_sprite.set_y(v);
	move_camera_y = v == screen_min_y ? -1 : (v == screen_max_y ? 1 : 0);
}

void hand::update() {
	if(is_pressing == -1) {
		update_intro();
	} else if(is_actively_pressing()) {
		update_press();
	} else if(is_actively_grabbing()) {
		update_grab();
	} else {
		update_normal();
	}
}

void hand::update_normal() {
	if(time++ > 12) {
		time = 0;

		frame = frame == 0 ? 1 : 0;
		set_frame(frame);
	}
}

void hand::update_press() {
	if(time++ >= 2) {
		time = 0;

		if(is_pressing == 1) {
			frame++;
			if(frame == 5) {
				is_pressing = 2;
				frame = 3;
			}
		} else {
			frame--;
			if(frame == 1) {
				is_pressing = 0;
			}
		}
	}
	set_frame(frame);
}

void hand::update_grab() {
	if(time++ >= 2) {
		time = 0;
		if(is_grabbing < 6) {
			is_grabbing++;
			set_frame((is_grabbing - 2) + 5);
		} else {
			is_grabbing = 0;
		}
	}
}

void hand::update_movement() {
	move_camera_x = 0;
	move_camera_y = 0;

	bn::fixed x = 0;
	bn::fixed y = 0;
	
	if(bn::keypad::left_held()) {
		x = -1;
	} else if(bn::keypad::right_held()) {
		x = 1;
	}

	if(bn::keypad::up_held()) {
		y = -1;
	} else if(bn::keypad::down_held()) {
		y = 1;
	}

	if(x != 0 && y != 0) {
		auto sqrt2 = bn::sqrt(bn::fixed(2.0));
		x /= sqrt2;
		y /= sqrt2;
	}

	constexpr bn::fixed speed = 3;

	add_x(x * speed);
	add_y(y * speed);
}

void hand::start_intro() {
	time = 20;
	is_pressing = -1;
	hand_sprite.set_visible(false);
}

bool hand::update_intro() {
	if(time > 0) {
		time--;

		// Easing bs
		bn::fixed ratio;
		if(time > 10) {
			ratio = (bn::fixed(10 - (time - 10)) / bn::fixed(10));
			ratio = (ratio - 1.0) * (ratio - 1.0) * (ratio - 1.0) + 1.0;
			ratio *= bn::fixed(2.0);
		} else {
			bn::fixed r = bn::fixed(10 - time) / bn::fixed(10);
			r = r * r * r;
			ratio = bn::fixed(2.0) - r * bn::fixed(1.0);
		}

		hand_sprite.set_visible(ratio != 0);
		if(ratio > 0) hand_sprite.set_scale(ratio);

		if(time == 0) {
			is_pressing = 0;
			return true;
		}
	}

	return false;
}

void hand::set_frame(int f) {
	hand_sprite.set_tiles(bn::sprite_items::dh_hand.tiles_item(), f);
}

void hand::press() {
	is_pressing = 1;
}

bool hand::is_actively_pressing() const {
	return is_pressing > 0;
}

void hand::grab() {
	is_grabbing = 1;
}

bool hand::is_actively_grabbing() const {
	return is_grabbing > 0;
}

bn::fixed hand::get_push_ratio() const {
	return bn::fixed(0.5) + ((bn::fixed(5 - frame) / 5.0) * bn::fixed(0.5));
}

DH_END_NAMESPACE
