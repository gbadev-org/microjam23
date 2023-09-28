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
        uint8_t games_count = 0;
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

    if(data.games_count < max_games)
    {
        data.functions[data.games_count] = function;
        ++data.games_count;
        return true;
    }

    data.failed = true;
    return false;
}

bn::span<function_type> get()
{
    static_data& data = static_data_instance();
    BN_BASIC_ASSERT(! data.failed, "Too much games added");

    return bn::span<function_type>(data.functions, data.games_count);
}

}
