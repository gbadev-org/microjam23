#pragma once

#include "bn_log.h"

#define DH_START_NAMESPACE namespace dh {
#define DH_START_ANIMATIONS_NAMESPACE namespace dh::animations {
#define DH_END_NAMESPACE }

#define DH_UNUSED(v) (void)v

#define DH_FOR(len_expr) auto len = len_expr;\
for(decltype(len) i = 0; i < len; i++)
