#pragma once

#include "bn_fixed.h"
#include "bn_random.h"
#include "bn_sprite_ptr.h"
#include "bn_deque.h"

#include "dh_defines.h"

DH_START_NAMESPACE

enum allow_type_similarity : uint8_t {
	PREVENT,
	ALLOW,
	FORCE
};

class candy {
public:
	candy();

	[[nodiscard]] inline bn::fixed x() const { return candy_sprite.x(); }
	[[nodiscard]] inline bn::fixed y() const { return candy_sprite.y(); }
	[[nodiscard]] inline bn::fixed rotation() const { return candy_icon_sprite.rotation_angle(); }
	[[nodiscard]] inline int get_candy_type() const { return candy_type; }

	void set_x(bn::fixed v);
	void set_y(bn::fixed v);
	void set_rotation(bn::fixed v);
	void set_scale(bn::fixed v);
	void set_visible(bool show);
	void move_to_top();
	void move_to_bottom();

	void set_candy_type(int type);
	void set_position(bn::fixed x, bn::fixed y, bn::fixed rotation, bn::fixed scale);
	[[nodiscard]] bool check_press(bn::fixed x, bn::fixed y);

	void set_sprite_palette(const bn::sprite_palette_ptr& palette);

	void randomize_type(bn::random& random, int taken, allow_type_similarity similarity_type);
	void randomize_position(bn::random& random);
	void randomize_unique_position(bn::random& random, const bn::ideque<candy>& existing_candy);

	void refresh_position_from_index(bn::random& random);

	constexpr static int tiles_width = 10;
	constexpr static int tiles_height = 5;

private:
	bn::sprite_ptr candy_sprite;
	bn::sprite_ptr candy_icon_sprite;

	int candy_type = -1;
	int position_index = 0;
};

DH_END_NAMESPACE
