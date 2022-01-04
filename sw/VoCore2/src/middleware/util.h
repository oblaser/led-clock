/*
author          Oliver Blaser
date            04.01.2022
copyright       GNU GPLv3 - Copyright (c) 2022 Oliver Blaser
*/

#ifndef IG_MW_UTIL_H
#define IG_MW_UTIL_H

#include <stddef.h>
#include <stdint.h>
#include <time.h>

void timespec_diff(const struct timespec* start, const struct timespec* end, struct timespec* res);
int64_t timespec_diff_ms(const struct timespec* start, const struct timespec* end);
void timespec_cpy(struct timespec* dest, const struct timespec* src);

#endif // IG_MW_UTIL_H
