/*
author          Oliver Blaser
date            03.01.2022
copyright       GNU GPLv3 - Copyright (c) 2022 Oliver Blaser
*/

#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <time.h>

#include "middleware/serialPort.h"


struct flags
{
    unsigned int verbose : 1;
};

static void initFlags(struct flags* flags);
static void printSerialPortError(const char* action, const SPO_port* port);
static void timespec_diff(const struct timespec* start, const struct timespec* end, struct timespec* res);
static int64_t timespec_diff_ms(const struct timespec* start, const struct timespec* end);
static void timespec_cpy(struct timespec* dest, const struct timespec* src);



int main(int argc, char** argv)
{
    struct flags flags;
    initFlags(&flags);

    for(int i = 1; i < argc; ++i)
    {
        if(strcmp(argv[i], "-v") == 0) flags.verbose = 1;
    }

    SPO_port portData;
    SPO_port* const port = &portData;

    SPO_init(port);
    if(flags.verbose && port->code != SPO_OK) printSerialPortError("init", port);
    
    SPO_open(port, "/dev/ttyS1", 9600);
    if(flags.verbose && port->code != SPO_OK) printSerialPortError("SPO_open", port);

    if(SPO_isOpen(port))
    {
        SPO_write(port, (uint8_t*)"AD-EEEE1\n", 9);
        if(flags.verbose && port->code != SPO_OK) printSerialPortError("SPO_write", port);

        char bufferOld[10];
        struct timespec tmspecOld, tmspecNow;
        clock_gettime(CLOCK_MONOTONIC_RAW, &tmspecOld);

        while(port->code == SPO_OK)
        {
            clock_gettime(CLOCK_MONOTONIC_RAW, &tmspecNow);

            if(timespec_diff_ms(&tmspecOld, &tmspecNow) >= 250)
            {
                timespec_cpy(&tmspecOld, &tmspecNow);

                uint8_t buffer[10];
                char* cBuffer = (char*)buffer;
                
                const time_t tNow = time(NULL);
                struct tm* datetime = localtime(&tNow);

                if(datetime)
                {
                    if(sprintf(cBuffer, "AD-%2i%02i%i\n", datetime->tm_hour, datetime->tm_min, (datetime->tm_sec & 0x01 ? 1 : 0)) != 9)
                    {
                        strcpy(cBuffer, "AD-CEEC1\n");
                    }
                }
                else strcpy(cBuffer, "AD-FFFF1\n");

                SPO_write(port, buffer, 9);
                if(flags.verbose && port->code != SPO_OK) printSerialPortError("SPO_write", port);

                if(flags.verbose && strncmp(cBuffer, bufferOld, 7) != 0)
                {
                    strcpy(bufferOld, cBuffer);
                    printf("%s", cBuffer);
                }
            }
        }

        SPO_close(port);
        if(flags.verbose && port->code != SPO_OK) printSerialPortError("SPO_close", port);
    }

    return -1;
}



void initFlags(struct flags* flags)
{
    flags->verbose = 0;
}

void printSerialPortError(const char* action, const SPO_port* port)
{
    char buffer[100];

    if(SPO_getErrorStr(port, buffer, sizeof(buffer)) != SPO_OK)
    {
        strcpy(buffer, "SPO_getErrorStr() failed!");
    }

    printf("%s: %s\n", action, buffer);
}

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
