/*
author          Oliver Blaser
date            03.01.2022
copyright       GNU GPLv3 - Copyright (c) 2022 Oliver Blaser
*/

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char** argv)
{
    printf("sizeof types:\n");
    printf("\n");
    printf("char: %2u\n", sizeof(char));
    printf("short: %2u\n", sizeof(short));
    printf("int: %2u\n", sizeof(int));
    printf("long: %2u\n", sizeof(long));
    printf("long long: %2u\n", sizeof(long long));
    printf("float: %2u\n", sizeof(float));
    printf("double: %2u\n", sizeof(double));
    printf("long double: %2u\n", sizeof(long double));
    printf("\n");
    printf("int8_t: %2u\n", sizeof(int8_t));
    printf("int16_t: %2u\n", sizeof(int16_t));
    printf("int32_t: %2u\n", sizeof(int32_t));
    printf("int64_t: %2u\n", sizeof(int64_t));
    printf("uint8_t: %2u\n", sizeof(uint8_t));
    printf("uint16_t: %2u\n", sizeof(uint16_t));
    printf("uint32_t: %2u\n", sizeof(uint32_t));
    printf("uint64_t: %2u\n", sizeof(uint64_t));
    return 0;
}
