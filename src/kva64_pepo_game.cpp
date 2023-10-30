#include "kva64_pepo_game.h"

#include "bn_blending.h"
#include "bn_keypad.h"

#include "mj/mj_game_list.h"

#include "bn_regular_bg_items_kva64_pepo_big.h"
#include "bn_regular_bg_items_kva64_pepo_gourd.h"
#include "bn_regular_bg_items_kva64_pepo_small.h"

#include "bn_sound_items.h"

#include "bn_sprite_items_kva64_dpad.h"
#include "bn_sprite_items_kva64_pepo_assets.h"
#include "bn_sprite_items_kva64_pepo_knife.h"

namespace
{
    constexpr bn::string_view code_credits[] = { "kva64" };
    constexpr bn::string_view graphics_credits[] = { "kva64" };
    constexpr bn::string_view sfx_credits[] = { "kva64" };

    void play_random_cutting_sound(bn::random& randomizer)
    {
        switch(randomizer.get_int(0,3))
        {
        case 0:
            bn::sound_items::kva64_pepo_cut_1.play();
            break;
        case 1:
            bn::sound_items::kva64_pepo_cut_2.play();
            break;
        case 2:
            bn::sound_items::kva64_pepo_cut_3.play();
            break;
        default:
            bn::sound_items::kva64_pepo_cut_4.play();
            break;
        }
    }
}

// MJ_GAME_LIST_ADD(kva64::pepo_surgeon_game)
MJ_GAME_LIST_ADD_CODE_CREDITS(code_credits)
MJ_GAME_LIST_ADD_GRAPHICS_CREDITS(graphics_credits)
MJ_GAME_LIST_ADD_SFX_CREDITS(sfx_credits)

namespace kva64
{
    pepo_surgeon_game::pepo_surgeon_game(int completed_games, const mj::game_data& data) :
        cutting_direction(completed_games & 1),
        _total_frames(play_jingle(mj::game_jingle_type::TOTSNUK07, completed_games, data))
    {
        pepo_hat_ys.insert(pepoType::Gourd, -47);
        pepo_hat_ys.insert(pepoType::Small, -28);
        pepo_hat_ys.insert(pepoType::Big, -49);

        pepo_knife_pos.insert(pepoType::Gourd, bn::point(36, -12));
        pepo_knife_pos.insert(pepoType::Small, bn::point(48, 10));
        pepo_knife_pos.insert(pepoType::Big, bn::point(72, -16));

        pepo_cut_count.insert(pepoType::Gourd, 6);
        pepo_cut_count.insert(pepoType::Small, 9);
        pepo_cut_count.insert(pepoType::Big, 13);

        switch(completed_games % 3)
        {
        case 0:
            _bg = bn::regular_bg_items::kva64_pepo_gourd.create_bg(8, (256 - 160) / 2);
            current_pepo = pepoType::Gourd;
            break;
        case 1:
            _bg = bn::regular_bg_items::kva64_pepo_small.create_bg(8, (256 - 160) / 2);
            current_pepo = pepoType::Small;
            break;
        default:
            _bg = bn::regular_bg_items::kva64_pepo_big.create_bg(8, (256 - 160) / 2);
            current_pepo = pepoType::Big;
            break;
        }

        current_cut_count = pepo_cut_count[current_pepo];

        pepo_face[0] = bn::sprite_items::kva64_pepo_assets.create_sprite(-32, 28, static_cast<int>(current_pepo));
        pepo_face[1] = bn::sprite_items::kva64_pepo_assets.create_sprite(32, 28, static_cast<int>(current_pepo)+6);
        pepo_face[0]->set_blending_enabled(true);
        pepo_face[1]->set_blending_enabled(true);

        knife = bn::sprite_items::kva64_pepo_knife.create_sprite(pepo_knife_pos[current_pepo].x(), pepo_knife_pos[current_pepo].y());

        pepo_hat[0] = bn::sprite_items::kva64_pepo_assets.create_sprite(-32, pepo_hat_ys[current_pepo], static_cast<int>(current_pepo)+3);
        pepo_hat[1] = bn::sprite_items::kva64_pepo_assets.create_sprite(32, pepo_hat_ys[current_pepo], static_cast<int>(current_pepo)+9);

        dpad_directions = bn::sprite_items::kva64_dpad.create_sprite(90, 0, cutting_direction);
    }

    void pepo_surgeon_game::fade_in([[maybe_unused]] const mj::game_data& data)
    {

    }

    mj::game_result pepo_surgeon_game::play(const mj::game_data &data)
    {
        mj::game_result result;
        result.exit = data.pending_frames == 0;

        if(! _victory && ! _defeat)
        {
            if(!current_cut_count)
            {
                dpad_directions->set_visible(false);

                result.remove_title = true;
                _victory = true;
            }

            if ((bn::keypad::up_pressed() && cutting_direction) || (bn::keypad::down_pressed() && !cutting_direction))
            {
                play_random_cutting_sound(data.random);

                cutting_direction ^= 1;
                dpad_directions->set_tiles(bn::sprite_items::kva64_dpad.tiles_item(), cutting_direction);
                --current_cut_count;

                knife->set_x(knife->x()-10);
                knife->set_y(cutting_direction ? knife->y()-5 : knife->y()+5);
                knife->set_rotation_angle(cutting_direction ? 15 : 345);
            }
        }
        else
        {
            if(_show_result_frames)
            {
                --_show_result_frames;

                if (_show_result_anim_frames)
                {
                    --_show_result_anim_frames;

                    for(auto pepo : pepo_hat)
                    {
                        pepo->set_y(pepo->y()-0.5);
                    }

                    for(auto pepo : pepo_face)
                    {
                        bn::fixed h_scale{pepo->horizontal_scale()};
                        bn::fixed v_scale{pepo->vertical_scale()};
                        bn::fixed t_alpha{bn::blending::transparency_alpha()};

                        pepo->set_horizontal_scale(h_scale+0.1);
                        pepo->set_vertical_scale(v_scale+0.1);

                        pepo->set_y(pepo->y()*pepo->y()*-1);
                        bn::blending::set_transparency_alpha(bn::max(t_alpha - 0.02, bn::fixed(0)));
                    }

                    pepo_face[0]->set_x(pepo_face[0]->x()-5);
                    pepo_face[1]->set_x(pepo_face[1]->x()+5);
                }
            }
            else
            {
                result.exit = true;
            }
        }
        return result;
    }

    void pepo_surgeon_game::fade_out([[maybe_unused]] const mj::game_data &data)
    {
        bn::blending::set_transparency_alpha(1);
    }
}
