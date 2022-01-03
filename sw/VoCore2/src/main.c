/*
author          Oliver Blaser
date            03.01.2022
copyright       GNU GPLv3 - Copyright (c) 2022 Oliver Blaser
*/

#include <stdio.h>
#include <string.h>

#include "serialPort.h"

static void printSerialPortError(const char* action, const SPO_port* port)
{
    char buffer[100];

    if(SPO_getErrorStr(port, buffer, sizeof(buffer)) != SPO_OK)
    {
        strcpy(buffer, "SPO_getErrorStr() failed!");
    }

    printf("%s: %s\n", action, buffer);
}

int main(/*int argc, char** argv*/)
{
    SPO_port portData;
    SPO_port* const port = &portData;

    SPO_init(port);
    printSerialPortError("init", port);
    
    SPO_open(port, "/dev/ttyS1", 9600);
    printSerialPortError("open", port);
    
    SPO_write(port, (uint8_t*)"AD-12340\n", 9);
    printSerialPortError("write", port);

    SPO_close(port);
    printSerialPortError("close", port);

    return 0;
}
