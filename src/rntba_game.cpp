#include "rntba_game.h"

#include "bn_sprite_ptr.h"
#include "bn_sprite_tiles_ptr.h"
#include "bn_sprite_palette_ptr.h"

#include "mj/mj_game_list.h"

#include "bn_regular_bg_items_rntba_room.h"

#include "bn_sprite_items_rntba_explode.h"
#include "bn_sprite_items_rntba_pumpkin.h"
#include "bn_sprite_items_rntba_frame.h"
#include "bn_sprite_items_rntba_moon.h"
#include "bn_sprite_palette_items_rntba_pumpkin_alt.h"

#include "bn_sound_items.h"

namespace
{
    constexpr bn::string_view code_credits[] = { "Rakiex" };
    constexpr bn::string_view graphics_credits[] = { "Rakiex", "Bacteri" };
    constexpr bn::string_view sfx_credits[] = { "Rakiex" };
}

// MJ_GAME_LIST_ADD(rntba::test_game)
MJ_GAME_LIST_ADD_CODE_CREDITS(code_credits)
MJ_GAME_LIST_ADD_GRAPHICS_CREDITS(graphics_credits)
MJ_GAME_LIST_ADD_SFX_CREDITS(sfx_credits)

namespace rntba
{

test_game::test_game(int completed_games, const mj::game_data& data) :
    _bg(bn::regular_bg_items::rntba_room.create_bg((256 - 240) / 2, (256 - 160) / 2))
    , _total_frames(play_jingle(mj::game_jingle_type::TOTSNUK01, completed_games, data))
    //, _total_frames(recommended_total_frames(600, completed_games, data))
    , _spider(completed_games, data, _total_frames)
    , _pumpkin(bn::sprite_items::rntba_pumpkin.create_sprite(47, 1, 0))
    , _frame(bn::sprite_items::rntba_frame.create_sprite(-51, -42, 0))
    , _moon(bn::sprite_items::rntba_moon.create_sprite(-69, -44, 0))
{
    _pumpkin.set_z_order(2);
    _moon.set_z_order(-1);
}

void test_game::fade_in([[maybe_unused]] const mj::game_data& data)
{
}

mj::game_result test_game::play(const mj::game_data& data)
{
    mj::game_result result;
    result.exit = data.pending_frames == 0;

    if (pumpkin_timer > 15){
        if (pumpkin_pal)
            _pumpkin.set_palette(bn::sprite_palette_items::rntba_pumpkin_alt.create_palette());
        else
            _pumpkin.set_palette(bn::sprite_items::rntba_pumpkin.palette_item());
        pumpkin_pal = !pumpkin_pal;
        pumpkin_timer = 0;
    }
    else
        pumpkin_timer++;

    int hand_data = _hand.process();
    
    int spider_data = _spider.process(hand_data, _hand.hit_top(), _hand.hit_len());

    switch (spider_data){
        case 3:{
            _victory = true;
          // Shocked pumpkin
            bn::sprite_tiles_ptr shocktiles = _pumpkin.tiles();
            shocktiles.set_tiles_ref(bn::sprite_items::rntba_pumpkin.tiles_item(), 1);
        }
        break;
        case 5:
            if (hit_frame == 0){
                hit_frame++;
              // Change Moon into explosion
                _moon.set_tiles(bn::sprite_items::rntba_explode.tiles_item(), 0);
                _moon.set_palette(bn::sprite_items::rntba_explode.palette_item());
              // Play boom sfx
                bn::sound_items::rntba_boom.play();
              // Thumbs up!
                _hand.thumbsup();
            }
            if (hit_timer > 3){
                hit_timer = 0;
                if (hit_frame > 5){
                    _moon.set_visible(false);
                }
                else{
                    _moon.set_tiles(bn::sprite_items::rntba_explode.tiles_item(), hit_frame);
                    hit_frame++;
                }
            }
            else
                hit_timer++;
            break;
        case 6:
            _defeat = true;
            break;
        default:
            break;
    }

    if(_victory || _defeat){
        if(_show_result_frames)
            --_show_result_frames;
        else
            result.exit = true;
    }

    return result;
}

void test_game::fade_out([[maybe_unused]] const mj::game_data& data)
{
}

}
