/*
author          Oliver Blaser
date            04.01.2022
copyright       GNU GPLv3 - Copyright (c) 2022 Oliver Blaser
*/

/*
Module          Serial Port
Module abbr.    SPO
*/

#ifndef IG_MW_SERIALPORT_H
#define IG_MW_SERIALPORT_H


// part of omw, should be moved to another file...

//#if (defined(_WIN32) && !defined(_WIN64))
//#define OMW_PLAT_WIN32 (1)
//#endif
//#ifdef _WIN64
//#define OMW_PLAT_WIN64 (1)
//#endif
#ifdef __unix__
#define OMW_PLAT_UNIX (1)
#endif
#if (defined(OMW_PLAT_WIN32) || defined(OMW_PLAT_WIN64))
#define OMW_PLAT_WIN (1)
#endif
#ifdef OMW_CCDEF_PLAT_OPENWRT
#define OMW_PLAT_OPENWRT
#endif

#ifdef OMW_PLAT_OPENWRT
typedef int errno_t;
#endif






#include <errno.h>
#include <stddef.h>
#include <stdint.h>

#ifdef OMW_PLAT_UNIX
#include <termios.h>
#endif

#define SPO_NAME_SIZE (30)

#define SPO_BAUD_50         (50)
#define SPO_BAUD_75         (75)
#define SPO_BAUD_110        (110)
#define SPO_BAUD_134        (134)
#define SPO_BAUD_150        (150)
#define SPO_BAUD_200        (200)
#define SPO_BAUD_300        (300)
#define SPO_BAUD_600        (600)
#define SPO_BAUD_1200       (1200)
#define SPO_BAUD_1800       (1800)
#define SPO_BAUD_2400       (2400)
#define SPO_BAUD_4800       (4800)
#define SPO_BAUD_9600       (9600)
#define SPO_BAUD_19200      (19200)
#define SPO_BAUD_38400      (38400)
#define SPO_BAUD_57600      (57600)
#define SPO_BAUD_115200     (115200)
#define SPO_BAUD_230400     (230400)
#define SPO_BAUD_460800     (460800)
#define SPO_BAUD_500000     (500000)
#define SPO_BAUD_576000     (576000)
#define SPO_BAUD_921600     (921600)
#define SPO_BAUD_1000000    (1000000)
#define SPO_BAUD_1152000    (1152000)
#define SPO_BAUD_1500000    (1500000)
#define SPO_BAUD_2000000    (2000000)
#define SPO_BAUD_2500000    (2500000)
#define SPO_BAUD_3000000    (3000000)
#define SPO_BAUD_3500000    (3500000)
#define SPO_BAUD_4000000    (4000000)

enum SPO_CODE
{
    SPO_OK = 0,
    SPOE_INIT,      // just initialized
    SPOE_NULL,      // instance pointer is null
    SPOE_ERROR,     // general error
    SPOE_BAUD,      // invalid baud rate
    SPOE_FD,        // invalid file descriptor / handle
    SPOE_NAME,

    SPOE_TCGETATTR,
    SPOE_CFSETISPEED,
    SPOE_CFSETOSPEED,
    SPOE_TCSETATTR,

    SPOE_WR_COUNT,

    SPO__code_end_
};
typedef int SPO_code;

// serial port instance
typedef struct
{
    int baud;
    SPO_code code;
    errno_t error;
    int fd;
    int good;
    char name[SPO_NAME_SIZE];
} SPO_port;

SPO_code SPO_init(SPO_port* port);
int SPO_isOpen(SPO_port* port);
SPO_code SPO_open(SPO_port* port, const char* name, int baud/*, parity, nStop, nBits*/);
SPO_code SPO_close(SPO_port* port);
SPO_code SPO_read(SPO_port* port, uint8_t* buffer, size_t bufferSize, size_t* nBytesRead);
SPO_code SPO_write(SPO_port* port, const uint8_t* data, size_t count);

SPO_code SPO_getErrorStr(const SPO_port* port, char* buffer, size_t bufferSize);
SPO_code SPO_getErrorStr_code(SPO_code code, errno_t stderror, char* buffer, size_t bufferSize);

#endif // IG_MW_SERIALPORT_H
