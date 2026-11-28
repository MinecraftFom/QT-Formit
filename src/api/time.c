// Code written by Fom477
// Copyright Fom477 2026

#include <time.h>

#include <time/time.h>

long long GetCurrentMillis()
{
    struct timespec Time;
    clock_gettime(CLOCK_REALTIME, &Time);

    return Time.tv_sec * 1000 + Time.tv_nsec / 1000000;
}