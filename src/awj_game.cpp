#include "awj_game.h"
#include "awj_utils.h"
#include "awj_luts.h"

#include "bn_math.h"

#include "bn_regular_bg_ptr.h"

#include "bn_keypad.h"

#include "bn_sprite_items_awj_arrow.h"
#include "bn_sprite_items_awj_bag.h"
#include "bn_sprite_items_awj_candy.h"
#include "bn_sprite_items_awj_light.h"
#include "bn_sprite_items_awj_shadow.h"

#include "bn_regular_bg_items_awj_bg.h"

#include "bn_sound_items.h"

#include "mj/mj_game_list.h"

namespace
{
    constexpr bn::string_view code_credits[] = { "AWJ" };
    constexpr bn::string_view graphics_credits[] = { "AWJ" };
    constexpr bn::string_view sfx_credits[] = { "AWJ" };
}

// MJ_GAME_LIST_ADD(awj::awj_game)
MJ_GAME_LIST_ADD_CODE_CREDITS(code_credits)
MJ_GAME_LIST_ADD_GRAPHICS_CREDITS(graphics_credits)
MJ_GAME_LIST_ADD_SFX_CREDITS(sfx_credits)

namespace awj
{

constexpr int CandyPlacementDuration = 50;
constexpr int FinishDuration = 45;

// full game run time is:
// 	candy placement duration +
//  frames per shuffle * shuffle count +
//  user selection duration +
//  finish duration
// (adjust if neccesary)

// z order:
// 5 shadows
// 4 selection indicator / light
// 3 the candy after
// 2 bags
// 1 the candy before

constexpr bn::fixed shadow_y = 32;
constexpr bn::fixed light_y = 30;
constexpr bn::fixed bag_y = 0;
constexpr bn::fixed selected_bag_y = bag_y - 8;
constexpr bn::fixed arrow_y = 48;
constexpr bn::fixed candy_y = -48;

constexpr int bag_count_from_difficulty[]{ 3, 4, 5 };
constexpr int shuffle_count_from_difficulty[]{ 5, 6, 7 };
constexpr int frames_per_shuffle_from_difficulty[]{ 30, 27, 24 };
constexpr int user_selection_duration_from_difficulty[]{ 60, 58, 56 };

awj_game::awj_game([[maybe_unused]] const int completed_games, [[maybe_unused]] const mj::game_data& data)
	: enter_state_funcs{
		&awj_game::enter_state_candy_placement,
		&awj_game::enter_state_shuffling,
		&awj_game::enter_state_user_selection,
		&awj_game::enter_state_finish
	}
	, play_state_funcs{
		&awj_game::play_state_candy_placement,
		&awj_game::play_state_shuffling,
		&awj_game::play_state_user_selection,
		&awj_game::play_state_finish
	}
	, leave_state_funcs{
		&awj_game::leave_state_candy_placement,
		&awj_game::leave_state_shuffling,
		&awj_game::leave_state_user_selection,
		&awj_game::leave_state_finish
	}
	, next_bags{ 0, 1, 2, 3, 4 }
{
	current_state = 0;
	state_time = 0;

	const uint8_t difficulty = static_cast<uint8_t>(recommended_difficulty_level(completed_games, data));

	bag_count = bag_count_from_difficulty[difficulty];
	frames_per_shuffle = frames_per_shuffle_from_difficulty[difficulty];
	shuffle_count = shuffle_count_from_difficulty[difficulty];
	user_selection_duration = user_selection_duration_from_difficulty[difficulty];

	time_limits[0] = CandyPlacementDuration;
	time_limits[1] = frames_per_shuffle * shuffle_count;
	time_limits[2] = user_selection_duration;
	time_limits[3] = FinishDuration;
	total_time = time_limits[0] + time_limits[1] + time_limits[2] + time_limits[3]; // who needs accumulate

	winning_bag = data.random.get_int(bag_count);
	selection = bag_count / 2;
	has_made_selection = false;

	for (int i = 0; i < bag_count; i++)
	{
		const auto x = bn::fixed{ 180 * i } / (bag_count - 1) - 90;
		x_positions[i] = x;

		shadow_sprites[i] = bn::sprite_items::awj_shadow.create_sprite(x, shadow_y);
		shadow_sprites[i]->set_z_order(4);

		bag_sprites[i] = bn::sprite_items::awj_bag.create_sprite(x, bag_y);
		bag_sprites[i]->set_z_order(1);
	}

	candy_sprite = bn::sprite_items::awj_candy.create_sprite(0, candy_y);
	candy_sprite->set_scale(2);

	arrow_sprite = bn::sprite_items::awj_arrow.create_sprite(0, arrow_y);
	arrow_sprite->set_visible(false);
	arrow_sprite->set_x(x_positions[selection]);

	light_sprite = bn::sprite_items::awj_light.create_sprite(0, light_y);
	light_sprite->set_visible(false);
	light_sprite->set_x(x_positions[selection]);

	bg = bn::regular_bg_items::awj_bg.create_bg(0, 0);

	(void)play_jingle(mj::game_jingle_type::TOTSNUK06, completed_games, data);
}

int awj_game::total_frames() const
{
	return total_time;
}

// Candy placement

void awj_game::enter_state_candy_placement([[maybe_unused]] const mj::game_data& data, [[maybe_unused]] mj::game_result& result)
{
	// BN_LOG(__FUNCTION__);
}

void awj_game::play_state_candy_placement([[maybe_unused]] const mj::game_data& data, [[maybe_unused]] mj::game_result& result)
{
	const auto alpha = bn::fixed{ state_time + 1 } / CandyPlacementDuration;

	const auto a2 = (3 * alpha + 1) * (1 - alpha);
	candy_sprite->set_y(candy_y * a2);
	candy_sprite->set_x(util::ease_in_out(alpha) * x_positions[winning_bag]);
	candy_sprite->set_scale((5 - 3 * alpha) / 2);
	candy_sprite->set_rotation_angle(alpha * 350);

	if (state_time == CandyPlacementDuration / 2)
		candy_sprite->set_z_order(3);
}

void awj_game::leave_state_candy_placement([[maybe_unused]] const mj::game_data& data, [[maybe_unused]] mj::game_result& result)
{
	// BN_LOG(__FUNCTION__);

	candy_sprite->set_visible(false);
}

// Shuffling

void awj_game::enter_state_shuffling([[maybe_unused]] const mj::game_data& data, [[maybe_unused]] mj::game_result& result)
{
	// BN_LOG(__FUNCTION__);
}

void awj_game::play_state_shuffling(const mj::game_data& data, [[maybe_unused]] mj::game_result& result)
{
	const int r = state_time % frames_per_shuffle;
	if (r == 0)
	{
		util::copy(next_bags, bags);

		const int factorial = luts::factorial[bag_count];
		const int shuffle_id = data.random.get_int(1, factorial);
		util::shuffle(next_bags, bag_count, shuffle_id);

		bn::sound_items::awj_shuffle.play();
	}
	
	const bn::fixed alpha = bn::fixed{ r + 1 } / frames_per_shuffle;

	const bn::fixed y_multiplier = util::parabola(alpha);
	for (int i = 0; i < bag_count; i++)
	{
		const int d = bn::abs(bags[i] - next_bags[i]);
		if (d != 0)
		{
			const bn::fixed new_x = util::lerp(
				alpha,
				x_positions[bags[i]],
				x_positions[next_bags[i]]
			);

			bag_sprites[i]->set_position(
				new_x,
				d * y_multiplier * -16
			);

			shadow_sprites[i]->set_x(new_x);
			shadow_sprites[i]->set_scale(1 - y_multiplier / 2);
		}
	}
}

void awj_game::leave_state_shuffling([[maybe_unused]] const mj::game_data& data, [[maybe_unused]] mj::game_result& result)
{
	// BN_LOG(__FUNCTION__);
	
	util::copy(next_bags, bags);
}

// User selection

void awj_game::enter_state_user_selection([[maybe_unused]] const mj::game_data& data, [[maybe_unused]] mj::game_result& result)
{
	// BN_LOG(__FUNCTION__);

	arrow_sprite->set_visible(true);
	light_sprite->set_visible(true);

	// we've shuffled the sprites around to random positions
	// put the sprites back in their default position
	for (int i = 0; i < bag_count; i++)
	{
		bag_sprites[i]->set_x(x_positions[i]);
		bag_sprites[i]->set_y(bag_y);
	}

	bag_sprites[selection]->set_y(selected_bag_y);
}

void awj_game::play_state_user_selection([[maybe_unused]] const mj::game_data& data, [[maybe_unused]] mj::game_result& result)
{
	const bn::fixed alpha = bn::fixed{ state_time } / user_selection_duration;
	light_sprite->set_scale(1 - alpha); // should never be zero

	const int old_selection = selection;
	if (bn::keypad::left_pressed() && selection > 0)
	{
		selection--;
		arrow_sprite->set_x(x_positions[selection]);
		light_sprite->set_x(x_positions[selection]);
	}
	else if (bn::keypad::right_pressed() && selection < bag_count - 1)
	{
		selection++;
		arrow_sprite->set_x(x_positions[selection]);
		light_sprite->set_x(x_positions[selection]);
	}

	if (old_selection != selection)
	{
		bag_sprites[old_selection]->set_y(bag_y);
		bag_sprites[selection]->set_y(selected_bag_y);
	}

	if (bn::keypad::a_pressed() || bn::keypad::b_pressed())
	{
		has_made_selection = true;
		go_to_next_state = true;
	}
}

void awj_game::leave_state_user_selection([[maybe_unused]] const mj::game_data& data, [[maybe_unused]] mj::game_result& result)
{
	// BN_LOG(__FUNCTION__);
	
	(selection == bags[winning_bag] ? bn::sound_items::awj_succeed : bn::sound_items::awj_fail).play();

	light_sprite->set_visible(false);
	arrow_sprite->set_visible(false);
}

// Finish

void awj_game::enter_state_finish([[maybe_unused]] const mj::game_data& data, [[maybe_unused]] mj::game_result& result)
{
	// BN_LOG(__FUNCTION__);
	
	candy_sprite->set_x(x_positions[bags[winning_bag]]);
	candy_sprite->set_visible(true);
}

void awj_game::play_state_finish([[maybe_unused]] const mj::game_data& data, [[maybe_unused]] mj::game_result& result)
{
	const bn::fixed alpha = bn::fixed{ state_time + 1 } / FinishDuration;
	candy_sprite->set_y(candy_y * util::ease_out_cubic(alpha));
	candy_sprite->set_scale(1 + alpha);
	candy_sprite->set_rotation_angle(util::ease_out_cubic(alpha) * 180);
}

void awj_game::leave_state_finish([[maybe_unused]] const mj::game_data& data, mj::game_result& result)
{
	// BN_LOG(__FUNCTION__);
	
	result.exit = true;
}

mj::game_result awj_game::play([[maybe_unused]] const mj::game_data& data)
{
	mj::game_result result{};

	if (go_to_next_state)
	{
		current_state++;
		state_time = 0;

		(this->*enter_state_funcs[current_state])(data, result);

		go_to_next_state = false;
	}
	
	(this->*play_state_funcs[current_state])(data, result);

	if (state_time++; state_time == time_limits[current_state])
	{
		go_to_next_state = true;
	}
	
	if (go_to_next_state)
	{
		(this->*leave_state_funcs[current_state])(data, result);
	}

	return result;
}

bool awj_game::victory() const
{
	return has_made_selection && (selection == bags[winning_bag]);
}

} // awj
