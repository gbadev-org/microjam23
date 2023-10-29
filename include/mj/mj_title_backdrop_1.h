#ifndef MJ_TITLE_BACKDROP_1_H
#define MJ_TITLE_BACKDROP_1_H

#include "bn_camera_ptr.h"
#include "bn_regular_bg_ptr.h"

#include "mj_title_backdrop.h"

namespace mj
{

class title_backdrop_1 final : public title_backdrop
{

public:
    title_backdrop_1();

    ~title_backdrop_1() final;

    [[nodiscard]] bn::optional<bn::fixed> update() final;

private:
    bn::regular_bg_ptr _bg_1;
    bn::regular_bg_ptr _bg_2;
    int _counter = 0;
    bn::camera_ptr _camera;

    void _update_bgs();
};

}

#endif
