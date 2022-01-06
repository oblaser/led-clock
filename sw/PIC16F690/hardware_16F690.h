/* 
 * Author: Oliver Blaser
 *
 * Date: 02.11.2015
 * Date e:  03.12.2020
 * 
 */

#ifndef HARDWARE_16F690_H
#define	HARDWARE_16F690_H

#include <pic16f690.h>
#include <xc.h>

#include "EUSART.h"

//------------------------------------------------------------------------------
// Defines (Projekt)
//------------------------------------------------------------------------------

#define Taster      PORTBbits.RB6

#define ANZ_DATA    PORTCbits.RC2
#define ANZ_CLK     PORTCbits.RC1
#define ANZ__EN     PORTCbits.RC0
#define ANZ_DP      PORTCbits.RC6

#define TCO         PORTBbits.RB4

#define THRESHOLD_TASTER    100

#define INP_T       0x01
#define INP_DCF     0x02

#define S_IDLE      1
#define S_SYNC      2
#define S_SET       3
#define S_DP        4
#define S_PRINT     5

//------------------------------------------------------------------------------
// Variablen (global)
//------------------------------------------------------------------------------

// inputs
unsigned char inp;
unsigned char inp_alt;
unsigned char inp_neu;
unsigned char inp_pos;
unsigned char inp_neg;

unsigned char inp_cnt_t;

// timers

// Anzeige
unsigned char print_data[4];    // 0: hz / 1: he / 2: mz / 3: me

// sync

//------------------------------------------------------------------------------
// Funktionen
//------------------------------------------------------------------------------

void HW_Init();

void GIE_set();

void SW_Init();

void delay_ms(unsigned int f_time);

void MC14499(unsigned char d4, unsigned char d3, unsigned char d2,
                                unsigned char d1, unsigned char dp);

void print();

void uhrzeit_zaehler();

void input_handler();

void sync();

void take_time();

void send_time();

#endif	/* HARDWARE_16F690_H */

