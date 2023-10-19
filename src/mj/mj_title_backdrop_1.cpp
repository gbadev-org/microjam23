#include "mj/mj_title_backdrop_1.h"

#include "bn_rect_window.h"

#include "bn_regular_bg_items_mj_tombstone_1.h"
#include "bn_regular_bg_items_mj_tombstone_2.h"
#include "bn_regular_bg_items_mj_tombstone_3.h"

namespace mj
{

title_backdrop_1::title_backdrop_1() :
    _bg_1(bn::regular_bg_items::mj_tombstone_1.create_bg((256 - 240) / 2, (256 - 160) / 2)),
    _bg_2(bn::regular_bg_items::mj_tombstone_2.create_bg(_bg_1.x() + 240, (256 - 160) / 2)),
    _camera(bn::camera_ptr::create(0, 0))
{
    bn::rect_window internal_window = bn::rect_window::internal();
    internal_window.set_boundaries(-80, _bg_2.x() - 128, 80, _bg_2.x() + 112);
    internal_window.set_camera(_camera);
    bn::rect_window::outside().set_show_bg(_bg_2, false);

    _bg_1.set_camera(_camera);
    _bg_2.set_camera(_camera);
}

title_backdrop_1::~title_backdrop_1()
{
    bn::rect_window::internal().restore();
}

bn::optional<bn::fixed> title_backdrop_1::update()
{
    bn::optional<bn::fixed> fade_intensity;
    ++_counter;

    if(_counter == frames_per_bg_update)
    {
        bn::fixed camera_x = _camera.x() + 1;
        _counter = 0;

        if(camera_x == 240)
        {
            _bg_1.set_x(_bg_1.x() - 32);
            _bg_1.set_item(bn::regular_bg_items::mj_tombstone_3);
        }
        else if(camera_x == 456 - (fade_frames / frames_per_bg_update))
        {
            fade_intensity = 1;
        }
        else if(camera_x == 456)
        {
            _bg_1.set_x(_bg_1.x() + 32);
            _bg_1.set_item(bn::regular_bg_items::mj_tombstone_1);
            camera_x = 0;
            fade_intensity = 0;
        }

        _camera.set_x(camera_x);
    }

    return fade_intensity;
}

}
