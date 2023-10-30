/**
 * Generated from `blender/delete_empty_images.ts` script in DoorHell.gba repo.
 * 
 * Check out the script source code + Blender project files here:
 * https://github.com/SomeRanDev/DoorHell.gba/blob/main/blender/delete_empty_images.ts
 */

#pragma once

#include "dh_defines.h"

#include "bn_regular_bg_item.h"

#include "bn_regular_bg_items_dh_right_bell_1.h"
#include "bn_regular_bg_items_dh_right_bell_2.h"
#include "bn_regular_bg_items_dh_right_bell_3.h"
#include "bn_regular_bg_items_dh_right_bell_4.h"
#include "bn_regular_bg_items_dh_right_bell_5.h"
#include "bn_regular_bg_items_dh_right_bell_6.h"
#include "bn_regular_bg_items_dh_right_bell_7.h"
#include "bn_regular_bg_items_dh_right_bell_8.h"
#include "bn_regular_bg_items_dh_right_bell_9.h"
#include "bn_regular_bg_items_dh_right_bell_10.h"
#include "bn_regular_bg_items_dh_right_bell_11.h"
#include "bn_regular_bg_items_dh_right_bell_12.h"
#include "bn_regular_bg_items_dh_right_bell_13.h"
#include "bn_regular_bg_items_dh_right_bell_14.h"

DH_START_NAMESPACE

// ---------------------------------------------------------------
// FRAMES

#define ITEM(INDEX) &bn::regular_bg_items::dh_right_bell_ ## INDEX

constexpr int dh_right_bell_first_index = 21;
constexpr bn::regular_bg_item const* dh_right_bell_frames[] = {
	nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr,
	nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr,
	nullptr, ITEM(1), ITEM(2), ITEM(3), ITEM(4), ITEM(5), nullptr, nullptr, nullptr, nullptr,
	nullptr, nullptr, nullptr, nullptr, nullptr, ITEM(6), ITEM(7), ITEM(8), ITEM(9), ITEM(10),
	nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr,
	ITEM(11), ITEM(12), ITEM(13), ITEM(14), nullptr, nullptr, nullptr, nullptr, nullptr, nullptr,
	nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr
};

#undef ITEM

DH_END_NAMESPACE
