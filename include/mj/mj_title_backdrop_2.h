#ifndef MJ_TITLE_BACKDROP_2_H
#define MJ_TITLE_BACKDROP_2_H

#include "bn_regular_bg_ptr.h"

#include "mj_title_backdrop.h"

namespace mj
{

class title_backdrop_2 final : public title_backdrop
{

public:
    title_backdrop_2();

    [[nodiscard]] bn::optional<bn::fixed> update() final;

private:
    bn::regular_bg_ptr _bg_1;
    bn::regular_bg_ptr _bg_2;
    bn::regular_bg_ptr _bg_3;
    int _counter = 0;

    void _update_bgs();
};

}

#endif
