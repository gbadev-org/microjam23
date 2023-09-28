#ifndef MJ_GAME_LIST_H
#define MJ_GAME_LIST_H

#include "mj_game.h"

// PRIVATE MACROS //

#define MJ_GAME_LIST_UNIQUE_NAME(Name, Line) Name##Line

#define MJ_GAME_LIST_ADD_IMPL2(FunctionName, GameClass, Line) \
    static mj::game* FunctionName(int completed_games, const mj::game_data& data); \
    namespace { \
        bool MJ_GAME_LIST_UNIQUE_NAME(MJ_GAME_LIST_AUTO_REG_, Line){ mj::game_list::add(&(FunctionName)) }; \
    } \
    static mj::game* FunctionName(int completed_games, const mj::game_data& data) { \
        return new GameClass(completed_games, data); \
    }

#define MJ_GAME_LIST_ADD_IMPL(GameClass, Line) \
    MJ_GAME_LIST_ADD_IMPL2(MJ_GAME_LIST_UNIQUE_NAME(MJ_GAME_LIST_FUNCTION_, Line), GameClass, Line)


// PUBLIC MACRO //

#define MJ_GAME_LIST_ADD(GameClass) MJ_GAME_LIST_ADD_IMPL(GameClass, __LINE__)

// PUBLIC API //

namespace mj::game_list
{
    using function_type = mj::game*(*)(int, const mj::game_data&);

    [[nodiscard]] bool add(function_type function);

    [[nodiscard]] bn::span<function_type> get();
}

#endif
