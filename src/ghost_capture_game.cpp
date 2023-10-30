#include "ghost_capture_game.h"

#include <bn_sprite_tiles_ptr.h>
#include <bn_display.h>
#include <bn_math.h>
#include <bn_sound.h>

#include "bn_sound_items.h"

#include "bn_sprite_items_ghost_capture_baloon.h"
#include "bn_sprite_items_ghost_capture_hand.h"
#include "bn_sprite_items_ghost_capture_ghost.h"
#include "bn_sprite_items_ghost_capture_ghost_pull.h"
#include "bn_sprite_items_ghost_capture_ghost_eyes.h"
#include "bn_sprite_items_ghost_capture_cat.h"
#include "bn_regular_bg_items_ghost_capture_bg_forest.h"
#include "bn_keypad.h"

#include "mj/mj_game_list.h"

namespace
{
    constexpr bn::string_view code_credits[] = { "E-Vinyl" };
    constexpr bn::string_view graphics_credits[] = { "E-Vinyl" };
    constexpr bn::string_view sfx_credits[] = { "Volvion" };
}

// MJ_GAME_LIST_ADD(ghost_capture::ghost_capture_game)
MJ_GAME_LIST_ADD_CODE_CREDITS(code_credits)
MJ_GAME_LIST_ADD_GRAPHICS_CREDITS(graphics_credits)
// MJ_GAME_LIST_ADD_MUSIC_CREDITS(music_credits)
MJ_GAME_LIST_ADD_SFX_CREDITS(sfx_credits)

namespace ghost_capture
{
    hand::hand(const bn::fixed_t<8>& in_speed, int limits_width, int limits_height) :
        is_enabled(true),
        is_grabbing(false),
        speed(in_speed),
        hand_sprite(bn::sprite_items::ghost_capture_hand.create_sprite(0, 0))
    {
        bn::size size = hand_sprite.dimensions() / 2;

        screen_limits.set_position(-(limits_width / 2) + size.width(), -(limits_height / 2) + size.height());
        screen_limits.set_width(limits_width - size.width() * 2);
        screen_limits.set_height(limits_height - size.height() * 2);
    }

    void hand::move(const bn::fixed_point& direction)
    {
        if(!is_enabled)
        {
            return;
        }

        bn::fixed_point new_position = hand_sprite.position() + direction * speed;

        if(new_position.x() < screen_limits.left())
        {
            new_position.set_x(screen_limits.left());
        }
        else if(new_position.x() > screen_limits.right())
        {
            new_position.set_x(screen_limits.right());
        }
        
        if(new_position.y() < screen_limits.top())
        {
            new_position.set_y(screen_limits.top());
        }
        else if(new_position.y() > screen_limits.bottom())
        {
            new_position.set_y(screen_limits.bottom());
        }

        hand_sprite.set_position(new_position);
    }

    void hand::set_enabled(bool new_enabled)
    {
        if(new_enabled != is_enabled)
        {
            is_enabled = new_enabled;
            hand_sprite.set_visible(is_enabled);
        }
    }

    void hand::set_grab(bool new_grab)
    {
        if(is_enabled && new_grab != is_grabbing)
        {
            is_grabbing = new_grab;

            hand_sprite.set_tiles(bn::sprite_items::ghost_capture_hand.tiles_item().create_tiles(is_grabbing ? 1 : 0));
        }
    }

    cat::cat(int x, int y, int z) :
        cat_sprite(bn::sprite_items::ghost_capture_cat.create_sprite(x, y)),
        cat_animation(bn::sprite_animate_action<4>::forever(cat_sprite, 12, bn::sprite_items::ghost_capture_cat.tiles_item(), bn::span<const uint16_t>({ 0, 1, 2, 3 })))
    { 
        cat_sprite.set_z_order(z);
    }

    void cat::update()
    {
        cat_animation.update();
    }


    ghost::ghost(int z, bn::random& in_random, int in_speed) :
        is_scared(false),
        grab_state(grab_state::NONE),
        random(in_random),
        tick(0),
        initial_tick_y(random.get_int(2048)),
        eye_offset(-14),
        speed(in_speed),
        play_next_sound(60 * random.get_int(2)),
        danger_zone(0, 9, 22, 16),
        ghost_sprite(bn::sprite_items::ghost_capture_ghost.create_sprite(0, 0)),
        ghost_animation(bn::sprite_animate_action<4>::forever(ghost_sprite, 10, bn::sprite_items::ghost_capture_ghost.tiles_item(), bn::span<const uint16_t>({ 0, 1, 2, 3 }))),
        ghost_eyes_angry_sprite(bn::sprite_items::ghost_capture_ghost_eyes.create_sprite(0, eye_offset)),
        ghost_eyes_angry_animation(bn::sprite_animate_action<4>::forever(ghost_eyes_angry_sprite, 10, bn::sprite_items::ghost_capture_ghost_eyes.tiles_item(), bn::span<const uint16_t>({ 4, 5, 6, 7 }))),
        ghost_eyes_surprised_sprite(bn::sprite_items::ghost_capture_ghost_eyes.create_sprite(0, eye_offset)),
        ghost_eyes_surprised_animation(bn::sprite_animate_action<4>::forever(ghost_eyes_surprised_sprite, 10, bn::sprite_items::ghost_capture_ghost_eyes.tiles_item(), bn::span<const uint16_t>({ 0, 1, 2, 3 }))),
        ghost_pull_sprite(bn::sprite_items::ghost_capture_ghost_pull.create_sprite(0, 0)),
        ghost_pull_animation(bn::sprite_animate_action<6>::once(ghost_pull_sprite, 3, bn::sprite_items::ghost_capture_ghost_pull.tiles_item(), bn::span<const uint16_t>({ 0, 1, 2, 3, 4, 5 }))),
        baloon(bn::sprite_items::ghost_capture_baloon.create_sprite(0, 0))
    { 
        ghost_moans[0] = &bn::sound_items::ghost_capture_moan_1;
        ghost_moans[1] = &bn::sound_items::ghost_capture_moan_2;
        ghost_moans[2] = &bn::sound_items::ghost_capture_moan_3;
        ghost_moans[3] = &bn::sound_items::ghost_capture_moan_4;
        ghost_moans[4] = &bn::sound_items::ghost_capture_moan_5;

        ghost_sprite.set_z_order(z);

        ghost_eyes_angry_sprite.set_z_order(z - 1);
        ghost_eyes_surprised_sprite.set_z_order(z - 1);

        ghost_eyes_surprised_sprite.set_visible(false);

        ghost_pull_sprite.set_visible(false);
        baloon.set_visible(false);

        y_speed = (speed * 3.5);

        move();
    }

    void ghost::move()
    {
        bn::fixed_t x = 120 * bn::lut_cos((tick * speed) & 2047);
        bn::fixed_t y = 10 * bn::lut_cos((tick * y_speed) & 2047);
        bn::fixed_t major_y = 35 * bn::lut_cos(((initial_tick_y + tick) * 5) & 2047);

        ghost_sprite.set_position(bn::fixed_point(x, major_y + y));

        ghost_eyes_angry_sprite.set_position(bn::fixed_point(x, major_y + y + eye_offset));
        ghost_eyes_surprised_sprite.set_position(bn::fixed_point(x, major_y + y + eye_offset));

        danger_zone.set_position(x.integer(), major_y.integer() + y.integer() + 9);
    }

    void ghost::update()
    {
        switch(grab_state)
        {
            default:
            case NONE:
            {
                move();

                ghost_animation.update();

                ghost_eyes_surprised_animation.update();
                ghost_eyes_angry_animation.update();

                ++tick;

                if(play_next_sound <= 0)
                {
                    play_next_sound = (bn::fixed_t<12>(60) * random.get_fixed(2.5, 4.5)).integer();
                    int index = random.get_int(5);
                    ghost_moans[index]->play(1);
                }

                --play_next_sound;
                
                break;
            }
            case GRABBING:
            {
                if(!ghost_pull_animation.done())
                {
                    ghost_pull_animation.update();
                }
                else
                {
                    grab_state = grab_state::GRABBED;

                    baloon.set_position(ghost_pull_sprite.position());
                    baloon.set_visible(true);
                }
                break;
            }
            case GRABBED:
            {
                bn::fixed_point point = baloon.position();
                point.set_y(point.y() - 1);
                baloon.set_position(point);
            }
        }
    }

    void ghost::check_collision(ghost_capture::hand& hand)
    {
        bn::fixed_point hand_pivot = hand.get_position();
        set_scared(danger_zone.contains(bn::point(hand_pivot.x().integer(), hand_pivot.y().integer())));

        if(is_scared && hand.can_grab() && grab_state == grab_state::NONE)
        {
            ghost_eyes_surprised_sprite.set_visible(false);
            ghost_eyes_angry_sprite.set_visible(false);
            ghost_sprite.set_visible(false);

            ghost_pull_sprite.set_position(ghost_sprite.position());

            ghost_pull_sprite.set_visible(true);

            hand.set_enabled(false);

            grab_state = grab_state::GRABBING;
        }
    }

    void ghost::set_scared(bool new_scared)
    {
        if(is_scared != new_scared)
        {
            is_scared = new_scared;

            ghost_eyes_surprised_sprite.set_visible(is_scared);
            ghost_eyes_angry_sprite.set_visible(!is_scared);
        }
    }

    ghost_capture_game::ghost_capture_game(int completed_games, const mj::game_data& data) :
        _victory(false),
        _show_result_frames(40),
        hand(1.3, bn::display::width(), bn::display::height()),
        cat(-83, 4, 3),
        ghost(2, data.random, bn::min((completed_games / 5), 8) + 2),
        background(bn::regular_bg_items::ghost_capture_bg_forest.create_bg((256 - bn::display::width()) / 2, (256 - bn::display::height()) / 2))
    {
    }

    void ghost_capture_game::fade_in([[maybe_unused]] const mj::game_data& data)
    {
    }

    mj::game_result ghost_capture_game::play(const mj::game_data& data)
    {
        mj::game_result result;
        result.exit = data.pending_frames == 0;

        if(!_victory)
        {
            bn::fixed_point point(0, 0);
            bool is_grabbing = bn::keypad::a_held();

            if(bn::keypad::right_held())
            {
                point.set_x(1);
            }
            else if(bn::keypad::left_held())
            {
                point.set_x(-1);
            }

            if(bn::keypad::up_held())
            {
                point.set_y(-1);
            }
            else if(bn::keypad::down_held())
            {
                point.set_y(1);
            }

            hand.move(point);
            hand.set_grab(is_grabbing);

            ghost.check_collision(hand);

            _victory = (ghost.get_grab_state() == ghost::grab_state::GRABBED);
        }
        else
        {
            if(_show_result_frames)
            {
                --_show_result_frames;
            }
            else
            {
                result.exit = true;
            }
        }

        cat.update();
        ghost.update();

        return result;
    }

    void ghost_capture_game::fade_out([[maybe_unused]] const mj::game_data& data)
    {
    }

}
