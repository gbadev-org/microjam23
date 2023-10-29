#pragma once

#include "mj/mj_game.h"
#include "mj/mj_difficulty_level.h"

#include "dh_defines.h"
#include "dh_camera.h"

DH_START_NAMESPACE

/**
 * The implementation for "Door Hell" (the part 1 version).
 * 
 * This contains behavior and fields related to the first micro game.
 */
class impl1 {
public:
	impl1();

	void setup_palette(camera& cam, int progress);

	void on_first_update(camera& cam, mj::difficulty_level level, const mj::game_data& data);
	int generate_unique_random_position(mj::difficulty_level level, const mj::game_data& data) const;

	[[nodiscard]] bool update(camera& cam, bool is_victory);
	void update_controls(camera& cam);

	void update_movement(camera& cam);
	void update_a_pressed(camera& cam);

	void on_pause_start(camera& cam);
	void on_pause_end(camera& cam);

private:
	int sleep = 0;

	int camera_move_cooldown = 0;
	int footstep_cooldown = 0;

	int last_move_type = -1;
	int footstep_type = 0;
};

DH_END_NAMESPACE
