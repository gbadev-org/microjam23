#include "dh_impl2.h"

#include "bn_keypad.h"
#include "bn_random.h"
#include "bn_sprite_palette_item.h"

#include "bn_sound_items.h"

#include "animations/dh_part_2_intro_palettes.h"

DH_START_NAMESPACE

impl2::impl2() {
}

void impl2::init(camera& cam, const mj::game_data& data) {
	candy_type = data.random.get_int(animations::max_icon_palette + 1);
	cam.set_candy_type(candy_type);

	bn::sound_items::dh_doorbell.play();
}

void impl2::start_playing(camera& cam, mj::difficulty_level level, int completed_games, const mj::game_data& data) {
	// Setup camera
	cam.clear_foregrounds();

	// Setup candy
	generate_palettes();
	generate_candy(level, completed_games, data);

	// Setup hand
	cursor.start_intro();
}

void impl2::generate_palettes() {
	#define b bn::color(0, 0, 0)

	constexpr bn::color unselected_palette_colors[] = {
		b, b, b, bn::color(31, 31, 31), b, b, b, b, b, b, b, b, b, b, b, b
	};
	constexpr bn::color selected_palette_colors[] = {
		b, bn::color(31, 26, 0), b, bn::color(31, 31, 31), b, b, b, b, b, b, b, b, b, b, b, b
	};

	#undef b

	unselected_palette = bn::sprite_palette_item(unselected_palette_colors, bn::bpp_mode::BPP_4).create_palette();
	selected_palette = bn::sprite_palette_item(selected_palette_colors, bn::bpp_mode::BPP_4).create_palette();
	display_palette = bn::sprite_palette_item(unselected_palette_colors, bn::bpp_mode::BPP_4).create_new_palette();
}

void impl2::generate_candy(mj::difficulty_level level, int completed_games, const mj::game_data& data) {
	bn::random& random = data.random;

	{
		candy c;
		c.set_candy_type(candy_type);
		c.randomize_position(random);
		candy_objects.push_back(bn::move(c));
	}

	constexpr int super_hard_threshold = 9 * 4;

	int candy_count = 12;//1;
	allow_type_similarity type_similarity = allow_type_similarity::PREVENT;
	
	int level_number = static_cast<int>(level);
	if(completed_games > super_hard_threshold) {
		level_number = 4;
	}

	switch(level_number) {
		case 0: { //easy
			candy_count = 2 + random.get_int(4);
			break;
		}
		case 1: { // normal
			candy_count = 6 + random.get_int(4);
			type_similarity = allow_type_similarity::ALLOW;
			break;
		}
		case 2: { // hard
			if(random.get_int(2) == 0) {
				candy_count = 4 + random.get_int(2);
				type_similarity = allow_type_similarity::FORCE;
			} else {
				candy_count = 10 + random.get_int(2);
				type_similarity = allow_type_similarity::ALLOW;
			}
			break;
		}
		case 4: {
			candy_count = 8 + random.get_int(4);
			type_similarity = allow_type_similarity::FORCE;
			break;
		}
		default: {
		}
	}

	// auto len = random.get_int(20) + 4;
	DH_FOR(candy_count - 1) {
		candy c;
		c.randomize_type(random, candy_type, type_similarity);
		c.randomize_unique_position(random, candy_objects);
		c.set_sprite_palette(unselected_palette.value());
		candy_objects.push_back(bn::move(c));
	}
}

int impl2::update() {
	if(cursor.is_actively_grabbing()) {

		cursor.update();

	} else if(is_displaying_candy) {

		if(update_candy_display_animation()) {
			return 3;
		}

	} else if(picked_result != 0) {

		if(picked_result == 1) {
			bn::sound_items::dh_success.play(0.9);
		} else {
			bn::sound_items::dh_incorrect.play(0.9);
		}

		init_candy_display_animation();
		// cursor.set_visible(false);
		return picked_result;

	} else {

		update_hovered_candy();

		if(cursor.is_actively_pressing()) {
			if(pushing_candy != nullptr) {
				pushing_candy->set_scale(cursor.get_push_ratio());
			}
		} else if(!cursor.is_actively_grabbing()) {
			cursor.update_movement();
			if(update_a_press()) {
				grab_hovered_candy();
			} else if(update_b_press()) {
				push_down_hovered_candy();
			}
		}

		if(!cursor.is_actively_pressing() && pushing_candy != nullptr) {
			pushing_candy->set_scale(1);
			pushing_candy = nullptr;
		}

		cursor.update();
	}

	return 0;
}

void impl2::update_hovered_candy() {
	auto hand_x = cursor.x();
	auto hand_y = cursor.y();
	candy* new_hovered_candy = nullptr;
	for(auto i = candy_objects.rbegin(); i != candy_objects.rend(); i++) {
		if((*i).check_press(hand_x, hand_y)) {
			new_hovered_candy = &(*i);
			break;
		}
	}

	if(hovered_candy != new_hovered_candy) {
		if(hovered_candy != nullptr) {
			hovered_candy->set_sprite_palette(unselected_palette.value());
		}
		if(new_hovered_candy != nullptr) {
			new_hovered_candy->set_sprite_palette(selected_palette.value());
		}
		hovered_candy = new_hovered_candy;
	}
}

bool impl2::update_a_press() {
	if(bn::keypad::a_pressed()) {
		cursor.grab();
		bn::sound_items::dh_fall.play();
		if(hovered_candy != nullptr) {
			return true;
		}
	}
	return false;
}

void impl2::grab_hovered_candy() {
	if(hovered_candy != nullptr) {
		picked_result = hovered_candy->get_candy_type() == candy_type ? 1 : 2;
	}
}

bool impl2::update_b_press() {
	if(bn::keypad::b_pressed()) {
		cursor.press();
		if(hovered_candy != nullptr) {
			return true;
		}
	}
	return false;
}

void impl2::push_down_hovered_candy() {
	if(hovered_candy != nullptr) {
		pushing_candy = hovered_candy;
		
		bn::sound_items::dh_fall.play();

		auto it = candy_objects.begin();
		auto end = candy_objects.end();
		while(it != end) {
			if(pushing_candy == &(*it)) {
				break;
			}
			++it;
		}

		if(it != end) {
			candy temp = (*pushing_candy);
			candy_objects.erase(it);
			candy_objects.push_front(temp);
			temp.move_to_bottom();
			pushing_candy = &candy_objects.front();
		}
	}
}

void impl2::init_candy_display_animation() {
	is_displaying_candy = true;

	hovered_candy->move_to_top();
	hovered_candy->set_sprite_palette(display_palette.value());

	animation_time = 0;
	start_x = hovered_candy->x();
	start_y = hovered_candy->y();
	start_rotation = hovered_candy->rotation();

	// If it's not going to spin that much, let's make it SPINNNNNN
	if(start_rotation < 180) start_rotation += 360;
}

bool impl2::update_candy_display_animation() {
	if(animation_time < 1.0) {
		animation_time += 0.02;
		if(animation_time > 1.0) animation_time = 1.0;
	}

	auto r = (animation_time - 1) * (animation_time - 1) * (animation_time - 1) + bn::fixed(1.0);

	unselected_palette->set_fade_intensity(animation_time * bn::fixed(0.5));

	hovered_candy->set_x(start_x + (-start_x * r));
	hovered_candy->set_y(start_y + (-start_y * r));
	hovered_candy->set_rotation(start_rotation + (-start_rotation * r));
	hovered_candy->set_scale(1.0 + r);

	return r == 1.0;
}

void impl2::on_pause_start() {
	for(auto& c : candy_objects) {
		c.set_visible(false);
	}
}

void impl2::on_pause_end() {
	for(auto& c : candy_objects) {
		c.set_visible(true);
	}
}

DH_END_NAMESPACE
