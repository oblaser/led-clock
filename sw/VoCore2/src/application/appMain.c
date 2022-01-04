/*
author          Oliver Blaser
date            04.01.2022
copyright       GNU GPLv3 - Copyright (c) 2022 Oliver Blaser
*/

#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <time.h>

#include "appMain.h"

#include <unistd.h>



static const struct flags* flags;
static SPO_port* port;
static char cBufferOld[10];
static uint8_t buffer[10];
static char* cBuffer = (char*)buffer;



void APP_init(SPO_port* spo, const struct flags* flgs)
{
    flags = flgs;
    port = spo;
}

int APP_task()
{
    const time_t tNow = time(NULL);
    struct tm* datetime = localtime(&tNow);

    if(datetime)
    {
        int colon;
        if(flags->colonMode == APP_COLON_MODE_ON) colon = 1;
        else if(flags->colonMode == APP_COLON_MODE_BLINK) colon = (datetime->tm_sec & 0x01 ? 0 : 1);
        else colon = 0;

        if(sprintf(cBuffer, "AD-%2i%02i%i\n", datetime->tm_hour, datetime->tm_min, colon) != 9)
        {
            strcpy(cBuffer, "AD-CEEC1\n");
        }
    }
    else strcpy(cBuffer, "AD-FFFF1\n");

    SPO_write(port, buffer, 9);
    APP_printSpoError("SPO_write", port, flags);

    if(flags->verbose && strncmp(cBuffer, cBufferOld, 7) != 0)
    {
        strcpy(cBufferOld, cBuffer);
        printf("%s", cBuffer);
    }

    return (port->code == SPO_OK ? 0 : 1);
}



int APP_envelope(const char* dispStr)
{
    int r;

    SPO_write(port, (uint8_t*)dispStr, strlen(dispStr));
    APP_printSpoError("SPO_write", port, flags);
    SPO_write(port, (uint8_t*)"\n", 1);
    APP_printSpoError("SPO_write", port, flags);

    size_t rxIndex = 0;
    uint8_t rxBuffer[20];
    int received = 0;
    while(!received)
    {
        received = 1;
    }

    r = 0;

    return r;
}



void APP_printSpoError(const char* action, const SPO_port* port, const struct flags* flags)
{
    if(flags->verbose && port->code != SPO_OK)
    {
        char buffer[100];

        if(SPO_getErrorStr(port, buffer, sizeof(buffer)) != SPO_OK)
        {
            strcpy(buffer, "SPO_getErrorStr() failed!");
        }

        printf("%s: %s\n", action, buffer);
    }
}
