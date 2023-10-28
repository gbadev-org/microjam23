#ifndef AWJ_GAME_H
#define AWJ_GAME_H

#include "mj/mj_game.h"

#include "bn_optional.h"
#include "bn_sprite_ptr.h"
#include "bn_regular_bg_ptr.h"

namespace awj
{

constexpr int MaxBagCount = 6;

class awj_game : public mj::game
{
public:
	awj_game(int completed_games, const mj::game_data& data);

	[[nodiscard]] bn::string<16> title() const final
	{
		return "FIND THE CANDY!";
	}

	int total_frames() const final;

	mj::game_result play(const mj::game_data& data) final;

	bool victory() const final;

	inline void fade_in([[maybe_unused]] const mj::game_data& data) final { }
	inline void fade_out([[maybe_unused]] const mj::game_data& data) final { }

	void enter_state_candy_placement(const mj::game_data& data, mj::game_result& result);
	void play_state_candy_placement(const mj::game_data& data, mj::game_result& result);
	void leave_state_candy_placement(const mj::game_data& data, mj::game_result& result);

	void enter_state_shuffling(const mj::game_data& data, mj::game_result& result);
	void play_state_shuffling(const mj::game_data& data, mj::game_result& result);
	void leave_state_shuffling(const mj::game_data& data, mj::game_result& result);

	void enter_state_user_selection(const mj::game_data& data, mj::game_result& result);
	void play_state_user_selection(const mj::game_data& data, mj::game_result& result);
	void leave_state_user_selection(const mj::game_data& data, mj::game_result& result);

	void enter_state_finish(const mj::game_data& data, mj::game_result& result);
	void play_state_finish(const mj::game_data& data, mj::game_result& result);
	void leave_state_finish(const mj::game_data& data, mj::game_result& result);

private:
	int current_state;
	bool go_to_next_state;

	int state_time;
	int time_limits[4];
	int total_time;

	using state_func_ptr = void (awj_game::*)(const mj::game_data& data, mj::game_result& result);

	state_func_ptr enter_state_funcs[4];
	state_func_ptr play_state_funcs[4];
	state_func_ptr leave_state_funcs[4];

	int bag_count;
	int frames_per_shuffle;
	int shuffle_count;
	int user_selection_duration;

	int winning_bag;
	int bags[MaxBagCount];
	int next_bags[MaxBagCount];

	int selection;
	bool has_made_selection;

	bn::fixed x_positions[MaxBagCount];

	bn::fixed moon_alpha;

	bn::optional<bn::sprite_ptr> bag_sprites[MaxBagCount];
	bn::optional<bn::sprite_ptr> shadow_sprites[MaxBagCount];
	bn::optional<bn::sprite_ptr> candy_sprite;
	bn::optional<bn::sprite_ptr> arrow_sprite;
	bn::optional<bn::sprite_ptr> light_sprite;
	bn::optional<bn::sprite_ptr> moon_sprite;

	bn::optional<bn::regular_bg_ptr> bg;
};

}

#endif
