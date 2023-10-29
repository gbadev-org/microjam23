#include "rntba_spider.h"

#include "bn_sprite_tiles_ptr.h"

#include "bn_sprite_items_rntba_spider.h"
#include "bn_sprite_items_rntba_string.h"
#include "bn_sprite_items_rntba_face.h"

#include "bn_sound_items.h"
//#include "bn_sound_items_info.h"

namespace rntba {

//spider::spider(){}

spider::spider(int completed_games, const mj::game_data& data, int total_frames) :
    _spider(bn::sprite_items::rntba_spider.create_sprite(0, 69, 0))
    , _face(bn::sprite_items::rntba_face.create_sprite(1, 69, 0))
    , _action_speed(total_frames / 20)
    , _data(data)
{
  // Start at a random point on it's vertical line
    /*int starty = data.random.get_int() % 40;
    starty = _upper_bound + (starty * 3);*/
    int starty = _upper_bound;
    _spider.set_y(starty);
    _y_destination = starty;
    // uh, nevermind, fuck that, spider always started like, right next to the hand, so it'll be set instead...

  // Change _move_speed, _wait_speed, _shake_speed based on completed games
    _move_speed = 1 + (completed_games/5); // 1~12
    if (_move_speed > 12)
      _move_speed = 12;
    _wait_speed = 5 - (completed_games/10); // 5~0
    if (_wait_speed < 0)
      _wait_speed = 0;
    _shake_speed = 1 + (completed_games/15); // 1~6
    if (_shake_speed > 6)
      _shake_speed = 6;

  // For when it spins+shrinks as it flies towards the moon
    _spider.set_affine_mat(_affine_spider);
    //_spider.set_double_size_mode(bn::sprite_double_size_mode::ENABLED);

    /*bn::sprite_tiles_ptr tiles = _face.tiles();
    int frame = data.random.get_int() % 3;
    tiles.set_tiles_ref(bn::sprite_items::rntba_face.tiles_item(), frame);*/

  // Web is updated as the spider moves, always connecting to the ceiling but not persisting below the spider
    int fuck = -80 + 4;
    for (int i=0; i<20; i++){
        bn::sprite_ptr web =  bn::sprite_items::rntba_string.create_sprite(0, fuck);
        web.set_z_order(100);
        web.set_visible(false);
        _string.push_back(bn::move(web));
        fuck += 8;
    }

    update_position();
}

int spider::process(int hand_mode, int hand_top, int hand_len)
{

  // Check if the hand hit the spider
    if (hand_mode == 4){
        bool is = is_line_within_hurtbox(hand_top,hand_top+hand_len);
        if (is){
            disable();
            bn::sound_items::rntba_hit.play();
        }
        else{
          bn::sound_items::rntba_miss.play();
          int ypos = _spider.y().floor_integer();
          if (ypos > hand_top)
            start_shake(hand_top);
        }
    }

    switch (_mode){

        case 0:
            /*if (hand_mode == 1){
                int ypos = _spider.y().floor_integer();
                if (ypos < hand_top)
                  start_drop_to_hand(hand_top); // If the hand is mid-flick, drop to it at 2x speed
            }
            else*/
            start_random_move(); // Or just shmop off some random direction
            break;

        case 1:
            process_moving();
            break;
        case 4:
            process_dropping(hand_mode);
            break;

        case 2:
            if (_post_act_wait) // Wait after finishing movement, for a bit
                _post_act_wait--;
            else
              _mode = 0;
            break;

        case 3:
            process_blastoff();
            break;

        case 6:
            process_crawlarm();
            break;

        case 7:
            process_shake();
            break;

        default:
          return _mode;
    }

    return _mode;
}

bool spider::is_line_within_hurtbox(int top, int bot)
{
    int y = _spider.y().floor_integer();
    int t = y - (31 - _body_top);
    int b = y - (31 - _body_bot);

  // Top of hit is beween top/bottom of body, OR bottom of hit is between top/bottom of body
    if ( (top > t && top < b) || (bot < t && bot > b) ){
        return true;
    }
  // Top is above body, bot is below body
    if ( top < t && bot > b ){
        return true;
    }

    return false;
}

void spider::disable(){
    _disabled = true;
    _counter = 0;
    _mode = 3;

    _blast_start_y = _spider.y().floor_integer();
    
    bn::sprite_tiles_ptr tiles = _spider.tiles();
    tiles.set_tiles_ref(bn::sprite_items::rntba_spider.tiles_item(), 2);

    _face.set_visible(false);

  // Final string segment changes to snapped frame
    bn::sprite_tiles_ptr frayTilePtr = bn::sprite_tiles_ptr::create(bn::sprite_items::rntba_string.tiles_item(), 1);
    _string[_visible_strings].set_tiles(frayTilePtr);
}

void spider::update_position(){
    int ypos = _spider.y().floor_integer();
  // Face
    _face.set_y(ypos - 7);

  // Web
    int startline = ypos + 72; // I pulled this number right out of my ass, I have no fucking clue why it is 72
    int strings_that_should_be_visible = startline / 8; // every 8 away should be a chunk
    if (strings_that_should_be_visible != _visible_strings){
        int dir = (strings_that_should_be_visible > _visible_strings) ? 1 : -1 ;
        bool vis = (dir == 1) ? true : false ;
        while (strings_that_should_be_visible != _visible_strings){
            _string[_visible_strings].set_visible(vis);
            _visible_strings += dir;
        }
    }
}

void spider::process_moving(){

  // Used for both movement and determining destination
    int ypos = _spider.y().floor_integer();

  // Has somewhere to be...
    if (ypos != _y_destination){

      // The relocation
        int vel = _move_speed;
        int dir = 1;
        if (_y_destination < ypos)
            dir = -1;
        int dest = ypos + (vel*dir);
        if ( ( (dir == 1) && (dest > _y_destination) ) || ( (dir == -1) && (dest < _y_destination) ) )
          dest = _y_destination;

        _spider.set_y(dest);
        update_position();

      // Swaps frame every 3f, as some semblance of locomotion
        bn::sprite_tiles_ptr tiles = _spider.tiles();
        int frame = (_walk_cycle_timer > 2) ? 1 : 0;
        tiles.set_tiles_ref(bn::sprite_items::rntba_spider.tiles_item(), frame);
        _walk_cycle_timer++;
        if (_walk_cycle_timer > 5){
          _walk_cycle_timer = 0;
          bn::sound_items::rntba_moving.play();
        }

    }
    else{
      _mode = 2;
    }
}

void spider::process_dropping(int hand_mode){
    int ypos = _spider.y().floor_integer();
    if (ypos != _y_destination){

        int vel = _move_speed * 2;
        int newy = ypos + vel;
        if (newy > _y_destination) // Uh oh, we moved PAST the destination, lets pull back a biiiit
            newy = _y_destination;
        _spider.set_y(newy);
        update_position();
    }
    else{

        if (hand_mode == 1){
          // something something you lose
            _mode = 6;
            bn::sprite_tiles_ptr tiles = _face.tiles();
            tiles.set_tiles_ref(bn::sprite_items::rntba_face.tiles_item(), 1);
        }
        else{
            bn::sprite_tiles_ptr tiles = _face.tiles();
            tiles.set_tiles_ref(bn::sprite_items::rntba_face.tiles_item(), 0);
            _mode = 2;
        }
      
    }
}

void spider::process_blastoff(){

    if (_counter > 9){
        _spider.set_visible(false);
        _mode = 5;
        return;
    }

    bn::fixed c = bn::fixed_t<12>(_counter);
    bn::fixed cp = bn::fixed_t<12>(1.0);
    bn::fixed ten = bn::fixed_t<12>(10.0);
    bn::fixed p = ( (c + cp) / ten);

    bn::fixed newx = -70.0 * p; // 0 to -70
    bn::fixed newy = -44.0 + ( (_blast_start_y - -44.0) * (1.0 - p) );
    _spider.set_position(newx,newy);

    int newrot = 15 * _counter;
    _affine_spider.set_rotation_angle(newrot);

    bn::fixed cf = bn::fixed_t<12>(0.1);
    bn::fixed newscale = cp - (c * cf);
    _affine_spider.set_horizontal_scale(newscale);
    _affine_spider.set_vertical_scale(newscale);

    _counter++;
}

void spider::process_crawlarm(){
}

void spider::start_random_move(){
    int ypos = _spider.y().floor_integer();

    //bn::sound_items::move.play()
    //bn::sound_items::sfx.play();
    bn::sound_items::rntba_move.play();

  // Determine the min and max we'll be moving
    //int min_steps =  3 * ( 1 + (_big_act_counter * 3) );
    //int max_steps =  9 * (1 + _big_act_counter);
    int min_steps =  4;//1 + (_big_act_counter * 3); // 1 - 4 -  7 - 10 ( 3,12,21,30)
    int max_steps =  16;//4 * (_big_act_counter + 1); // 4 - 8 - 12 - 16 (12,24,36,48)

  // Grab the values needed
    int step = _data.random.get_int() % (max_steps-min_steps + 1);
    int distance = ( step + min_steps ) * 3;
    int dir = _data.random.get_int() % 2;

  // If we'd move off the screen, change direction (0 is up, 1 is down)
    if ( dir == 0 && (ypos - distance < _upper_bound) )
        dir = 1;
    else if ( dir == 1 && (ypos + distance > _lower_bound) )
        dir = 0;

  // One last step: make dir a negative value if the spider is going up (moving negatively on the y-axis)
    if (!dir)
        dir = -1;

  // Set the destination, and post-move waiting time which is based on the distance
    _y_destination = ypos + (distance * dir);
    _post_act_wait = step * _wait_speed; //distance/2;

  // Progress to bigger steps, until reseting back to small
    _big_act_counter++;
    if (_big_act_counter > 3)
        _big_act_counter = 0;
    
    _mode = 1;
}

void spider::start_drop_to_hand(int hand_top){
    int ypos = _spider.y().floor_integer();

  // Whoops, hand is above us. Maybe not the best place to do this, whatev
    /*if (ypos > hand_top){
        start_random_move();
        return;
    }*/

    bn::sprite_tiles_ptr tiles = _face.tiles();
    tiles.set_tiles_ref(bn::sprite_items::rntba_face.tiles_item(), 2);

  // Keep the spider's movement at a step of 3
    int mod = hand_top % 3;
    if (mod != 0){
        hand_top += 3 - mod;
    }

  // Get hand's top y, set that as destination
  // Set mode to "fast fall" - it'll share code with normal move, but know to go 2x fast
    _y_destination = hand_top;
    int distance = hand_top - ypos;
    _post_act_wait = distance/2;

    _mode = 4;
}

void spider::start_shake(int hand_top){
    int ypos = _spider.y().floor_integer();

    _counter = 0;

    // If the hand was too close, the web breaks and the spider falls
    // Else, the web shakes and the spider halts

  // Whoops, hand is above us. Maybe not the best place to do this, whatev
    /*if ( (ypos+80) - (hand_top+80) < 1){
        _mode = 8;
        
        bn::sprite_tiles_ptr tiles = _face.tiles();
        tiles.set_tiles_ref(bn::sprite_items::rntba_face.tiles_item(), 5);
    }
    else{*/
        _mode = 7;

        // distance from spider affects shake duration: larger distance = more time
        _shake_time = (ypos+80) - (hand_top+80); // adjusted by 80 so the values are always at least 0
        _shake_time /= _shake_speed;

        bn::sprite_tiles_ptr tiles = _face.tiles();
        tiles.set_tiles_ref(bn::sprite_items::rntba_face.tiles_item(), 3);

      // Snap to vertical px divisible by 3
        int mod = ypos % 3;
        if (mod != 0){
          _spider.set_y(ypos + (3-mod));
          update_position();
        }
    //}
}

void spider::process_shake(){
    if (_counter > _shake_time){
      _mode = 0;
      for (int i=0; i<_visible_strings; i++){
          _string[i].set_x(0);
      }
      _spider.set_x(0);
      _face.set_x(1);
      bn::sprite_tiles_ptr tiles = _face.tiles();
      tiles.set_tiles_ref(bn::sprite_items::rntba_face.tiles_item(), 0);
      return;
    }

    if (_counter == 0 || _counter % 10 == 0){
      int rsfx = _data.random.get_int() % 3;
      if (rsfx == 0)
        bn::sound_items::rntba_shake1.play();
      else if (rsfx == 1)
        bn::sound_items::rntba_shake2.play();
      else
        bn::sound_items::rntba_shake3.play();

    }

  // Make everything (strings, spider/face) jitter horizontally
    int idx = _counter % 4; // 0:Middle,to right ; 1:Right ; 2:Middle,to left ; 3:Left

    for (int i=0; i<_visible_strings; i++){

      switch (idx){
        case 0:
          _string[i].set_x(0);
          break;

        case 1:
          _string[i].set_x(2);
          break;
          
        case 2:
          _string[i].set_x(0);
          break;
          
        case 3:
          _string[i].set_x(-2);
          break;

        default:
          break;
      }

      idx++;
      if (idx > 3)
        idx = 0;
    }

    int soff = 0;
    switch (idx){
      case 1:
        soff = 1;
        break;
      case 3:
        soff = -1;
        break;
      default:
        break;
    }
    _spider.set_x(soff);
    _face.set_x(soff+1);

    _counter++;
}

}
