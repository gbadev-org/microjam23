#include "dh_candy.h"

#include "bn_math.h"
#include "bn_sprite_double_size_mode.h"

#include "bn_sprite_items_dh_candy.h"
#include "bn_sprite_items_dh_candy_icon.h"

#include "animations/dh_part_2_intro_palettes.h"

DH_START_NAMESPACE

candy::candy():
	candy_sprite(bn::sprite_items::dh_candy.create_sprite(0, 0)),
	candy_icon_sprite(bn::sprite_items::dh_candy_icon.create_sprite(0, 0))
{
	candy_icon_sprite.put_above();
	candy_icon_sprite.set_double_size_mode(bn::sprite_double_size_mode::ENABLED);
}

void candy::set_x(bn::fixed v) {
	candy_sprite.set_x(v);
	candy_icon_sprite.set_x(v - 2);
}

void candy::set_y(bn::fixed v) {
	candy_sprite.set_y(v);
	candy_icon_sprite.set_y(v - 2);
}

void candy::set_rotation(bn::fixed v) {
	candy_sprite.set_tiles(bn::sprite_items::dh_candy.tiles_item(), ((v / 180.0) * 9.0).floor_integer() % 9);
	candy_icon_sprite.set_rotation_angle(v.floor_integer() % 360);
}

void candy::set_scale(bn::fixed v) {
	candy_sprite.set_scale(v);
	candy_icon_sprite.set_scale(v);
}

void candy::set_visible(bool show) {
	candy_sprite.set_visible(show);
	candy_icon_sprite.set_visible(show);
}

void candy::move_to_top() {
	candy_sprite.set_bg_priority(1);
	candy_icon_sprite.set_bg_priority(1);
	candy_icon_sprite.put_above();
}

void candy::move_to_bottom() {
	candy_icon_sprite.put_below();
	candy_sprite.put_below();
}

void candy::set_candy_type(int type) {
	if(type < 0 || type > animations::max_icon_palette) {
		type = 0;
	}

	candy_type = type;
	candy_icon_sprite.set_tiles(bn::sprite_items::dh_candy_icon.tiles_item(), candy_type);
}

void candy::set_position(bn::fixed x, bn::fixed y, bn::fixed rotation, bn::fixed scale) {
	DH_UNUSED(scale);

	set_x(x);
	set_y(y);

	// ---------
	// Wrapper
	candy_sprite.set_tiles(bn::sprite_items::dh_candy.tiles_item(), ((rotation / 180.0) * 9.0).floor_integer() % 9);
	// candy_sprite.set_scale(scale);

	// ---------
	// Icon
	candy_icon_sprite.set_rotation_angle(rotation);
	// candy_icon_sprite.set_scale(scale * 2.0);
}

bool candy::check_press(bn::fixed test_x, bn::fixed test_y) {
	// Radius of the candy hitbox
	constexpr int size = 20;

	bn::fixed xo = test_x - candy_sprite.x() + (size / 2);
	if(xo > size || xo < -size) return false;

	bn::fixed yo = test_y - candy_sprite.y() + (size / 2);
	if(yo > size || yo < -size) return false;

	return bn::sqrt(xo * xo + yo * yo) < size;
}

void candy::set_sprite_palette(const bn::sprite_palette_ptr& palette) {
	candy_sprite.set_palette(palette);
}

void candy::randomize_type(bn::random& random, int taken, allow_type_similarity similarity_type) {
	int base_taken_type = taken / 3;
	int result_type;

	switch(similarity_type) {
		case allow_type_similarity::PREVENT: {
			do {
				result_type = random.get_int(animations::max_icon_palette + 1);
			} while((result_type / 3) == base_taken_type);
			break;
		}
		case allow_type_similarity::ALLOW: {
			do {
				result_type = random.get_int(animations::max_icon_palette + 1);
			} while(result_type == taken);
			break;
		}
		case allow_type_similarity::FORCE: {
			int relative_taken_type = taken % 3;
			int relative_result;
			do {
				relative_result = random.get_int(3);
			} while(relative_result == relative_taken_type);
			result_type = (base_taken_type * 3) + relative_result;
			break;
		}
		default: {
			result_type = taken == 0 ? 3 : 0;
		}
	}

	set_candy_type(result_type);
}

void candy::randomize_position(bn::random& random) {
	position_index = random.get_int(tiles_width * tiles_height);
	refresh_position_from_index(random);

	// Hardcoded approximate boundaries of the "candy bag" background.
	// set_position(random.get_int(-100, 66), random.get_int(-12, 66), random.get_int(360), random.get_fixed(0.5) + 0.5);
}

void candy::randomize_unique_position(bn::random& random, const bn::ideque<candy>& existing_candy) {
	while(true) {
		randomize_position(random);

		bool overlaps = false;

		// Make sure no overlap with past 6 pieces
		int start = bn::max(0, existing_candy.size() - 6);
		int end = existing_candy.size();
		for(int i = start; i < end; i++) {
			const candy& c = existing_candy[i];
			if(
				bn::abs(c.position_index - position_index) <= 2 ||
				(c.position_index - tiles_width) == position_index ||
				(c.position_index + tiles_width) == position_index
			) {
				overlaps = true;
				break;
			}
		}

		if(!overlaps) {
			break;
		}
	}
}

void candy::refresh_position_from_index(bn::random& random) {
	set_position(
		((position_index % tiles_width) * 16) - 100,
		((position_index / tiles_width) * 16) - 12,
		random.get_int(360),
		random.get_fixed(0.5) + 0.5
	);
}

DH_END_NAMESPACE
