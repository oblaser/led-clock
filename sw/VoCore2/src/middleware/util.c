/*
author          Oliver Blaser
date            04.01.2022
copyright       GNU GPLv3 - Copyright (c) 2022 Oliver Blaser
*/

#include <stddef.h>
#include <stdint.h>
#include <time.h>

#include "util.h"



void timespec_diff(const struct timespec* start, const struct timespec* end, struct timespec* res)
{
    // todo: add null ptr check

    if ((end->tv_nsec - start->tv_nsec) < 0)
    {
        res->tv_sec = end->tv_sec - start->tv_sec - 1;
        res->tv_nsec = 1000000000 + end->tv_nsec - start->tv_nsec;
    }
    else
    {
        res->tv_sec = end->tv_sec - start->tv_sec;
        res->tv_nsec = end->tv_nsec - start->tv_nsec;
    }
}

int64_t timespec_diff_ms(const struct timespec* start, const struct timespec* end)
{
    // todo: add null ptr check and exact calc func

    struct timespec res;
    timespec_diff(start, end, &res);
    return (res.tv_sec * 1000) + (res.tv_nsec / 1000000);
}

void timespec_cpy(struct timespec* dest, const struct timespec* src)
{
    if(dest && src)
    {
        dest->tv_sec = src->tv_sec;
        dest->tv_nsec = src->tv_nsec;
    }
}
