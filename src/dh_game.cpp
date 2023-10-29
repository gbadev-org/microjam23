#include "dh_game.h"

#include "bn_keypad.h"
#include "bn_math.h"

#include "bn_sound_items.h"

DH_START_NAMESPACE

int game::progress = 0;
int game::stored_completed_games = 0;

game::game(int _completed_games, const mj::game_data& data):
	text_ratio(-0.5), // Set to negative number to delay appearance
	cam(check_if_part_2(completed_games)),
	level(recommended_difficulty_level(completed_games, data)),
	completed_games(_completed_games),
	is_part_2(cam.get_is_part_2()) // Weird C++ workaround. Doesn't stay if assigned from `check_if_part_2`.
{
	total_frames_value = play_bgm(data);

	if(is_part_2) {
		part_2.init(cam, data);
	} else {
		generate_tutorial_text("Ring the doorbell.", data);
		part_1.setup_palette(cam, progress);
	}
}

bool game::check_if_part_2(int _completed_games) {
	// Reset progress if this is new run...
	// TODO: request for better way to do this??
	if(stored_completed_games > _completed_games) {
		progress = 0;
	}
	stored_completed_games = _completed_games;

	// Check if this is an "odd" run.
	return progress % 2 == 1;
}

int game::play_bgm(const mj::game_data& data) {
	auto jingle = is_part_2 ? mj::game_jingle_type::TOTSNUK16 : (
		(completed_games >= 24 || progress >= 6) ? mj::game_jingle_type::TOTSNUK05 : mj::game_jingle_type::TOTSNUK06
	);
	return play_jingle(jingle, completed_games, data);
}

void game::generate_tutorial_text(const char* msg, const mj::game_data& data) {
	// Show tutorial text
	auto x = 260;
	data.small_text_generator.generate(x, 5, msg, text_sprites);

	// Record text offsets
	auto mid = data.small_text_generator.width(msg) / -2;
	DH_FOR(text_sprites.size()) {
		text_sprites[i].set_bg_priority(1); // show text above backgrounds
		text_offsets.push_back(text_sprites[i].x() - x + mid);
	}
}

void game::fade_in([[maybe_unused]] const mj::game_data& data) {
	update_intro();
	update_text();
}

void game::fade_out([[maybe_unused]] const mj::game_data& data) {
	if(is_part_2) {
		update();
	}
}

mj::game_result game::play(const mj::game_data& data) {
	if(!is_initialized) {
		is_initialized = true;
		on_first_update(data);
	}

	mj::game_result result;

	// This never hits 0, so closest we can get to handling "out of time" is 1.
	if(data.pending_frames == 1) {
		if(!is_victory) {
			progress = 0; // Ran out of time = failure. Reset progress...
		}
	}

	result.exit = data.pending_frames == 0;

	if(!is_victory && !is_defeat) {
		set_current_references(result, data);
		update();
		reset_current_references();
	} else {
		update_exit(result);
	}

	return result;
}

void game::on_first_update(const mj::game_data& data) {
	if(!is_part_2) {
		part_1.on_first_update(cam, level, data);
	}
}

void game::update_exit(mj::game_result& result) {
	if(show_result_frames == -1) {
		update_exit_preresult();
	} else if(show_result_frames) {
		show_result_frames--;
	} else {
		result.exit = true;
	}
}

void game::update_exit_preresult() {
	if(is_part_2) {
		if(part_2.update() == 3) {
			show_result_frames = 15;
		}
		update_text();
	} else {
		show_result_frames = 45;
	}
}

void game::set_current_references(mj::game_result& result, const mj::game_data& data) {
	current_result = &result;
	current_data = &data;
}

void game::reset_current_references() {
	current_result = nullptr;
	current_data = nullptr;
}

void game::set_victory() {
	if(current_result) {
		current_result->remove_title = true;
	}
	is_victory = true;
	progress++;
}

void game::set_defeat() {
	if(current_result) {
		current_result->remove_title = true;
	}
	is_defeat = true;
	progress = 0;
}

void game::start_playing() {
	state = Playing;

	if(is_part_2) {
		generate_tutorial_text("Pick your favorite.", *current_data);
		part_2.start_playing(cam, level, completed_games, *current_data);
		controls_sprite.emplace(true, current_data->small_text_generator);
	}
}

void game::update() {
	if(state == Intro) {
		update_intro();
	} else if(is_part_2) {
		switch(part_2.update()) {
			case 1: {
				set_victory();
				break;
			}
			case 2: {
				set_defeat();
				break;
			}
			default: {}
		}
	} else {
		if(part_1.update(cam, is_victory)) {
			set_victory();
		}
	}

	update_text();
}

void game::update_intro() {
	if(cam.update_intro()) {
		start_playing();
	}
}

void game::update_text() {
	if(text_offsets.size() > 0 && text_ratio < 1.0) {
		bool is_middle = text_ratio > 0.42 && text_ratio < 0.58;
		text_ratio += is_middle ? 0.003 : 0.05;

		DH_FOR(text_sprites.size()) {
			if(is_middle) {
				text_sprites[i].set_x(text_sprites[i].x() + 1);
			} else {
				text_sprites[i].set_x(text_offsets[i] + (260.0 * text_ratio) - (240.0 / 2.0));
			}
		}

		if(text_ratio >= 1.0) {
			text_sprites.clear();
		}
	}
}

void game::on_pause_start(const mj::game_data& data) {
	DH_UNUSED(data);

	if(is_part_2) {
		part_2.on_pause_start();
	} else {
		part_1.on_pause_start(cam);
		controls_sprite.emplace(false, current_data->small_text_generator);
	}
}

void game::on_pause_end(const mj::game_data& data) {
	DH_UNUSED(data);

	if(is_part_2) {
		part_2.on_pause_end();
	} else {
		part_1.on_pause_end(cam);
		controls_sprite.reset();
	}
}

DH_END_NAMESPACE
