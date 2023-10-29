#pragma once

#include "bn_fixed.h"
#include "bn_optional.h"
#include "bn_sprite_ptr.h"
#include "bn_vector.h"

#include "mj/mj_game.h"
#include "mj/mj_difficulty_level.h"

#include "dh_defines.h"
#include "dh_camera.h"
#include "dh_candy.h"
#include "dh_controls.h"
#include "dh_hand.h"
#include "dh_impl1.h"
#include "dh_impl2.h"

DH_START_NAMESPACE

enum State {
	Intro,
	Playing
};

/**
 * The implementation for both parts of "Door Hell".
 * 
 * I'm implementing both into the same `mj::game` class so I can ensure
 * "part 2" only occurs after winning "part 1".
 */
class game : public mj::game {
public:
	game(int _completed_games, const mj::game_data& data);

	[[nodiscard]]
	bool check_if_part_2(int _completed_games);

	[[nodiscard]]
	bn::string<16> title() const final {
		if(is_part_2) {
			return "Door Hell (Pt 2)";
		}
		return "Door Hell";
	}

	[[nodiscard]]
	int total_frames() const final {
		return total_frames_value;
	}

	[[nodiscard]]
	bool victory() const final {
		return is_victory;
	}

	void fade_in(const mj::game_data& data) final;
	void fade_out(const mj::game_data& data) final;

	[[nodiscard]]
	mj::game_result play(const mj::game_data& data) final;

	void on_pause_start(const mj::game_data& data) final;
	void on_pause_end(const mj::game_data& data) final;

private:
	// -------------------------------------------
	// STATICS

	/**
	 * Tracks progress to decide which "part" the player is on.
	 * Only play "part 2" after successfully winning the first part.
	 */
	static int progress;

	/**
	 * Tracks the number of completed games since the last game.
	 * If this is greater, that means this is a different run.
	 */
	static int stored_completed_games;

	// -------------------------------------------
	// FUNCTIONS
	int play_bgm(const mj::game_data& data);
	void generate_tutorial_text(const char* msg, const mj::game_data& data);
	void setup_palette(int completed_games);

	void on_first_update(const mj::game_data& data);

	void update_exit(mj::game_result& result);
	void update_exit_preresult();

	int generate_unique_random_position(const mj::game_data& data) const;

	void set_current_references(mj::game_result& result, const mj::game_data& data);
	void reset_current_references();

	void set_victory();
	void set_defeat();

	void start_playing();

	void start_playing_part_2();
	void generate_candy();

	void update();

	void update_intro();
	void update_text();
	void update_game();
	void update_movement();

	void update_game_part_2();

	// -------------------------------------------
	// TEXT SPRITES
	bn::vector<bn::sprite_ptr, 5> text_sprites;
	bn::vector<bn::fixed, 5> text_offsets;
	bn::fixed text_ratio;

	// -------------------------------------------
	// STATE
	State state = Intro;

	// -------------------------------------------
	// MICRO GAME
	mj::game_result* current_result = nullptr;
	const mj::game_data* current_data = nullptr;

	// -------------------------------------------
	// CAMERA
	camera cam;

	// -------------------------------------------
	// CONTROLS
	bn::optional<controls> controls_sprite;

	/**
	 * The implementation and behavior for the "part 2" version.
	 */
	impl1 part_1;

	/**
	 * The implementation and behavior for the "part 2" version.
	 */
	impl2 part_2;

	// -------------------------------------------
	// MICRO GAME
	mj::difficulty_level level;
	int completed_games;

	int total_frames_value;
	int show_result_frames = -1;

	bool is_victory = false;
	bool is_defeat = false;

	bool is_initialized = false;
	bool is_part_2 = false;
};

DH_END_NAMESPACE
