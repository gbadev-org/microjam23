#ifndef GHOST_CAPTURE_GAME_H
#define GHOST_CAPTURE_GAME_H

#include <bn_sprite_ptr.h>
#include <bn_regular_bg_ptr.h>
#include <bn_rect.h>
#include <bn_sprite_animate_actions.h>
#include <bn_fixed.h>
#include <bn_fixed_point.h>
#include <bn_top_left_rect.h>
#include <bn_sound_item.h>

#include "mj/mj_game.h"


#define GHOST_GAME_FRAME_COUNT 7 * 60

namespace ghost_capture
{
    class hand
    {
        public:
            hand(const bn::fixed_t<8>& in_speed, int limits_width, int limits_height);

            void move(const bn::fixed_point& direction);

            bn::fixed_point get_position() { return hand_sprite.position(); }

            void set_enabled(bool new_enabled);

            bool enabled() { return is_enabled; }

            void set_grab(bool new_grab);

            bool can_grab() { return is_grabbing; } 
        private:
            bool is_enabled;

            bool is_grabbing;

            bn::fixed_t<8> speed;

            bn::sprite_ptr hand_sprite;

            bn::top_left_rect screen_limits;
    };

    class cat
    {
        public:
            cat(int x, int y, int z);

            void update();

        private:
            bn::sprite_ptr cat_sprite;
            bn::sprite_animate_action<4> cat_animation;
    };

    class ghost
    {
        public:
            enum grab_state : uint8_t
            {
                NONE,
                GRABBING,
                GRABBED
            };

            ghost(int z, bn::random& in_random, int in_speed);

            void update();

            void check_collision(ghost_capture::hand& hand);

            grab_state get_grab_state() { return grab_state; }
        
        private:
            void set_scared(bool new_scared);

            void move();

            bool is_scared;

            grab_state grab_state;

            bn::random& random;

            int tick;
            int initial_tick_y;
            int eye_offset;
            int speed;
            int y_speed;

            int play_next_sound;

            const bn::sound_item* ghost_moans[5];

            bn::rect danger_zone;

            bn::sprite_ptr ghost_sprite;
            bn::sprite_animate_action<4> ghost_animation;

            bn::sprite_ptr ghost_eyes_angry_sprite;
            bn::sprite_animate_action<4> ghost_eyes_angry_animation;

            bn::sprite_ptr ghost_eyes_surprised_sprite;
            bn::sprite_animate_action<4> ghost_eyes_surprised_animation;

            bn::sprite_ptr ghost_pull_sprite;
            bn::sprite_animate_action<6> ghost_pull_animation;

            bn::sprite_ptr baloon;
    };

    class ghost_capture_game : public mj::game
    {
        public:
            ghost_capture_game(int completed_games, const mj::game_data& data);

            [[nodiscard]] bn::string<16> title() const final
            {
                return "GRAB THE GHOST!";
            }

            [[nodiscard]] int total_frames() const final
            {
                return GHOST_GAME_FRAME_COUNT;
            }

            void fade_in(const mj::game_data& data) final;

            [[nodiscard]] mj::game_result play(const mj::game_data& data) final;

            [[nodiscard]] bool victory() const final
            {
                return _victory;
            }

            void fade_out(const mj::game_data& data) final;

        private:
            bool _victory;
            int _show_result_frames;
            ghost_capture::hand hand;
            ghost_capture::cat cat;
            ghost_capture::ghost ghost;
            bn::regular_bg_ptr background;
    };
}

#endif  //GHOST_CAPTURE_GAME_H
