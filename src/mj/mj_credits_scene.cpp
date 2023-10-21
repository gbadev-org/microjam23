#include "mj/mj_credits_scene.h"

#include "bn_keypad.h"
#include "bn_reciprocal_lut.h"

#include "mj/mj_core.h"
#include "mj/mj_scene_type.h"

#include "bn_affine_bg_items_mj_big_pumpkin_night.h"
#include "bn_affine_bg_items_mj_credits_land.h"
#include "bn_affine_bg_items_mj_credits_sky.h"
#include "bn_sprite_items_mj_small_pumpkin_night.h"

namespace mj
{

namespace
{
    constexpr int camera_x = bn::fixed(256 - 12).data();
    constexpr int camera_y = bn::fixed(192).data() >> 4;
    constexpr int camera_cos = 256;
    constexpr int camera_sin = 0;
    constexpr int horizon_y = 80 - 36;
    constexpr int blending_height = 16;
    constexpr bn::fixed big_pumpkin_initial_distance = 8;
    constexpr bn::fixed big_pumpkin_final_distance = 0.5;
}

credits_scene::credits_scene(core& core) :
    _land_map(bn::affine_bg_items::mj_credits_land.create_map()),
    _sky_map(bn::affine_bg_items::mj_credits_sky.create_map()),
    _pumpkin_bg(bn::affine_bg_items::mj_big_pumpkin_night.create_bg(0, 0)),
    _bg(bn::affine_bg_ptr::create(0, 0, _land_map)),
    _pa_hbe(bn::affine_bg_pa_register_hbe_ptr::create(_bg, _pa_values)),
    _dx_hbe(bn::affine_bg_dx_register_hbe_ptr::create(_bg, _dx_values)),
    _dy_hbe(bn::affine_bg_dy_register_hbe_ptr::create(_bg, _dy_values)),
    _pumpkin_animation(bn::create_sprite_animate_action_forever(
                           bn::sprite_items::mj_small_pumpkin_night.create_sprite(0, 60), 16,
                           bn::sprite_items::mj_small_pumpkin_night.tiles_item(), 1, 0, 1, 2)),
    _credits(core),
    _transparency_attributes_hbe(bn::blending_transparency_attributes_hbe_ptr::create(_transparency_attributes))
{
    bn::affine_bg_attributes land_attributes(_land_map, 3, true, false);
    _bg_attributes.assign(horizon_y, bn::affine_bg_attributes(_sky_map, 3, true, false));

    while(! _bg_attributes.full())
    {
        _bg_attributes.push_back(land_attributes);
    }

    _bg_attributes_hbe = bn::affine_bg_attributes_hbe_ptr::create(
                _bg, bn::span<const bn::affine_bg_attributes>(_bg_attributes.data(), _bg_attributes.size()));

    for(int index = 0, limit = horizon_y - blending_height; index < limit; ++index)
    {
        bn::fixed intensity_alpha = 1 - (bn::fixed(index) / limit);
        _transparency_attributes[index].set_transparency_and_intensity_alphas(0.75, intensity_alpha);
    }

    for(int index = 0; index < blending_height; ++index)
    {
        bn::fixed alpha = index / bn::fixed(blending_height);
        _transparency_attributes[horizon_y + index].set_transparency_alpha(alpha);
        _transparency_attributes[horizon_y - index - 1].set_transparency_alpha(
                    alpha.unsafe_multiplication(bn::fixed(0.75)));
    }

    _pumpkin_bg.set_wrapping_enabled(false);
    _bg.set_blending_enabled(true);
    _update_gfx();
}

bn::optional<scene_type> credits_scene::update()
{
    bn::optional<scene_type> result;

    if(_credits.update())
    {
        _update_gfx();
    }
    else
    {
        result = scene_type::TITLE;
    }

    return result;
}

void credits_scene::_update_gfx()
{
    bn::fixed dir_z = bn::fixed::from_data(8);
    int updates = bn::keypad::a_held() ? credits::speed_up_frames : 1;
    _elapsed_frames += updates;
    _camera_z -= dir_z * camera_cos * updates;
    _update_hbe_values();
    _pa_hbe.reload_values_ref();
    _dx_hbe.reload_values_ref();
    _dy_hbe.reload_values_ref();

    for(int index = 0; index < updates; ++index)
    {
        _pumpkin_animation.update();
    }

    bn::fixed big_pumpkin_distance_diff = big_pumpkin_final_distance - big_pumpkin_initial_distance;
    bn::fixed distance_inc = (_elapsed_frames * big_pumpkin_distance_diff) / _credits.total_frames();
    bn::fixed distance = bn::max(big_pumpkin_initial_distance + distance_inc, big_pumpkin_final_distance);
    bn::fixed scale = bn::fixed(1).unsafe_division(distance);
    _pumpkin_bg.set_position(32, -56 - (scale * 52));
    _pumpkin_bg.set_scale(scale);
    _pumpkin_bg.set_rotation_angle(360 - ((distance - big_pumpkin_final_distance) * 2));
}

void credits_scene::_update_hbe_values()
{
    int camera_z = _camera_z.data();
    int y_shift = 160;

    for(int index = 0; index < horizon_y; ++index)
    {
        int reciprocal = bn::reciprocal_lut[horizon_y + horizon_y - index].data() >> 4;
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

    for(int index = horizon_y; index < 160; ++index)
    {
        int reciprocal = bn::reciprocal_lut[index].data() >> 4;
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
