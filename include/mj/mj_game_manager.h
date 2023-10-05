#ifndef MJ_GAME_MANAGER_H
#define MJ_GAME_MANAGER_H

#include "bn_unique_ptr.h"

#include "mj_game.h"

namespace mj
{

class core;

class game_manager
{

public:
    game_manager(int completed_games, const mj::game_data& data, core& core);

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
