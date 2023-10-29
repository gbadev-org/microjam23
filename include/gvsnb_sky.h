#ifndef GVSNB_SKY_H
#define GVSNB_SKY_H

#include "bn_affine_bg_dx_register_hbe_ptr.h"
#include "bn_affine_bg_dy_register_hbe_ptr.h"
#include "bn_affine_bg_pa_register_hbe_ptr.h"
#include "bn_affine_bg_ptr.h"

namespace gvsnb
{

class sky
{

public:
    sky();

    void update(bn::fixed tempo);

private:
    alignas(int) int16_t _pa_values[160];
    alignas(int) int _dx_values[160];
    alignas(int) int _dy_values[160];
    bn::affine_bg_ptr _bg;
    bn::affine_bg_pa_register_hbe_ptr _pa_hbe;
    bn::affine_bg_dx_register_hbe_ptr _dx_hbe;
    bn::affine_bg_dy_register_hbe_ptr _dy_hbe;
    bn::fixed _camera_z;

    void _update_hbe_values();
};

}

#endif
