#include "gvsnb_sky.h"

#include "bn_reciprocal_lut.h"

#include "bn_affine_bg_items_mj_credits_sky.h"

namespace gvsnb
{

namespace
{
    constexpr int camera_x = 0;
    constexpr int camera_y = bn::fixed(128).data() >> 4;
    constexpr int camera_cos = 256;
    constexpr int camera_sin = 0;
}

sky::sky() :
    _bg(bn::affine_bg_items::mj_credits_sky.create_bg(0, 0)),
    _pa_hbe(bn::affine_bg_pa_register_hbe_ptr::create(_bg, _pa_values)),
    _dx_hbe(bn::affine_bg_dx_register_hbe_ptr::create(_bg, _dx_values)),
    _dy_hbe(bn::affine_bg_dy_register_hbe_ptr::create(_bg, _dy_values))
{
}

void sky::update(bn::fixed tempo)
{
    bn::fixed dir_z = bn::fixed::from_data(32).unsafe_multiplication(tempo);
    _camera_z -= dir_z * camera_cos;
    _update_hbe_values();
    _pa_hbe.reload_values_ref();
    _dx_hbe.reload_values_ref();
    _dy_hbe.reload_values_ref();
}

void sky::_update_hbe_values()
{
    int camera_z = _camera_z.data();
    int y_shift = 160;

    for(int index = 0; index < 80; ++index)
    {
        int reciprocal = bn::reciprocal_lut[159 - index].data() >> 4;
        int lam = camera_y * reciprocal >> 12;
        int lcf = lam * camera_cos >> 8;
        int lsf = lam * camera_sin >> 8;

        _pa_values[index] = int16_t(lcf >> 4);

        int lxr = (240 / 2) * lcf;
        int lyr = y_shift * lsf;
        _dx_values[index] = (camera_x - lxr + lyr) >> 4;

        lxr = (240 / 2) * lsf;
        lyr = y_shift * lcf;
        _dy_values[index] = (camera_z - lxr - lyr) >> 4;
    }
}

}
