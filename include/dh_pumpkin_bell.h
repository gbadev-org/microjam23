/**
 * Generated from `blender/delete_empty_images.ts` script in DoorHell.gba repo.
 * 
 * Check out the script source code + Blender project files here:
 * https://github.com/SomeRanDev/DoorHell.gba/blob/main/blender/delete_empty_images.ts
 */

#pragma once

#include "dh_defines.h"

#include "bn_regular_bg_item.h"

#include "bn_regular_bg_items_dh_pumpkin_bell_16.h"
#include "bn_regular_bg_items_dh_pumpkin_bell_17.h"
#include "bn_regular_bg_items_dh_pumpkin_bell_18.h"
#include "bn_regular_bg_items_dh_pumpkin_bell_19.h"
#include "bn_regular_bg_items_dh_pumpkin_bell_20.h"
#include "bn_regular_bg_items_dh_pumpkin_bell_26.h"
#include "bn_regular_bg_items_dh_pumpkin_bell_27.h"
#include "bn_regular_bg_items_dh_pumpkin_bell_28.h"
#include "bn_regular_bg_items_dh_pumpkin_bell_29.h"
#include "bn_regular_bg_items_dh_pumpkin_bell_30.h"

DH_START_NAMESPACE

// ---------------------------------------------------------------
// FRAMES

#define ITEM(INDEX) &bn::regular_bg_items::dh_pumpkin_bell_ ## INDEX

constexpr int dh_pumpkin_bell_first_index = 15;
constexpr bn::regular_bg_item const* dh_pumpkin_bell_frames[] = {
	nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr,
	nullptr, nullptr, nullptr, nullptr, nullptr, ITEM(16), ITEM(17), ITEM(18), ITEM(19), ITEM(20),
	nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, ITEM(26),
	ITEM(27), ITEM(28), ITEM(29), ITEM(30), nullptr, nullptr, nullptr, nullptr, nullptr, nullptr,
	nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr,
	nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr,
	nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr
};

#undef ITEM

DH_END_NAMESPACE
