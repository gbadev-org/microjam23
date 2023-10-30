#ifndef TERM_TERMINAL_GAME_H
#define TERM_TERMINAL_GAME_H

#include "bn_color.h"
#include "bn_sprite_palette_ptr.h"
#include "bn_sprite_palette_item.h"
#include "bn_bg_palette_ptr.h"
#include "bn_bg_palette_item.h"
#include "bn_regular_bg_tiles_ptr.h"
#include "bn_regular_bg_map_ptr.h"
#include "bn_regular_bg_ptr.h"
#include "bn_sprite_ptr.h"

#include "mj/mj_game.h"


#define TERM_COLOR_COUNT 12
//extern unsigned short term_colorLUT[TERM_COLOR_COUNT][2];


#define TEXT_PAL 14 //the palbank that the terminal text uses




#define TERM_X (-120 + 4)
#define TERM_Y  (-73)
#define TERM_Y_SPACING 16
#define TERM_TEXT_PAL_MEM (0x05000200 + (TEXT_PAL << 5) + (1 << 1))
#define TERM_STATIC_W 8
#define TERM_STATIC_H 4
#define TERM_STATIC_COLCOUNT 16
#define TERM_STATIC_AND (TERM_STATIC_COLCOUNT - 1)

#define TERM_OPTIONCOUNT 3
#define TERM_PERM_OPTIONS 6
#define TERM_MONSTERCOUNT 14

#define TERM_KEYDELAY  25 //how many frames to wait before key repeat kicks in
#define TERM_KEYREPEAT 14 //how many frames to wait between key repeats

#define TERM_SELECT_VOLUME .5
#define TERM_CORRECT_VOLUME TERM_SELECT_VOLUME
#define TERM_INCORRECT_VOLUME TERM_SELECT_VOLUME

#define TERM_CLICK_VOLUME 1

namespace terminal_game
{

class game : public mj::game
{

public:
    game(int completed_games, const mj::game_data& data);

    [[nodiscard]] bn::string<16> title() const final
    {
        return "Text Adventure!";
    }

    [[nodiscard]] int total_frames() const final
    {
        return _total_frames;
    }

    void fade_in(const mj::game_data& data) final;

    [[nodiscard]] mj::game_result play(const mj::game_data& data) final;

    [[nodiscard]] bool victory() const final
    {
        return _victory;
    }

    void fade_out(const mj::game_data& data) final;
	
	void on_pause_start(const mj::game_data& data) final;
	
	void term_anim(const mj::game_data& data);
    void hdma_anim();
    void move_cursor();
    void play_option(const mj::game_data& data);
    void play_select(const mj::game_data& data);

private:
	//sprite constructor
    //bn::sprite_ptr _sprites;
	bn::vector<bn::sprite_ptr, 80> _text_sprites;
	bn::vector<bn::sprite_ptr,  1> _cursor_sprites;
	alignas(int)bn::array<bn::color, 16> _text_colors;
	bn::sprite_palette_item _text_palette_item;
	bn::sprite_palette_ptr _text_palette;
	bn::sprite_text_generator _cursor_text_generator;
	bn::sprite_text_generator _terminal_text_generator;
	
	alignas(int)bn::array<bn::color, 128> _colors;
    bn::bg_palette_item _palette_item;
    bn::bg_palette_ptr _palette;
    bn::regular_bg_tiles_ptr _tiles;
    bn::regular_bg_map_ptr _map;
    bn::regular_bg_ptr _bg;
    bn::span<bn::tile> _tiles_span;
    bn::span<bn::regular_bg_map_cell> _map_span;
	
    bn::regular_bg_ptr _black_bg;
	
	uint16_t hdma_colors[2][322];
	int current_hdma = 0;
	
    bool _victory = false;
    bool _defeat = false;
    int _show_result_frames = 60;
	
	//each permutation of options available
	char _permutations[TERM_PERM_OPTIONS][3] = {{0, 1, 2},
												{1, 0, 2},
												{2, 1, 0},
												{2, 0, 1},
												{0, 2, 1},
												{1, 2, 0}};
												
	int _hdma_tracker = 0;
	
	int _current_permutation;
	int _current_correct_answer;
	int _current_option = 0; //start at the topmost option
	
	int _key_delay = 0;
	int _key_repeat = 0;
	
	//the pointer to the palette entry we want to do an HDMA effect to
	uint16_t* palval;
	
	int _total_frames = 10 * 60;
	
	int _current_monster;
	//int _total_frames;
};

}

#endif
