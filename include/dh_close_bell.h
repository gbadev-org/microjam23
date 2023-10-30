/**
 * Generated from `blender/delete_empty_images.ts` script in DoorHell.gba repo.
 * 
 * Check out the script source code + Blender project files here:
 * https://github.com/SomeRanDev/DoorHell.gba/blob/main/blender/delete_empty_images.ts
 */

#pragma once

#include "dh_defines.h"

#include "bn_regular_bg_item.h"

#include "bn_regular_bg_items_dh_close_bell_1.h"
#include "bn_regular_bg_items_dh_close_bell_2.h"
#include "bn_regular_bg_items_dh_close_bell_3.h"
#include "bn_regular_bg_items_dh_close_bell_4.h"

DH_START_NAMESPACE

// ---------------------------------------------------------------
// FRAMES

#define ITEM(INDEX) &bn::regular_bg_items::dh_close_bell_ ## INDEX

constexpr int dh_close_bell_first_index = 22;
constexpr bn::regular_bg_item const* dh_close_bell_frames[] = {
	nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr,
	nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr,
	nullptr, nullptr, ITEM(1), ITEM(2), ITEM(3), ITEM(4), nullptr, nullptr, nullptr, nullptr,
	nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr,
	nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr,
	nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr,
	nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr
};

#undef ITEM

DH_END_NAMESPACE
