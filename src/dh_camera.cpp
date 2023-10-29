#include "dh_camera.h"

#include "bn_bpp_mode.h"
#include "bn_bg_palette_item.h"
#include "bn_bg_palette_ptr.h"
#include "bn_color.h"
#include "bn_core.h"
#include "bn_sound_items.h"

#include "bn_bg_palette_items_dh_background_alt_palette.h"
#include "bn_bg_palette_items_dh_foreground_alt_palette.h"
#include "bn_bg_palette_items_dh_pumpkin_bell_press_alt_background_palette.h"
#include "bn_bg_palette_items_dh_pumpkin_bell_press_alt_foreground_palette.h"

#include "frames/dh_intro.h"
#include "frames/dh_high_bell.h"
#include "frames/dh_window_bell.h"
#include "frames/dh_right_bell.h"
#include "frames/dh_pumpkin_bell.h"
#include "frames/dh_close_bell.h"
#include "frames/dh_close_left_bell.h"

#include "animations/dh_close_bell_press.h"
#include "animations/dh_close_left_bell_press.h"
#include "animations/dh_right_bell_press.h"
#include "animations/dh_pumpkin_bell_press.h"
#include "animations/dh_high_bell_press.h"
#include "animations/dh_window_bell_press.h"

#include "animations/dh_part_2_intro.h"
#include "animations/dh_part_2_intro_palettes.h"

DH_START_NAMESPACE

namespace {
	constexpr int bg_x = (256 - 240) / 2;
	constexpr int bg_y = (256 - 160) / 2;
}

camera::camera(bool _is_part_2):
	background_bg(initial_background(_is_part_2)->create_bg(bg_x, bg_y)),
	foreground_bg(initial_foreground(_is_part_2)->create_bg(bg_x, bg_y)),
	is_part_2(_is_part_2)
{
	foreground_bg->set_priority(3);
}

bn::regular_bg_item const* camera::initial_background(bool _is_part_2) const {
	return _is_part_2 ? animations::dh_part_2_intro_back_frames[0] : background_frames[0];
}

bn::regular_bg_item const* camera::initial_foreground(bool _is_part_2) const {
	return _is_part_2 ? animations::dh_part_2_intro_frames[0] : frames[0];
}

void camera::set_palette_type(int type) {
	palette_type = type;
}

void camera::set_doorbell_position(int pos) {
	doorbell_position = pos;

	switch(pos) {
		case 0: {
			doorbell_frames = dh_high_bell_frames;
			doorbell_first_index = dh_high_bell_first_index;
			break;
		}
		case 1: {
			doorbell_frames = dh_right_bell_frames;
			doorbell_first_index = dh_right_bell_first_index;
			break;
		}
		case 2: {
			doorbell_frames = dh_close_bell_frames;
			doorbell_first_index = dh_close_bell_first_index;
			break;
		}
		case 3: {
			doorbell_frames = dh_close_left_bell_frames;
			doorbell_first_index = dh_close_left_bell_first_index;
			break;
		}
		case 4: {
			doorbell_frames = dh_pumpkin_bell_frames;
			doorbell_first_index = dh_pumpkin_bell_first_index;
			break;
		}
		case 5: {
			doorbell_frames = dh_window_bell_frames;
			doorbell_first_index = dh_window_bell_first_index;
			break;
		}
		default: {}
	}

	_overlay_bg = doorbell_frames[doorbell_first_index]->create_bg(bg_x, bg_y);
	_overlay_bg->set_visible(false);
}

void camera::set_candy_type(int type) {
	if(type < 0 || type > animations::max_icon_palette) {
		type = 0;
	}

	phone_icon_palette = animations::get_palette_from_icon(type).create_palette();

	constexpr auto start = animations::dh_part_2_intro_phone_frames_start;
	_overlay_bg = animations::dh_part_2_intro_phone_frames[start]->create_bg(bg_x, bg_y);
	_overlay_bg->set_priority(2);
	_overlay_bg->set_visible(false);
}

void camera::set_frame_index(int index) {
	if(is_part_2) {
		set_frame_index_part_2(index);
	} else {
		set_frame_index_part_1(index);
	}
}

void camera::set_frame_index_part_1(int index) {
	if(_overlay_bg && (animation == nullptr || animation_frame == -1)) {
		if(doorbell_frames[index] == nullptr) {
			_overlay_bg->set_visible(false);
		} else {
			if(current_overlay_bg_index != index) {
				current_overlay_bg_index = index;
				_overlay_bg->set_item(*doorbell_frames[index]);
			}
			_overlay_bg->set_visible(true);
		}
	}

	foreground_bg->set_item(*frames[index]);
	background_bg->set_item(*background_frames[index]);

	if(palette_type == 1) {
		foreground_bg->set_palette(bn::bg_palette_items::dh_foreground_alt_palette);
	} else if(palette_type == 2) {
		background_bg->set_palette(bn::bg_palette_items::dh_background_alt_palette);
	}
}

void camera::set_frame_index_part_2(int index) {
	DH_UNUSED(index);
}

int camera::get_intro_frame_count() const {
	return is_part_2 ? animations::dh_part_2_intro_length : intro_frame_count;
}

bool camera::update_intro() {
	if(is_part_2) {
		return update_intro_part_2();
	} else {
		return update_intro_part_1();
	}
}

bool camera::update_intro_part_1() {
	if(intro_time++ > 2) {
		intro_time = 0;
		intro_frame++;
		if(intro_frame < get_intro_frame_count()) {
			set_frame_index(intro_frame);
		}
	}
	return intro_frame >= get_intro_frame_count();
}

bool camera::update_intro_part_2() {
	// if(intro_frame == 20 && animation_time == 0) {
	// 	if(bn::keypad::a_pressed()) {
	// 		animation_time = 1;
	// 		intro_time = 0;
	// 	} else {
	// 		return false;
	// 	}
	// }

	if(intro_frame >= get_intro_frame_count()) {
		return true;
	}

	intro_time++;
	if(intro_time == 2) {
		if(foreground_bg) {
			auto fg_frame = animations::dh_part_2_intro_frames[intro_frame];
			if(fg_frame != nullptr) {
				foreground_bg->set_item(*fg_frame);
				foreground_bg->set_visible(true);
			} else {
				foreground_bg->set_visible(false);
			}
		}

		if(_overlay_bg) {
			auto item = animations::dh_part_2_intro_phone_frames[intro_frame];
			if(item != nullptr) {
				_overlay_bg->set_item(*item);
				_overlay_bg->set_visible(true);
				_overlay_bg->set_palette(phone_icon_palette.value());
			} else if(_overlay_bg->visible()) {
				_overlay_bg.reset();
			}
		}
	} else if(intro_time == 4) {
		intro_time = 0;
		background_bg->set_item(*animations::dh_part_2_intro_back_frames[intro_frame]);

		if(intro_frame == 16) {
			bn::sound_items::dh_door_open.play();
		} else if(intro_frame == 18) {
			bn::sound_items::dh_wood_squeak.play();
		}

		intro_frame++;
	}

	return intro_frame >= get_intro_frame_count();
}

bool camera::should_update_hand_intro() const {
	return intro_frame >= 13;
}

bool camera::set_position(int _x, int _y) {
	bool changed = false;

	if(_x >= 0 && _x < world_width) {
		x = _x;
		changed = true;
	}
	if(_y >= 0 && _y < world_height) {
		y = _y;
		changed = true;
	}

	if(changed) {
		refresh_position();
	}

	return changed;
}

void camera::refresh_position() {
	int y_off = y % 4;
	int y_sec = y / 4;
	int index;
	if(y_off == 0) {
		index = ((4 - y_sec - 1) * (world_width + 3)) + x;
	} else {
		index = ((4 - y_sec - 1) * (world_width + 3)) - y_off;
	}
	set_frame_index(game_frame_start + index);
}

void camera::restrict_movement(int& _x, int& _y) const {
	constexpr int world_width_2 = (world_width / 2);

	if(_x != 0) {
		auto y_off = y % 4;
		if(y_off != 0) {
			_y = y_off <= 1 ? -1 : 1;
			_x = 0;
		}
	}

	auto new_y = y + _y;
	if(_y != 0 && new_y >= 0 && new_y < world_height) {
		if(x > world_width_2) {
			_x = -1;
			_y = 0;
		} else if(x < world_width_2) {
			_x = 1;
			_y = 0;
		}
	}
}

bool camera::shift(int _x, int _y) {
	return set_position(x + _x, y + _y);
}

bool camera::on_a_press() {
	if(_overlay_bg && _overlay_bg->visible()) {
		switch(doorbell_position) {
			case 0: {
				animation = animations::dh_high_bell_press_frames;
				animation_data = animations::dh_high_bell_press_data;
				break;
			}
			case 1: {
				animation = animations::dh_right_bell_press_frames;
				animation_data = animations::dh_right_bell_press_data;
				break;
			}
			case 2: {
				animation = animations::dh_close_bell_press_frames;
				animation_data = animations::dh_close_bell_press_data;
				break;
			}
			case 3: {
				animation = animations::dh_close_left_bell_press_frames;
				animation_data = animations::dh_close_left_bell_press_data;
				break;
			}
			case 4: {
				animation = animations::dh_pumpkin_bell_press_frames;
				animation_data = animations::dh_pumpkin_bell_press_data;
				break;
			}
			case 5: {
				animation = animations::dh_window_bell_press_frames;
				animation_data = animations::dh_window_bell_press_data;
				break;
			}
			default: {}
		}

		// Can only press when on the corrent "Y" position
		if(animation_data[2] != (world_height - y - 1)) {
			animation = nullptr;
			animation_data = nullptr;
			return false;
		}

		animation_frame = -1;
		animation_time = 3;
		return true;
	}
	return false;
}

bool camera::update_animation() {
	if(animation == nullptr) {
		return true;
	}

	if(--animation_time == 0) {
		animation_time = 3;

		// Align "X" before playing animation
		if(x != animation_data[1]) {
			(void)shift(animation_data[1] > x ? 1 : -1, 0);
		} else {
			animation_frame++;

			// Play doorbell sound effect at certain time for some press animations
			if(animation_frame == 5) {
				if(animation == animations::dh_close_bell_press_frames) {
					bn::sound_items::dh_doorbell.play();
				} else if(
					animation == animations::dh_close_left_bell_press_frames ||
					animation == animations::dh_pumpkin_bell_press_frames
				) {
					bn::sound_items::dh_fall.play();
				}
			}

			if(!animation_done() && _overlay_bg) {
				// For pumpkin bell press, let's remove the other backgrounds...
				if(animation_frame == 0 && animation == animations::dh_pumpkin_bell_press_frames) {
					background_bg.reset();
					foreground_bg.reset();
				}

				_overlay_bg->set_item(*animation[animation_frame]);
				_overlay_bg->set_visible(true);

				// Replace palette for pumpkin bell press animation
				if(animation == animations::dh_pumpkin_bell_press_frames) {
					if(palette_type == 1) {
						_overlay_bg->set_palette(bn::bg_palette_items::dh_pumpkin_bell_press_alt_foreground_palette);
					} else if(palette_type == 2) {
						_overlay_bg->set_palette(bn::bg_palette_items::dh_pumpkin_bell_press_alt_background_palette);
					}
				}
			}
		}
	}

	return false;
}

void camera::play_animation_done_sound_effect() {
	if(animation == animations::dh_close_bell_press_frames) {
		bn::sound_items::dh_fall.play();
	} else {
		bn::sound_items::dh_doorbell.play();
	}
}

bool camera::animation_done() {
	return animation_frame >= animation_data[0];
}

void camera::clear_foregrounds() {
	if(foreground_bg) foreground_bg.reset();
	if(_overlay_bg) _overlay_bg.reset();
}

void camera::show_overlay() {
	if(overlay_was_shown) {
		_overlay_bg->set_visible(true);
	}
}

void camera::hide_overlay() {
	if(_overlay_bg && _overlay_bg->visible()) {
		overlay_was_shown = true;
		_overlay_bg->set_visible(false);
	} else {
		overlay_was_shown = false;
	}
}

DH_END_NAMESPACE
