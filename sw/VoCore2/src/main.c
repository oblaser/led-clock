/*
author          Oliver Blaser
date            04.01.2022
copyright       GNU GPLv3 - Copyright (c) 2022 Oliver Blaser
*/

#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>

#include "application/appMain.h"
#include "middleware/serialPort.h"

#include <unistd.h>



static char envelopeData[20];

static void initFlags(struct flags* flags);



int main(int argc, char** argv)
{
    int r = -1;

    struct flags flagsData;
    struct flags* const flags = &flagsData;
    initFlags(flags);

    for(int i = 1; i < argc; ++i)
    {
        int argOk = 1;
        const size_t argLen = strlen(argv[i]);

        if(strcmp(argv[i], "-v") == 0) flags->verbose = 1;
        else if((strncmp(argv[i], "--cm=", 5) == 0) && (argLen == 6)) flags->colonMode = *(argv[i] + 5) - 0x30;
        else if((strncmp(argv[i], "--envelope=", 11) == 0) && (argLen > 11) && (argLen < (11 + sizeof(envelopeData))))
        {
            strcpy(envelopeData, argv[i] + 11);
            flags->envelope = 1;
        }
        else argOk = 0;

        if(!argOk)
        {
            flags->argOk = 0;
            printf("invalid argument: \"%s\"\n", argv[i]);
        }
    }


    if(flags->argOk)
    {
        SPO_port portData;
        SPO_port* const port = &portData;

        if(SPO_init(port) != SPO_OK) APP_printSpoError("SPO_init", port, flags);
        
        SPO_open(port, "/dev/ttyS1", 9600);
        APP_printSpoError("SPO_open", port, flags);

        if(SPO_isOpen(port))
        {
            APP_init(port, flags);

            if(flags->envelope)
            {
                r = APP_envelope(envelopeData);
            }
            else
            {
                int taskRes = 0;
                while(taskRes == 0)
                {
                    taskRes = APP_task();
                    usleep(250 * 1000);
                }

                r = 1;
            }

            SPO_close(port);
            APP_printSpoError("SPO_close", port, flags);
        }
        else r = -3;
    }
    else
    {
        printf("\nUsage: led-clock [-v] [--cm=(0|1|2)] [--envelope=DISPSTR]\n\n");

        r = -2;
    }

    return r;
}



void initFlags(struct flags* flags)
{
    flags->argOk = 1;
    flags->envelope = 0;
    flags->verbose = 0;

    flags->colonMode = APP_COLON_MODE_BLINK;
}
