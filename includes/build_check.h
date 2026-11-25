// Code written by Fom477
// Copyright Fom477 2026
// This file is for module dependency check

#ifndef BUILD_CHECK
#define BUILD_CHECK(module) (FORMIT_##module - 1 == 0) // If target module isn't built, than the param will be *space* - 1 = -1, else, it will be 1 - 1 = 0
#define DEPRECATED(SINCE)
#endif