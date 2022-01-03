/*
author          Oliver Blaser
date            03.01.2022
copyright       GNU GPLv3 - Copyright (c) 2022 Oliver Blaser
*/

#include <errno.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>

#include "serialPort.h"

#ifdef OMW_PLAT_UNIX
#include <fcntl.h>
#include <sys/types.h>
#include <termios.h>
#include <unistd.h>
#endif



#ifdef OMW_PLAT_UNIX
static speed_t getUnixBaud(int baud, int* error);
#else
static int isValidBaud(int baud);
#endif

#ifndef RSIZE_MAX
#define RSIZE_MAX (SIZE_MAX >> 1)
#endif


SPO_code SPO_init(SPO_port* port)
{
    SPO_code r = SPO_OK;
    
    if(port)
    {
        port->baud = -1;
        port->code = SPO_OK;
        port->error = 0;
        port->fd = -1;
        port->name[0] = 0;
    }
    else r = SPOE_NULL;

    return r;
}

int SPO_isOpen(SPO_port* port)
{
    int r = 0;
    
    if(port)
    {
        if(port->fd >= 0) r = 1;
    }

    return r;
}

SPO_code SPO_open(SPO_port* port, const char* name, int baud/*, parity, nStop, nBits*/)
{
    SPO_code r = SPO_OK;
    
    if(port)
    {
        port->error = 0;

        if(name)
        {
            const size_t nameLen = strlen(name);
            if(nameLen < RSIZE_MAX && (nameLen + 1) <= SPO_NAME_SIZE) strcpy(port->name, name);
            else r = SPOE_NAME;
        }

        if(r == SPO_OK)
        {
#ifdef OMW_PLAT_WIN
#error "not implemented"
            port->baud = baud;
            if(isValidBaud(port->baud))
            {
            }
            else r = SPOE_BAUD;
#elif defined(OMW_PLAT_UNIX)

            port->fd = open(port->name, O_RDWR);

            if(port->fd >= 0)
            {
                struct termios tty;
                if(tcgetattr(port->fd, &tty) == 0)
                {
                    tty.c_cflag &= ~PARENB; // no parity
                    tty.c_cflag &= ~CSTOPB; // clear stop field => one stop bit
                    tty.c_cflag &= ~CSIZE;
                    tty.c_cflag |= CS8; // 8bit data word
                    //tty.c_cflag &= ~CRTSCTS; // disable RTS/CTS hardware flow control
                    tty.c_cflag |= CREAD | CLOCAL; // turn on READ & ignore ctrl lines (CLOCAL = 1)
                    
                    tty.c_lflag &= ~ICANON; // non-canonical mode
                    tty.c_lflag &= ~ECHO; // disable echo
                    tty.c_lflag &= ~ECHOE; // disable erasure
                    tty.c_lflag &= ~ECHONL; // disable new-line echo
                    tty.c_lflag &= ~ISIG; // disable interpretation of INTR, QUIT and SUSP
                    
                    tty.c_iflag &= ~(IXON | IXOFF | IXANY); // turn off s/w flow ctrl
                    tty.c_iflag &= ~(IGNBRK | BRKINT | PARMRK | ISTRIP | INLCR | IGNCR | ICRNL); // disable any special handling of received bytes
                    
                    tty.c_oflag &= ~OPOST; // prevent special interpretation of output bytes (e.g. newline chars)
                    tty.c_oflag &= ~ONLCR; // prevent conversion of newline to carriage return/line feed
                    //tty.c_oflag &= ~OXTABS; // prevent conversion of tabs to spaces (NOT PRESENT IN LINUX)
                    //tty.c_oflag &= ~ONOEOT; // prevent removal of C-d chars (0x004) in output (NOT PRESENT IN LINUX)
                    
                    // non blocking
                    tty.c_cc[VTIME] = 0;
                    tty.c_cc[VMIN] = 0;
                    
                    port->baud = baud;
                    int unixBaudErr;
                    const int unixBaud = getUnixBaud(port->baud, &unixBaudErr);
                    if(unixBaudErr == 0)
                    {
                        if(cfsetispeed(&tty, unixBaud) == 0)
                        {
                            if(cfsetospeed(&tty, unixBaud) == 0)
                            {
                                if(tcsetattr(port->fd, TCSANOW, &tty) == 0) r = SPO_OK;
                                else
                                {
                                    r = SPOE_TCSETATTR;
                                    port->error = errno;
                                }
                            }
                            else
                            {
                                r = SPOE_CFSETOSPEED;
                                port->error = errno;
                            }
                        }
                        else
                        {
                            r = SPOE_CFSETISPEED;
                            port->error = errno;
                        }
                    }
                    else r = SPOE_BAUD;
                }
                else
                {
                    r = SPOE_TCGETATTR;
                    port->error = errno;
                }
            }
            else
            {
                r = SPOE_FD;
                port->error = errno;
            }
#else
#error "unknown platform"
#endif
        }

        port->code = r;
    }
    else r = SPOE_NULL;
    
    return r;
}

SPO_code SPO_close(SPO_port* port)
{
    SPO_code r;
    
    if(port)
    {
        port->error = 0;
        
#ifdef OMW_PLAT_WIN
#error "not implemented"
#elif defined(OMW_PLAT_UNIX)
        if(close(port->fd) == 0) r = SPO_OK;
        else
        {
            r = SPOE_ERROR;
            port->error = errno;
        }
#else
#error "unknown platform"
#endif

        port->code = r;
    }
    else r = SPOE_NULL;
    
    return r;
}

SPO_code SPO_write(SPO_port* port, const uint8_t* data, size_t count)
{
    SPO_code r;
    
    if(port)
    {
        port->error = 0;
        
#ifdef OMW_PLAT_WIN
#error "not implemented"
#elif defined(OMW_PLAT_UNIX)
        const ssize_t wrres = write(port->fd, data, count);
        if(wrres < 0)
        {
            r = SPOE_ERROR;
            port->error = errno;
        }
        else
        {
            if((ssize_t)count == wrres) r = SPO_OK;
            else r = SPOE_WR_COUNT;
        }
#else
#error "unknown platform"
#endif

        port->code = r;
    }
    else r = SPOE_NULL;
    
    return r;
}

SPO_code SPO_getErrorStr(const SPO_port* port, char* buffer, size_t size)
{
    SPO_code r;
    
    if(port)
    {
        r = SPO_getErrorStr_code(port->code, port->error, buffer, size);
    }
    else r = SPOE_NULL;
    
    return r;
}

SPO_code SPO_getErrorStr_code(SPO_code code, errno_t stderror, char* buffer, size_t bufferSize)
{
    SPO_code r;
    char unknownBuffer[35];
    const char* errStr = "SPO_getErrorStr_code() internal error";

    if(code == SPO_OK) errStr = "OK";
    else if(code == SPOE_NULL) errStr = "instance pointer is null";
    else if(code == SPOE_ERROR) errStr = "error";
    else if(code == SPOE_BAUD) errStr = "invalid baud rate";
    else if(code == SPOE_FD) errStr = "invalid file descriptor";
    else if(code == SPOE_NAME) errStr = "invalid name";
    else if(code == SPOE_TCGETATTR) errStr = "tcgetattr failed";
    else if(code == SPOE_CFSETISPEED) errStr = "cfsetispeed failed";
    else if(code == SPOE_CFSETOSPEED) errStr = "cfsetospeed failed";
    else if(code == SPOE_TCSETATTR) errStr = "tcsetattr failed";
    else if(code == SPOE_WR_COUNT) errStr = "did not write all bytes";
    else
    {
        if(sprintf(unknownBuffer, "unknown error code: %i", code) > 0) errStr = unknownBuffer;
        else errStr = "unknown error code";
    }

    if(buffer)
    {
        const size_t errStrLen = strlen(errStr);
        if(bufferSize > (errStrLen + 1))
        {
            strcpy(buffer, errStr);
            r = SPO_OK;
        }
        else r = SPOE_ERROR;
    }
    else r = SPOE_ERROR;

    if(r == SPO_OK && stderror != 0)
    {
        const size_t tmpBufferLen = strlen(buffer);
        int stdBufferLen;
        char stdBuffer[50];
        int appendStdBuffer;
        
        stdBufferLen = sprintf(stdBuffer, " (%i: %s)", stderror, strerror(stderror));

        if(stdBufferLen > 0 && (tmpBufferLen + (size_t)stdBufferLen + 1) <= bufferSize) appendStdBuffer = 1;
        else
        {
            stdBufferLen = sprintf(stdBuffer, " (%i)", stderror);
            if(stdBufferLen > 0 && (tmpBufferLen + (size_t)stdBufferLen + 1) <= bufferSize) appendStdBuffer = 1;
            else appendStdBuffer = 0;
        }

        if(appendStdBuffer) strcat(buffer, stdBuffer);
    }

    return r;
}

#if 0
SPO_code SPO_template(SPO_port* port)
{
    SPO_code r = SPO_OK;
    
    if(port)
    {
        port->error = 0;
        
        r = -1;

        port->code = r;
    }
    else r = SPOE_NULL;
    
    return r;
}
#endif





#ifdef OMW_PLAT_UNIX
speed_t getUnixBaud(int baud, int* error)
{
    int e = 0;
    speed_t r;

    switch (baud)
    {
        case 0:
            r = B0;
            break;

        case 50:
            r = B50;
            break;

        case 75:
            r = B75;
            break;

        case 110:
            r = B110;
            break;

        case 134:
            r = B134;
            break;

        case 150:
            r = B150;
            break;

        case 200:
            r = B200;
            break;

        case 300:
            r = B300;
            break;

        case 600:
            r = B600;
            break;

        case 1200:
            r = B1200;
            break;

        case 1800:
            r = B1800;
            break;

        case 2400:
            r = B2400;
            break;

        case 4800:
            r = B4800;
            break;

        case 9600:
            r = B9600;
            break;

        case 19200:
            r = B19200;
            break;

        case 38400:
            r = B38400;
            break;

        case 57600:
            r = B57600;
            break;

        case 115200:
            r = B115200;
            break;

        case 230400:
            r = B230400;
            break;

        case 460800:
            r = B460800;
            break;

        case 500000:
            r = B500000;
            break;

        case 576000:
            r = B576000;
            break;

        case 921600:
            r = B921600;
            break;

        case 1000000:
            r = B1000000;
            break;

        case 1152000:
            r = B1152000;
            break;

        case 1500000:
            r = B1500000;
            break;

        case 2000000:
            r = B2000000;
            break;

        case 2500000:
            r = B2500000;
            break;

        case 3000000:
            r = B3000000;
            break;

        case 3500000:
            r = B3500000;
            break;

        case 4000000:
            r = B4000000;
            break;

        default:
            e = 1;
            break;
    };

    if(error) *error = e;

    return r;
}
#else
int isValidBaud(int baud)
{
    int r = 0;

    if(baud == 50       ||
        baud == 75      ||
        baud == 110     ||
        baud == 134     ||
        baud == 150     ||
        baud == 200     ||
        baud == 300     ||
        baud == 600     ||
        baud == 1200    ||
        baud == 1800    ||
        baud == 2400    ||
        baud == 4800    ||
        baud == 9600    ||
        baud == 19200   ||
        baud == 38400   ||
        baud == 57600   ||
        baud == 115200  ||
        baud == 230400  ||
        baud == 460800  ||
        baud == 500000  ||
        baud == 576000  ||
        baud == 921600  ||
        baud == 1000000 ||
        baud == 1152000 ||
        baud == 1500000 ||
        baud == 2000000 ||
        baud == 2500000 ||
        baud == 3000000 ||
        baud == 3500000 ||
#ifdef OMW_PLAT_UNIX // actually not needed here, but prevents copy pase errors
        baud == 0 ||
        baud == 4000000)
#else
        baud == 4000000)
#endif
    {
        r = 1;
    }

    return r;
}
#endif
