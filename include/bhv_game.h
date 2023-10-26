#ifndef BHV_GAME_H
#define BHV_GAME_H

#include "bn_optional.h"
#include "bn_regular_bg_ptr.h"
#include "bn_sprite_ptr.h"
#include "bn_vector.h"

#include "mj/mj_game.h"
#include "bhv_conductor.h"
#include "bhv_puppy.h"
#include "bhv_cat.h"

namespace bhv
{
    #define BHV_NOTE_COUNT_MAX 6

	class bhv_game : public mj::game
	{
		enum button_mapping
		{
			A = 0,
			B = 1,
			L = 2,
			R = 3,
			UP = 4,
			DOWN = 5,
			LEFT = 6,
			RIGHT = 7
		};

		enum bhv_game_phase
		{
			BHV_PHASE_TEACHING,
			BHV_PHASE_RECITING,
			BHV_PHASE_RESULTS
		};

	public:
		bhv_game(int completed_games, const mj::game_data &data);
		[[nodiscard]] mj::game_result play(const mj::game_data &data) final;

		[[nodiscard]] bn::string<16> title() const final
		{
			return "SING!";
		}

		[[nodiscard]] int total_frames() const final
		{
			return _total_frames;
		}

		[[nodiscard]] bool victory() const final
		{
			return _beats_to_victory == 0;
		}

        void fade_in(const mj::game_data &) final {}
        void fade_out(const mj::game_data &) final {}

	private:
		bn::regular_bg_ptr _bg;
        bn::vector<bn::sprite_ptr, BHV_NOTE_COUNT_MAX> _btn_sprites;
		bn::vector<bn::sprite_ptr, 7> _prompt_sprites;
		bn::optional<bn::sprite_ptr> _cat_sing_box;
		bn::optional<bn::sprite_ptr> _cat_sing_btn;
		bn::optional<bn::sprite_ptr> _pup_sing_box;
		bn::optional<bn::sprite_ptr> _pup_sing_btn;

		bhv_puppy _player_pup;
        bn::vector<bhv_cat, BHV_NOTE_COUNT_MAX> _singing_cats;
		bhv_conductor _conductor;

		int _total_frames;
		int _show_result_frames = 60;
		int _beats_to_victory = -1;
		int _beats_to_defeat = -1;
		int _show_prompt_frames = 30;

		bhv_game_phase _game_phase;
		int _note_count;

        bn::vector<int, BHV_NOTE_COUNT_MAX> _pattern_items;
		int _prompt_index;
		int _recite_index;
		int _player_index;
		int _frames_per_beat;
		int _player_on_tempo;
		bool _player_was_correct;

		void init_sprites(const mj::game_data &data);
		void clear();
		void start_win();
		void start_lose();
		void win();
		void lose();

		bool any_pressed_not_start_select();
		int get_pressed_button();
		void reveal_button();
		void reveal_all_buttons();
		void mark_button(int index);
		void hide_prompt();
		void player_recite_button(int btn);
		void cpu_recite_button();
		void advance_recite_index();
		void set_phase(bhv_game_phase phase);
		void update_animations();
	};

}

#endif
