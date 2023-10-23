#ifndef GVSNB_UTILS_H
#define GVSNB_UTILS_H

#include "bn_fixed_fwd.h"

namespace gvsnb::utils
{

constexpr int door_x = 72;

constexpr int y_1 = -69;
constexpr int y_2 = -57;
constexpr int y_3 = -22;

constexpr int enemy_z_order = 0;
constexpr int candy_z_order = -4;
constexpr int big_enemy_z_order = -6;
constexpr int hand_z_order = -10;

[[nodiscard]] bn::fixed x_mult(bn::fixed y);

[[nodiscard]] bn::fixed scale(bn::fixed y);

}

#endif
