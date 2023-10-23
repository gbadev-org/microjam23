#include "gvsnb_utils.h"

#include "bn_array.h"
#include "bn_fixed.h"

namespace gvsnb::utils
{

namespace
{
    constexpr bn::array<bn::fixed, 160> x_mult_lut = []{
        bn::fixed initial_scale = 0.165;
        bn::fixed scale_1 = 0.28;
        bn::fixed scale_2 = 0.475;
        bn::fixed scale_3 = 1;
        bn::fixed final_scale = 1.75;

        bn::array<bn::fixed, 160> result;
        bn::fixed scale = initial_scale;

        for(int index = -80; index < y_1; ++index)
        {
            result[index + 80] = scale;
            scale += (scale_1 - initial_scale) / (y_1 - (-80));
        }

        scale = scale_1;

        for(int index = y_1; index < y_2; ++index)
        {
            result[index + 80] = scale;
            scale += (scale_2 - scale_1) / (y_2 - y_1);
        }

        scale = scale_2;

        for(int index = y_2; index < y_3; ++index)
        {
            result[index + 80] = scale;
            scale += (scale_3 - scale_2) / (y_3 - y_2);
        }

        scale = scale_3;

        for(int index = y_3; index < 80; ++index)
        {
            result[index + 80] = scale;
            scale += (final_scale - scale_3) / (80 - y_3);
        }

        return result;
    }();

    constexpr bn::array<bn::fixed, 160> scale_lut = []{
        bn::fixed initial_scale = 0.1;
        bn::fixed scale_1 = 0.18;
        bn::fixed scale_2 = 0.3;
        bn::fixed scale_3 = 1;
        bn::fixed final_scale = 2;

        bn::array<bn::fixed, 160> result;
        bn::fixed scale = initial_scale;

        for(int index = -80; index < y_1; ++index)
        {
            result[index + 80] = scale;
            scale += (scale_1 - initial_scale) / (y_1 - (-80));
        }

        scale = scale_1;

        for(int index = y_1; index < y_2; ++index)
        {
            result[index + 80] = scale;
            scale += (scale_2 - scale_1) / (y_2 - y_1);
        }

        scale = scale_2;

        for(int index = y_2; index < y_3; ++index)
        {
            result[index + 80] = scale;
            scale += (scale_3 - scale_2) / (y_3 - y_2);
        }

        scale = scale_3;

        for(int index = y_3; index < 80; ++index)
        {
            result[index + 80] = scale;
            scale += (final_scale - scale_3) / (80 - y_3);
        }

        return result;
    }();
}

bn::fixed x_mult(bn::fixed y)
{
    return x_mult_lut._data[bn::clamp(y.right_shift_integer() + 80, 0, 159)];
}

bn::fixed scale(bn::fixed y)
{
    return scale_lut._data[bn::clamp(y.right_shift_integer() + 80, 0, 159)];
}

}
