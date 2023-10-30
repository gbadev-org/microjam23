#include "bhv_game.h"

#include "bn_fixed_point.h"
#include "bn_keypad.h"
#include "bn_music.h"
#include "bn_random.h"
#include "bn_sound_items.h"
#include "bn_sprite_builder.h"
#include "bn_sprite_item.h"

#include "mj/mj_game_list.h"

#include "bn_regular_bg_items_bhv_bg.h"

#include "bn_sprite_items_bhv_button_icons.h"
#include "bn_sprite_items_bhv_instruction_bubble.h"
#include "bn_sprite_items_bhv_sing_box.h"

#define BHV_PROMPT_DISAPPEAR false
#define BHV_PLAYER_INPUTS_ALL false
#define BHV_BEATS_TO_GAME_END 1

void play_tone(int btn);
bn::fixed_point get_pumpkin_pos(int pup_idx, int pup_count);

namespace
{
	constexpr bn::string_view code_credits[] = {"squishyfrogs"};
    constexpr bn::string_view graphics_credits[] = {"Moawling", "Kenney", "squishyfrogs"};
    constexpr bn::string_view sfx_credits[] = {"squishyfrogs"};

	constexpr bn::fixed_point cat_pos_3[] = {
        {-80, 30},
		{-40, 20},
		{70, 35}
	};
	constexpr bn::fixed_point cat_pos_4[] = {
		{-80, 30},
		{-50, 20},
		{-20, 35},
		{70, 35}
	};
	constexpr bn::fixed_point cat_pos_5[] = {
		{-80, 30},
		{-50, 20},
		{-25, 45},
		{5, 35},
		{70, 35}
	};
	constexpr bn::fixed_point conductor_sprite_pos = {30, -24};
	constexpr bn::fixed_point prompt_base_pos = {-2,-48};
	constexpr bn::fixed_point sing_offset = {8,-32};
	constexpr bn::fixed_point sing_offset_pup = {-8,-32};

	constexpr bn::sound_item btn_tones[] = {
		bn::sound_items::bhv_c2,
		bn::sound_items::bhv_d2,
		bn::sound_items::bhv_e2,
		bn::sound_items::bhv_f2,
		bn::sound_items::bhv_g2,
		bn::sound_items::bhv_a2,
		bn::sound_items::bhv_b2,
		bn::sound_items::bhv_c3
	};
	
}

// MJ_GAME_LIST_ADD(bhv::bhv_game)
MJ_GAME_LIST_ADD_CODE_CREDITS(code_credits)
MJ_GAME_LIST_ADD_GRAPHICS_CREDITS(graphics_credits)
// MJ_GAME_LIST_ADD_MUSIC_CREDITS(music_credits)
MJ_GAME_LIST_ADD_SFX_CREDITS(sfx_credits)

namespace bhv
{

	bhv_game::bhv_game(int completed_games, const mj::game_data &data) : 
		_bg(bn::regular_bg_items::bhv_bg.create_bg((256 - 240) / 2, (256 - 160) / 2)),
		// TODO: Select final bgm CONTENDERS: TOTSNUK10, TOTSNUK01,
		_total_frames(play_jingle(mj::game_jingle_type::TOTSNUK01, completed_games, data))
	{
		constexpr int frames_diff = maximum_frames - minimum_frames;
		constexpr int maximum_speed_completed_games = 30;

		clear();

		completed_games = bn::min(completed_games, maximum_speed_completed_games);

		int frames_reduction = (frames_diff * completed_games) / maximum_speed_completed_games;
		_total_frames = maximum_frames - frames_reduction;
		_total_frames -= data.random.get_int(60);
		_total_frames = bn::clamp(_total_frames, minimum_frames, maximum_frames);

		_frames_per_beat = _total_frames / 18; //TODO: fix this to match bpm

		// TODO: prefer longer patterns the more minigames have been cleared?
		// 3-5 notes seems right?
		_note_count = 3 + data.random.get_int(3);
		_prompt_index = 0;
		_recite_index = 0;
		// _player_index = 2 + data.random.get_int(3);
		// if(_player_index >= _note_count)
		// {
		// 	_player_index = _note_count - 1;
		// }
		_player_index = _note_count-1;

		int tempo = _frames_per_beat / 6;
		_player_pup.set_position(get_pumpkin_pos(_player_index, _note_count));
		_player_pup.set_wait_updates(4);
		_conductor.set_position(conductor_sprite_pos);
		_conductor.set_wait_updates(tempo);
		for (int i = 0; i < _note_count - 1; i++)
		{
			bn::fixed_point pos = get_pumpkin_pos(i, _note_count);
			_singing_cats.push_back(bhv_cat(pos));
			_singing_cats.back().set_wait_updates(tempo);
			_singing_cats.back().set_flip(true);
		}

		init_sprites(data);
		_game_phase = BHV_PHASE_TEACHING;
		_beats_to_victory = -1;
		_beats_to_defeat = -1;
		_player_on_tempo = 0;
		_player_was_correct = false;
	}

	void bhv_game::init_sprites(const mj::game_data &data)
	{
		
		// BUILD BUTTON SPRITES
		{
			// init sprite builder
			bn::sprite_builder builder(bn::sprite_items::bhv_button_icons);
			builder.set_bg_priority(1);
			builder.set_z_order(10);

			for (int i = 0; i < _note_count; i++)
			{
				// identify button for pattern
				int button = data.random.get_int(8);
				_pattern_items.push_back(button);


				// sprite setup
				int x_off = ((i - _note_count) * 17) + 4;
				int y_off = (-2 * button) + 6;
				bn::fixed_point pos = {prompt_base_pos.x() + x_off, prompt_base_pos.y() + y_off};
				builder.set_position(pos);
				_btn_sprites.push_back(builder.build());
				int btn_spr = button * 2;
				bn::sprite_tiles_item sheet_tiles = bn::sprite_items::bhv_button_icons.tiles_item();
				_btn_sprites.back().set_tiles(sheet_tiles.create_tiles(btn_spr));
				_btn_sprites.back().set_visible(false);
			}
		}

		// BUILD PROMPT SPEECH BUBBLE
		{
			bn::sprite_builder builder(bn::sprite_items::bhv_instruction_bubble);
			bn::sprite_tiles_item speech_bubble_tiles = bn::sprite_items::bhv_instruction_bubble.tiles_item();
			builder.set_bg_priority(1);
			builder.set_z_order(20);

			for (int i = 0; i < _note_count + 2; i++)
			{
				bn::fixed_point pos = {prompt_base_pos.x() + (-16 * i), prompt_base_pos.y()};
				builder.set_position(pos);
				_prompt_sprites.push_back(builder.build());
				if (i == 0)
				{
					_prompt_sprites.back().set_tiles(speech_bubble_tiles.create_tiles(0));
				}
				else if (i == _note_count + 1)
				{
					_prompt_sprites.back().set_tiles(speech_bubble_tiles.create_tiles(2));
				}
				else
				{
					_prompt_sprites.back().set_tiles(speech_bubble_tiles.create_tiles(1));
				}
			}
		}

		{
			bn::sprite_builder builder(bn::sprite_items::bhv_sing_box);
			builder.set_bg_priority(1);
			builder.set_z_order(20);
			_cat_sing_box = builder.build();
			_pup_sing_box = builder.release_build();

			_cat_sing_box.get()->set_visible(false);
			_pup_sing_box.get()->set_visible(false);
			_pup_sing_box.get()->set_horizontal_flip(true);
		}
		{
			bn::sprite_builder builder(bn::sprite_items::bhv_button_icons);
			builder.set_bg_priority(1);
			builder.set_z_order(10);
			_cat_sing_btn = builder.build();
			_pup_sing_btn = builder.release_build();

			_cat_sing_btn.get()->set_visible(false);
			_pup_sing_btn.get()->set_visible(false);
		}
	}

	void bhv_game::clear()
	{
		_btn_sprites.clear();
		_pattern_items.clear();
		_prompt_sprites.clear();
		_singing_cats.clear();
	}

	void bhv_game::start_win()
	{
		if (_beats_to_victory < 0)
		{
            _beats_to_victory = BHV_BEATS_TO_GAME_END;
		}
	}
	
	void bhv_game::start_lose()
	{
		if (_beats_to_defeat < 0)
		{
            _beats_to_defeat = BHV_BEATS_TO_GAME_END;
		}
	}

	void bhv_game::win()
	{
		set_phase(BHV_PHASE_RESULTS);
		for (int i = 0; i < _singing_cats.size(); i++)
		{
			_singing_cats[i].set_anim_state(BHV_ANIM_WIN);
		}
		_player_pup.set_anim_state(BHV_ANIM_WIN);
	}

	void bhv_game::lose()
	{
		set_phase(BHV_PHASE_RESULTS);
		bn::music::stop();
		_conductor.set_anim_lose();
		for(int i = 0; i < _singing_cats.size(); i++)
		{
			_singing_cats[i].set_anim_state(BHV_ANIM_LOSE);
		}
	}

	mj::game_result bhv_game::play(const mj::game_data &data)
	{
		mj::game_result result;
		result.exit = data.pending_frames == 0;
		int frames_elapsed = _total_frames - data.pending_frames;

		if (_player_on_tempo > 0)
		{
			--_player_on_tempo;

			if (_player_on_tempo == 0)
			{
				set_phase(BHV_PHASE_RESULTS);
				if (_player_was_correct)
				{
					start_win();
				}
				else
				{
					start_lose();
				}
			}
		}

		update_animations();

		if ((_beats_to_victory != 0) && (_beats_to_defeat != 0))
		{
			
			// game beat
			if (frames_elapsed > 0 && (frames_elapsed % _frames_per_beat == 0))
			{

				if (_beats_to_victory > 0)
				{
					--_beats_to_victory;
					if (_beats_to_victory == 0)
					{
						win();
					}
				}
				if (_beats_to_defeat > 0)
				{
					--_beats_to_defeat;
					if (_beats_to_defeat == 0)
					{
						lose();
					}
				}

				if (_game_phase == BHV_PHASE_TEACHING)
				{
					if (_prompt_index == _btn_sprites.size())
					{
						set_phase(BHV_PHASE_RECITING);
					}
					else
					{
						reveal_button();
					}
				}
                else if (_game_phase == BHV_PHASE_RECITING)
				{
                    if (!BHV_PLAYER_INPUTS_ALL)
					{
						cpu_recite_button();
					}
					advance_recite_index();
				}
			} // game beat

            if (!BHV_PLAYER_INPUTS_ALL && (_game_phase == BHV_PHASE_RECITING))
			{
				if (any_pressed_not_start_select())
				{
					// check vs pattern
					int btn = get_pressed_button();
					if (btn >= 0)
					{
						player_recite_button(btn);
					}
				}
			}
            else if(BHV_PLAYER_INPUTS_ALL)
			{
				// keep prompt around for a bit
				if (_game_phase == BHV_PHASE_RECITING && _show_prompt_frames > 0)
				{
					_show_prompt_frames--;
					if (_show_prompt_frames == 0)
					{
						hide_prompt();
					}
				}

				if (any_pressed_not_start_select())
				{
					// check vs pattern
					int btn = get_pressed_button();
					if (btn >= 0 && (_game_phase == BHV_PHASE_RECITING))
					{
						player_recite_button(btn);
					}
				}
			}
			
			
			// update conductor sprite pos
			{
				// bn::fixed y_float = (3.5 * bn::sin(0.005*frames_elapsed));
				// bn::fixed_point pos = conductor_sprite_pos;
				// pos.set_y(pos.y() + y_float);
				// _conductor.set_position(pos);
			}
		}
		else
		{
			if (_show_result_frames)
			{
				--_show_result_frames;
			}
			else
			{
				result.exit = true;
				clear();
			}
		}

		return result;
	}

	bool bhv_game::any_pressed_not_start_select()
	{
		return bn::keypad::any_pressed() && !bn::keypad::start_pressed() && !bn::keypad::select_pressed();
	}

	int bhv_game::get_pressed_button()
	{
		if (bn::keypad::a_pressed())
		{
			return button_mapping::A;
		}
		if (bn::keypad::b_pressed())
		{
			return button_mapping::B;
		}
		if (bn::keypad::l_pressed())
		{
			return button_mapping::L;
		}
		if (bn::keypad::r_pressed())
		{
			return button_mapping::R;
		}
		if (bn::keypad::left_pressed())
		{
			return button_mapping::LEFT;
		}
		if (bn::keypad::right_pressed())
		{
			return button_mapping::RIGHT;
		}
		if (bn::keypad::up_pressed())
		{
			return button_mapping::UP;
		}
		if (bn::keypad::down_pressed())
		{
			return button_mapping::DOWN;
		}
		return -1;
	}

	void bhv_game::reveal_button()
	{
		if(_prompt_index >= _btn_sprites.size())
		{
			return;
		}

		_btn_sprites[_prompt_index].set_visible(true);
		play_tone(_pattern_items[_prompt_index]);

		_prompt_index++;
		
	}

	void bhv_game::reveal_all_buttons()
	{
		for (int i = 0; i < _btn_sprites.size(); i++)
		{
			_btn_sprites[i].set_visible(true);
		}
	}

	void bhv_game::mark_button(int index)
	{
		bn::sprite_tiles_item sheet_tiles = bn::sprite_items::bhv_button_icons.tiles_item();
		int btn_spr = (_pattern_items[index] * 2) + 1;
		_btn_sprites[index].set_tiles(sheet_tiles.create_tiles(btn_spr));
	}

	void bhv_game::hide_prompt()
	{
        if (!BHV_PROMPT_DISAPPEAR)
		{
			return;
		}

		for (int i = 0; i < _btn_sprites.size(); i++)
		{
			_btn_sprites[i].set_visible(false);
		}

		for (int i = 0; i < _prompt_sprites.size(); i++)
		{
			_prompt_sprites[i].set_visible(false);
		}
	}

	void bhv_game::player_recite_button(int btn)
	{
		mark_button(_player_index);
		_player_pup.set_anim_state(BHV_ANIM_SING);
		play_tone(btn);
		bn::fixed_point pos = _player_pup.get_position();

		bn::sprite_tiles_item sheet_tiles = bn::sprite_items::bhv_button_icons.tiles_item();
		_pup_sing_btn.get()->set_tiles(sheet_tiles.create_tiles(btn * 2));
		_pup_sing_box.get()->set_position(pos + sing_offset_pup);
		_pup_sing_btn.get()->set_position(pos + sing_offset_pup);

		_pup_sing_box.get()->set_visible(true);
		_pup_sing_btn.get()->set_visible(true);

		if(!_player_on_tempo)
		{
			set_phase(BHV_PHASE_RESULTS);
			start_lose();
			return;
		}

		if(btn != _pattern_items[_player_index])
		{
			set_phase(BHV_PHASE_RESULTS);
			start_lose();
			return;
		}

		_player_was_correct = true;
	}

	void bhv_game::cpu_recite_button()
	{
		_cat_sing_box.get()->set_visible(false);
		_cat_sing_btn.get()->set_visible(false);

		if(_recite_index >= _player_index)
		{
			return;
		}

		mark_button(_recite_index);
		_singing_cats[_recite_index].set_anim_state(BHV_ANIM_SING);
		play_tone(_pattern_items[_recite_index]);
		bn::fixed_point pos = get_pumpkin_pos(_recite_index, _note_count);
		bn::sprite_tiles_item sheet_tiles = bn::sprite_items::bhv_button_icons.tiles_item();
		_cat_sing_btn.get()->set_tiles(sheet_tiles.create_tiles(_pattern_items[_recite_index] * 2));
		_cat_sing_box.get()->set_position(pos + sing_offset);
		_cat_sing_btn.get()->set_position(pos + sing_offset);

		_cat_sing_box.get()->set_visible(true);
		_cat_sing_btn.get()->set_visible(true);
	}

	void bhv_game::advance_recite_index()
	{
		
		// advance index
		_recite_index++;
		if(_recite_index == _player_index)
		{
			_player_on_tempo = _frames_per_beat * 2 + 1;
		}

	}

	void bhv_game::set_phase(bhv_game_phase phase)
	{
		_game_phase = phase;
		_prompt_index = 0;
		if(phase == BHV_PHASE_RESULTS)
		{
			_cat_sing_box.get()->set_visible(false);
			_cat_sing_btn.get()->set_visible(false);
		}
	}

	void bhv_game::update_animations()
	{

		// cats
		for (int i = 0; i < _singing_cats.size(); i++)
		{
			_singing_cats[i].update_anim();
		}

		_player_pup.update_anim();
		_conductor.update_anim();
	}

}

void play_tone(int btn)
{
	if (btn < 0 || btn >= 8)
	{
		return;
	}
	btn_tones[btn].play();
}

bn::fixed_point get_pumpkin_pos(int pup_idx, int pup_count)
{
	if (pup_idx >= pup_count || pup_idx < 0)
	{
		return bn::fixed_point();
	}
	switch (pup_count)
	{
	case 3:
        return cat_pos_3[pup_idx];
	case 4:
        return cat_pos_4[pup_idx];
	case 5:
        return cat_pos_5[pup_idx];
	default:
		break;
	}
	return bn::fixed_point();
}
