#include "dh_controls.h"

#include "bn_sprite_items_dh_control_pad.h"
#include "bn_sprite_items_dh_a_button.h"
#include "bn_sprite_items_dh_b_button.h"

#include "bn_sprite_items_dh_look_icon.h"
#include "bn_sprite_items_dh_forward_icon.h"
#include "bn_sprite_items_dh_backward_icon.h"
#include "bn_sprite_items_dh_press_icon.h"

DH_START_NAMESPACE

namespace {
	constexpr int offset = 32;
	constexpr int icon_offset = 48;

	constexpr const char* part_2_a_text = "Grab Candy";
	constexpr const char* part_2_b_text = "Push Down";
}

controls::controls(bool is_part_2, const bn::sprite_text_generator& text_generator):
	a_button(bn::sprite_items::dh_a_button.create_sprite(0, 0))
{
	DH_UNUSED(is_part_2);

	if(is_part_2) {

		constexpr int base_x = 0;
		constexpr int base_y = -60;

		auto a_x = base_x - 40;
		a_button.set_x(a_x);
		a_button.set_y(base_y);

		auto b_x = base_x + 40;
		b_button = bn::sprite_items::dh_b_button.create_sprite(b_x, base_y);

		auto text_y = base_y + 15;
		text_generator.generate(a_x - (text_generator.width(part_2_a_text) / 2), text_y, part_2_a_text, text_1);
		text_generator.generate(b_x - (text_generator.width(part_2_b_text) / 2), text_y, part_2_b_text, text_1);

	} else {
		// ------------------------
		// CONTROL PAD

		constexpr int pad_x = -45;
		constexpr int pad_y = -35;

		control_pad = bn::sprite_items::dh_control_pad.create_sprite(pad_x - offset, pad_y);
		up_sprite = bn::sprite_items::dh_forward_icon.create_sprite(pad_x - offset, pad_y - 24);
		down_sprite = bn::sprite_items::dh_backward_icon.create_sprite(pad_x - offset, pad_y + 30);
		left_sprite = bn::sprite_items::dh_look_icon.create_sprite(pad_x - 60, pad_y + 2);

		right_sprite = bn::sprite_items::dh_look_icon.create_sprite(pad_x - 4, pad_y + 2);
		right_sprite->set_horizontal_flip(true);

		// ------------------------
		// A BUTTON

		constexpr int a_x = 40;
		constexpr int a_y = -35;

		a_button.set_x(a_x + offset);
		a_button.set_y(a_y);

		a_sprite = bn::sprite_items::dh_press_icon.create_sprite(a_x + 56, a_y - 2);
	}
}

DH_END_NAMESPACE
