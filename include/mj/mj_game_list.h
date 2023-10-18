#ifndef MJ_GAME_LIST_H
#define MJ_GAME_LIST_H

#include "bn_string_view.h"

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

#define MJ_GAME_LIST_ADD_CODE_CREDITS_IMPL2(FunctionName, Credits, Line) \
    namespace { \
        bool MJ_GAME_LIST_UNIQUE_NAME(MJ_GAME_LIST_AUTO_REG_, Line){ mj::game_list::add_code_credits(Credits) }; \
    }

#define MJ_GAME_LIST_ADD_CODE_CREDITS_IMPL(Credits, Line) \
    MJ_GAME_LIST_ADD_CODE_CREDITS_IMPL2(MJ_GAME_LIST_UNIQUE_NAME(MJ_GAME_LIST_FUNCTION_, Line), Credits, Line)

#define MJ_GAME_LIST_ADD_GRAPHICS_CREDITS_IMPL2(FunctionName, Credits, Line) \
    namespace { \
        bool MJ_GAME_LIST_UNIQUE_NAME(MJ_GAME_LIST_AUTO_REG_, Line){ mj::game_list::add_graphics_credits(Credits) }; \
    }

#define MJ_GAME_LIST_ADD_GRAPHICS_CREDITS_IMPL(Credits, Line) \
    MJ_GAME_LIST_ADD_GRAPHICS_CREDITS_IMPL2(MJ_GAME_LIST_UNIQUE_NAME(MJ_GAME_LIST_FUNCTION_, Line), Credits, Line)

#define MJ_GAME_LIST_ADD_MUSIC_CREDITS_IMPL2(FunctionName, Credits, Line) \
    namespace { \
        bool MJ_GAME_LIST_UNIQUE_NAME(MJ_GAME_LIST_AUTO_REG_, Line){ mj::game_list::add_music_credits(Credits) }; \
    }

#define MJ_GAME_LIST_ADD_MUSIC_CREDITS_IMPL(Credits, Line) \
    MJ_GAME_LIST_ADD_MUSIC_CREDITS_IMPL2(MJ_GAME_LIST_UNIQUE_NAME(MJ_GAME_LIST_FUNCTION_, Line), Credits, Line)

#define MJ_GAME_LIST_ADD_SFX_CREDITS_IMPL2(FunctionName, Credits, Line) \
    namespace { \
        bool MJ_GAME_LIST_UNIQUE_NAME(MJ_GAME_LIST_AUTO_REG_, Line){ mj::game_list::add_sfx_credits(Credits) }; \
    }

#define MJ_GAME_LIST_ADD_SFX_CREDITS_IMPL(Credits, Line) \
    MJ_GAME_LIST_ADD_SFX_CREDITS_IMPL2(MJ_GAME_LIST_UNIQUE_NAME(MJ_GAME_LIST_FUNCTION_, Line), Credits, Line)


// PUBLIC MACROS //

#define MJ_GAME_LIST_ADD(GameClass) MJ_GAME_LIST_ADD_IMPL(GameClass, __LINE__)

#define MJ_GAME_LIST_ADD_CODE_CREDITS(Credits) MJ_GAME_LIST_ADD_CODE_CREDITS_IMPL(Credits, __LINE__)

#define MJ_GAME_LIST_ADD_GRAPHICS_CREDITS(Credits) MJ_GAME_LIST_ADD_GRAPHICS_CREDITS_IMPL(Credits, __LINE__)

#define MJ_GAME_LIST_ADD_MUSIC_CREDITS(Credits) MJ_GAME_LIST_ADD_MUSIC_CREDITS_IMPL(Credits, __LINE__)

#define MJ_GAME_LIST_ADD_SFX_CREDITS(Credits) MJ_GAME_LIST_ADD_SFX_CREDITS_IMPL(Credits, __LINE__)

// PUBLIC API //

namespace mj::game_list
{
    using function_type = mj::game*(*)(int, const mj::game_data&);

    [[nodiscard]] bool add(function_type function);

    [[nodiscard]] bn::span<function_type> get();

    [[nodiscard]] bool add_code_credits(const bn::span<const bn::string_view>& credits);

    [[nodiscard]] bn::span<bn::span<const bn::string_view>> get_code_credits();

    [[nodiscard]] bool add_graphics_credits(const bn::span<const bn::string_view>& credits);

    [[nodiscard]] bn::span<bn::span<const bn::string_view>> get_graphics_credits();

    [[nodiscard]] bool add_music_credits(const bn::span<const bn::string_view>& credits);

    [[nodiscard]] bn::span<bn::span<const bn::string_view>> get_music_credits();

    [[nodiscard]] bool add_sfx_credits(const bn::span<const bn::string_view>& credits);

    [[nodiscard]] bn::span<bn::span<const bn::string_view>> get_sfx_credits();
}

#endif
