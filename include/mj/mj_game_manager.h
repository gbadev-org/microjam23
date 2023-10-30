#ifndef MJ_GAME_MANAGER_H
#define MJ_GAME_MANAGER_H

#include "bn_deque.h"
#include "bn_unique_ptr.h"

#include "mj_game.h"

namespace mj
{

class core;

class game_manager
{

public:
    game_manager(int completed_games, const game_data& data, core& core, bn::ideque<uint8_t>& game_history);

    ~game_manager();

    mj::game& game()
    {
        return *_game;
    }

private:
    bn::unique_ptr<mj::game> _game;
    int _used_alloc_ewram;
};

}

#endif
