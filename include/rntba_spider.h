#ifndef RNTBA_SPIDER_H
#define RNTBA_SPIDER_H

#include "bn_vector.h"

#include "bn_sprite_ptr.h"
#include "bn_sprite_affine_mat_ptr.h"

#include "mj_game_data.h"

namespace rntba {

class spider
{
public:
    //spider();
    spider(int completed_games, const mj::game_data& data, int total_frames);

    [[nodiscard]] int process(int hand_mode, int hand_top, int hand_len);

    [[nodiscard]] bool is_line_within_hurtbox(int top, int bot);

    void disable();

private:
    void update_position();
    void process_moving();
    void process_dropping(int hand_mode);
    void process_blastoff();
    void process_crawlarm();
    void process_shake();
    void start_drop_to_hand(int hand_top);
    void start_random_move();
    void start_shake(int hand_top);

    const int _body_top = 17;
    const int _body_bot = 29;
    const int _blast_dur = 30; // Frames it takes for the spooder to reach the heavens
    const int _upper_bound = -48; // -48 (-80 + 32) , 71 (80 - 9) , 16 ~ 23 = 39 steps of 3
    const int _lower_bound = 69; // spider's bounds are -48 and 69, it can move in steps divisible by 3; there are 39 of these steps within the two bounaries
    bn::sprite_ptr _spider;
    bn::sprite_ptr _face;
    bn::vector<bn::sprite_ptr, 20> _string;
    int _mode = 0; // 0:Idle , 1:Moving , 2:Tired , 3:BlastingOff , 4:FastMove , 5:FinishBlastOff , 6:CrawlHand , 7:Shake , 8:Drop
    int _visible_strings = 0;
    bn::sprite_affine_mat_ptr _affine_spider = bn::sprite_affine_mat_ptr::create();
    int _counter = 1; // Used for movement steps, the "hit to moon" anim, uhh a lot of shit idk lmao
    int _blast_start_y = 0; // When hit, this is the ylevel it started it's journey to the fuckin' moon at
    int _disabled = false; // I forgot what this was for
    int _start_wait = 0;
    int _action_speed = 0;
    int _big_act_counter = 0;
    int _y_destination = 0; // The y-value the spider is to move to, while moving
    int _post_act_wait = 0; // After moving, it waits for a bit - shorter wait if it moved a small distance, vice-versa
    int _walk_cycle_timer = 0; // Probs a smart way to do this using some other value, but I'm not smart (changes frame every 3px)
    const mj::game_data& _data; // Need access to RNG and remaining frames
    int _move_speed = 1; // px spider moves each frame, when moving - faster as difficulty increases (min 1 max 12)
    int _wait_speed = 5; // time spider waits for after moving - faster as difficulty increases, up to being removed entirely (min 5 max 0)
    int _shake_speed = 1; // time web shakes for when hit, less time as difficulty increases (min 1 max 6)
    int _shake_time = 60;
};

}

#endif
