/*
author          Oliver Blaser
date            04.01.2022
copyright       GNU GPLv3 - Copyright (c) 2022 Oliver Blaser
*/

#ifndef IG_APP_MAIN_H
#define IG_APP_MAIN_H

#include "../middleware/serialPort.h"


#define APP_COLON_MODE_OFF (0)
#define APP_COLON_MODE_ON (1)
#define APP_COLON_MODE_BLINK (2)

struct flags
{
    unsigned int argOk : 1;
    unsigned int envelope : 1;
    unsigned int help : 1;
    unsigned int verbose : 1;
    unsigned int version : 1;

    unsigned int colonMode : 2;
};

void APP_init(SPO_port* spo, const struct flags* flgs);
int APP_task();

int APP_envelope(const char* dispStr);

void APP_printSpoError(const char* action, const SPO_port* port, const struct flags* flags);

#endif // IG_APP_MAIN_H
