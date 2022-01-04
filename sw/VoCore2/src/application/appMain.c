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


#define ENVELOPE_ANS_TIMEOUT (100)
int APP_envelope(const char* dispStr)
{
    int r;

    size_t rxIndex = 1;
    uint8_t rxBuffer[20];
    while(rxIndex && port->good)
    {
        SPO_read(port, rxBuffer, sizeof(rxBuffer), &rxIndex);
        APP_printSpoError("SPO_read", port, flags);
    }

    SPO_write(port, (uint8_t*)dispStr, strlen(dispStr));
    APP_printSpoError("SPO_write", port, flags);
    SPO_write(port, (uint8_t*)"\n", 1);
    APP_printSpoError("SPO_write", port, flags);

#if 0 // todo...
    for(size_t i = 0; i < sizeof(rxBuffer); ++i) rxBuffer[i] = 0;
    rxIndex = 0;
    int received = 0;
    int timeout = 0;
    while(!received && (timeout < ENVELOPE_ANS_TIMEOUT) && port->good)
    {
        size_t nReceived;
        SPO_read(port, rxBuffer + rxIndex, sizeof(rxBuffer) - rxIndex, &nReceived);
        APP_printSpoError("SPO_read", port, flags);

        if(nReceived > 0)
        {
            printf("%s\n", rxBuffer);
        }

        usleep(5 * 1000);
        ++timeout;
    }

    if(port->good)
    {
        if(timeout < ENVELOPE_ANS_TIMEOUT)
        {
            r = 0;
        }
        else
        {
            r = 2;
            if(flags->verbose) printf("ans timeout\n");
        }
    }
    else r = 1;
#else
    r = (port->good ? 0 : 1);
#endif

    return r;
}



void APP_printSpoError(const char* action, const SPO_port* port, const struct flags* flags)
{
    if(flags->verbose && !port->good)
    {
        char buffer[100];

        if(SPO_getErrorStr(port, buffer, sizeof(buffer)) != SPO_OK)
        {
            strcpy(buffer, "SPO_getErrorStr() failed!");
        }

        printf("%s: %s\n", action, buffer);
    }
}
