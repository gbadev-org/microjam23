#include "mj/mj_title_backdrop_2.h"

#include "bn_regular_bg_items_mj_graveyard_1.h"
#include "bn_regular_bg_items_mj_graveyard_2.h"
#include "bn_regular_bg_items_mj_graveyard_3.h"

namespace mj
{

title_backdrop_2::title_backdrop_2() :
    _bg_1(bn::regular_bg_items::mj_graveyard_1.create_bg((256 - 240) / 2, (256 - 160) / 2)),
    _bg_2(bn::regular_bg_items::mj_graveyard_2.create_bg((256 - 240) / 2, (256 - 160) / 2)),
    _bg_3(bn::regular_bg_items::mj_graveyard_3.create_bg((256 - 240) / 2, (256 - 160) / 2))
{
}

bn::optional<bn::fixed> title_backdrop_2::update()
{
    ++_counter;

    if(_counter == frames_per_bg_update)
    {
        _bg_2.set_x(_bg_2.x() - 1);
        _counter = 0;
    }

    return bn::nullopt;
}

}
