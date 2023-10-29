#pragma once

#include "bn_regular_bg_ptr.h"
#include "bn_bg_palette_ptr.h"
#include "bn_optional.h"

#include "dh_defines.h"

DH_START_NAMESPACE

class camera {
public:
	camera(bool is_part_2);

	[[nodiscard]] inline int get_x() const { return x; }
	[[nodiscard]] inline int get_y() const { return y; }
	[[nodiscard]] inline int get_is_part_2() const { return is_part_2; }

	[[nodiscard]] bn::regular_bg_item const* initial_background(bool _is_part_2) const;
	[[nodiscard]] bn::regular_bg_item const* initial_foreground(bool _is_part_2) const;
	void create_foreground(bool _is_part_2);

	void set_palette_type(int type);
	void set_doorbell_position(int pos);

	void set_candy_type(int type);

	void set_frame_index(int index);
	void set_frame_index_part_1(int index);
	void set_frame_index_part_2(int index);

	[[nodiscard]] int get_intro_frame_count() const;
	[[nodiscard]] bool update_intro();
	[[nodiscard]] bool update_intro_part_1();
	[[nodiscard]] bool update_intro_part_2();
	[[nodiscard]] bool should_update_hand_intro() const;

	[[nodiscard]] bool set_position(int _x, int _y);
	void refresh_position();

	void restrict_movement(int& _x, int& _y) const;
	[[nodiscard]] bool shift(int _x, int _y);

	[[nodiscard]] bool on_a_press();
	[[nodiscard]] bool update_animation();
	[[nodiscard]] bool animation_done();

	void play_animation_done_sound_effect();

	void clear_foregrounds();

	void show_overlay();
	void hide_overlay();

	static constexpr int world_width = 11;
	static constexpr int world_height = 13;

private:
	bn::optional<bn::regular_bg_ptr> background_bg;
	bn::optional<bn::regular_bg_ptr> foreground_bg;
	bn::optional<bn::regular_bg_ptr> _overlay_bg;

	// Used to set icon on phone for part 2 micro game
	bn::optional<bn::bg_palette_ptr> phone_icon_palette;

	// ------------------
	// Part 1
	const bn::regular_bg_item* const* doorbell_frames;
	int doorbell_first_index = 0;
	int doorbell_position = 0;

	int palette_type = 0;

	int current_overlay_bg_index = -1;

	int x = 3;
	int y = 13;

	int intro_time = 0;
	int intro_frame = 0;

	// ------------------
	// Part 2
	int candy_type = 0;

	// ------------------
	// Animations
	const bn::regular_bg_item* const* animation = nullptr;
	int const* animation_data = nullptr;
	int animation_frame = 0;
	int animation_time = 0;

	// ------------------
	// Booleans
	bool is_part_2;

	bool overlay_was_shown = false;
};

DH_END_NAMESPACE
