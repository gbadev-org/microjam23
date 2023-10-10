#include "mj/mj_game_list.h"

namespace mj::game_list
{

namespace
{
    constexpr int max_games = 64;

    class static_data
    {

    public:
        function_type functions[max_games];
        bn::span<const bn::string_view> code_credits[max_games];
        bn::span<const bn::string_view> graphics_credits[max_games];
        bn::span<const bn::string_view> music_credits[max_games];
        bn::span<const bn::string_view> sfx_credits[max_games];
        uint8_t functions_count = 0;
        uint8_t code_credits_count = 0;
        uint8_t graphics_credits_count = 0;
        uint8_t music_credits_count = 0;
        uint8_t sfx_credits_count = 0;
        bool failed = false;
    };

    static_data& static_data_instance()
    {
        BN_DATA_EWRAM static static_data result;
        return result;
    }
}

bool add(function_type function)
{
    static_data& data = static_data_instance();

    if(data.functions_count < max_games)
    {
        data.functions[data.functions_count] = function;
        ++data.functions_count;
        return true;
    }

    data.failed = true;
    return false;
}

bn::span<function_type> get()
{
    static_data& data = static_data_instance();
    BN_BASIC_ASSERT(! data.failed, "Too much games added");

    return bn::span<function_type>(data.functions, data.functions_count);
}

bool add_code_credits(const bn::span<const bn::string_view>& credits)
{
    if(credits.empty())
    {
        return true;
    }

    static_data& data = static_data_instance();

    if(data.code_credits_count < max_games)
    {
        data.code_credits[data.code_credits_count] = credits;
        ++data.code_credits_count;
        return true;
    }

    data.failed = true;
    return false;
}

bn::span<bn::span<const bn::string_view>> get_code_credits()
{
    static_data& data = static_data_instance();
    BN_BASIC_ASSERT(! data.failed, "Too much games added");

    return bn::span<bn::span<const bn::string_view>>(data.code_credits, data.code_credits_count);
}

bool add_graphics_credits(const bn::span<const bn::string_view>& credits)
{
    if(credits.empty())
    {
        return true;
    }

    static_data& data = static_data_instance();

    if(data.graphics_credits_count < max_games)
    {
        data.graphics_credits[data.graphics_credits_count] = credits;
        ++data.graphics_credits_count;
        return true;
    }

    data.failed = true;
    return false;
}

bn::span<bn::span<const bn::string_view>> get_graphics_credits()
{
    static_data& data = static_data_instance();
    BN_BASIC_ASSERT(! data.failed, "Too much games added");

    return bn::span<bn::span<const bn::string_view>>(data.graphics_credits, data.graphics_credits_count);
}

bool add_music_credits(const bn::span<const bn::string_view>& credits)
{
    if(credits.empty())
    {
        return true;
    }

    static_data& data = static_data_instance();

    if(data.music_credits_count < max_games)
    {
        data.music_credits[data.music_credits_count] = credits;
        ++data.music_credits_count;
        return true;
    }

    data.failed = true;
    return false;
}

bn::span<bn::span<const bn::string_view>> get_music_credits()
{
    static_data& data = static_data_instance();
    BN_BASIC_ASSERT(! data.failed, "Too much games added");

    return bn::span<bn::span<const bn::string_view>>(data.music_credits, data.music_credits_count);
}

bool add_sfx_credits(const bn::span<const bn::string_view>& credits)
{
    if(credits.empty())
    {
        return true;
    }

    static_data& data = static_data_instance();

    if(data.sfx_credits_count < max_games)
    {
        data.sfx_credits[data.sfx_credits_count] = credits;
        ++data.sfx_credits_count;
        return true;
    }

    data.failed = true;
    return false;
}

bn::span<bn::span<const bn::string_view>> get_sfx_credits()
{
    static_data& data = static_data_instance();
    BN_BASIC_ASSERT(! data.failed, "Too much games added");

    return bn::span<bn::span<const bn::string_view>>(data.sfx_credits, data.sfx_credits_count);
}

}
