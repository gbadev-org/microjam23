/**
 * Generated from `blender/generate_animation.ts` script in DoorHell.gba repo.
 * 
 * Check out the script source code + Blender project files here:
 * https://github.com/SomeRanDev/DoorHell.gba/blob/main/blender/generate_animation.ts
 */

#pragma once

#include "dh_defines.h"

#include "bn_regular_bg_item.h"

#include "bn_regular_bg_items_dh_window_bell_press_1.h"
#include "bn_regular_bg_items_dh_window_bell_press_2.h"
#include "bn_regular_bg_items_dh_window_bell_press_3.h"
#include "bn_regular_bg_items_dh_window_bell_press_4.h"
#include "bn_regular_bg_items_dh_window_bell_press_5.h"
#include "bn_regular_bg_items_dh_window_bell_press_6.h"
#include "bn_regular_bg_items_dh_window_bell_press_7.h"
#include "bn_regular_bg_items_dh_window_bell_press_8.h"
#include "bn_regular_bg_items_dh_window_bell_press_9.h"
#include "bn_regular_bg_items_dh_window_bell_press_10.h"
#include "bn_regular_bg_items_dh_window_bell_press_11.h"
#include "bn_regular_bg_items_dh_window_bell_press_12.h"
#include "bn_regular_bg_items_dh_window_bell_press_13.h"
#include "bn_regular_bg_items_dh_window_bell_press_14.h"
#include "bn_regular_bg_items_dh_window_bell_press_15.h"
#include "bn_regular_bg_items_dh_window_bell_press_16.h"
#include "bn_regular_bg_items_dh_window_bell_press_17.h"
#include "bn_regular_bg_items_dh_window_bell_press_18.h"
#include "bn_regular_bg_items_dh_window_bell_press_19.h"
#include "bn_regular_bg_items_dh_window_bell_press_20.h"
#include "bn_regular_bg_items_dh_window_bell_press_21.h"

DH_START_ANIMATIONS_NAMESPACE

// ---------------------------------------------------------------
// FRAMES

#define ITEM(INDEX) &bn::regular_bg_items::dh_window_bell_press_ ## INDEX

constexpr int dh_window_bell_press_data[] = {
	21, 0, 12
};
constexpr bn::regular_bg_item const* dh_window_bell_press_frames[] = {
	ITEM(1), ITEM(2), ITEM(3), ITEM(4), ITEM(5), ITEM(6), ITEM(7), ITEM(8), ITEM(9), ITEM(10), ITEM(11), ITEM(12), ITEM(13), ITEM(14), ITEM(15), ITEM(16), ITEM(17), ITEM(18), ITEM(19), ITEM(20), ITEM(21)
};

#undef ITEM

DH_END_NAMESPACE
