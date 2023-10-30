#ifndef ABD_FALL_GAME_H
#define ABD_FALL_GAME_H

#include "bn_regular_bg_ptr.h"

#include "bn_sprite_ptr.h"
#include "bn_sprite_animate_actions.h"

#include "bn_sprite_items_mj_small_pumpkin.h"
#include "bn_sprite_items_abd_candy.h"

#include "mj/mj_game.h"

namespace abd
{

class abd_fall_game : public mj::game
{

public:

    static constexpr int minimum_frames = 3 * 60; //!< Minimum number of frames per game.
    static constexpr int maximum_frames = 5 * 60; //!< Maximum number of frames per game.

    abd_fall_game(int completed_games, const mj::game_data& data);

    [[nodiscard]] bn::string<16> title() const final
    {
        return "Catch the candy!";
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

    void setUpCandy(const mj::game_data& data){
        candyY = data.random.get_fixed(-120,-100);
        candyX = data.random.get_fixed(-55 - (25 * (gameSpeed-1)),55 + (25 * (gameSpeed-1)));
        candyFallSpeed = data.random.get_fixed(1.5 + (0.5 * (gameSpeed-1)),2 + (0.5 * (gameSpeed-1)));
        candyRotateDirection = data.random.get_fixed(0.7 + (1.0 * (gameSpeed-1)),1.5 + (1.0 * (gameSpeed-1)));
        _candySprite.set_position(candyX,candyY);
        _candySprite.set_horizontal_flip(data.random.get_int(0,1));
        _candySprite.set_vertical_flip(data.random.get_int(0,1));
        _candySprite.set_tiles(bn::sprite_items::abd_candy.tiles_item().create_tiles(data.random.get_int(0,7)));
    }

private:
    bn::regular_bg_ptr _bg;
    bn::sprite_ptr _playerSprite = bn::sprite_items::mj_small_pumpkin.create_sprite(0,50);
    bn::sprite_animate_action<4> _playerSpriteMoveAction = bn::create_sprite_animate_action_forever(_playerSprite,4,bn::sprite_items::mj_small_pumpkin.tiles_item(),1,2);
    bn::sprite_ptr _playerBowlSprite;
    bn::sprite_ptr _candySprite;

    int _total_frames;
    int _show_result_frames = 30;

    bn::fixed playerX = 0;
    bn::fixed playerY = 50;
    bn::fixed playerMoveSpeed = 3;

    bn::fixed bowlangle;

    int playerCatchRange = 48;

    bn::fixed candyX = 0;
    bn::fixed candyY = -100;
    bn::fixed candyFallSpeed = 2;
    bn::fixed candyRotateDirection = 0;
    int candiesCollected = 0;

    bn::fixed gameSpeed = 1;

    bool _victory = false;
    bool _defeat = false;
};

}

#endif
