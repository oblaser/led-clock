/* 
 * Author: Oliver Blaser
 *
 * Date: 02.11.2015
 * Date e:  03.12.2020
 * 
 */

#include "hardware_16F690.h"

//------------------------------------------------------------------------------
// Config
//------------------------------------------------------------------------------

#pragma config FOSC = INTRCIO   // Oscillator Selection bits (INTOSCIO oscillator: I/O function on RA4/OSC2/CLKOUT pin, I/O function on RA5/OSC1/CLKIN)
#pragma config WDTE = OFF       // Watchdog Timer Enable bit (WDT disabled)
#pragma config PWRTE = OFF      // Power-up Timer Enable bit (PWRT disabled)
#pragma config MCLRE = ON       // MCLR Pin Function Select bit (MCLR pin function is MCLR)
#pragma config CP = OFF         // Code Protection bit (Program memory code protection is disabled)
#pragma config CPD = OFF        // Data Code Protection bit (Data memory code protection is disabled)
#pragma config BOREN = OFF      // Brown-out Reset Selection bits (BOR disabled)
#pragma config IESO = OFF       // Internal External Switchover bit (Internal External Switchover mode is disabled)
#pragma config FCMEN = OFF      // Fail-Safe Clock Monitor Enabled bit (Fail-Safe Clock Monitor is disabled)

//------------------------------------------------------------------------------
// Funktionen
//------------------------------------------------------------------------------

void HW_Init() {
    
    //--------------------------------
    // Oscillator
    //--------------------------------
    
    OSCCON  = 0b01110001;   // 8MHz
    OSCTUNE = 0b00000000;
    
    //--------------------------------
    // PORTs
    //--------------------------------
    
    TRISA = 0b11111111;     // 0:Out 1:In
    PORTA = 0b00000000;
    WPUA  = 0b00000000;     // PullUp's: 0:disabled 1:enabled
    IOCA  = 0b00000000;     // Interrupt on change: 0:disabled 1:enabled
    
    TRISB = 0b11111111;     // 0:Out 1:In
    PORTB = 0b00000000;
    WPUB  = 0b00000000;     // PullUp's: 0:disabled 1:enabled
    IOCB  = 0b00000000;     // Interrupt on change: 0:disabled 1:enabled
    
    TRISC = 0b10111000;     // 0:Out 1:In
    PORTC = 0b00000000;
    
    //--------------------------------
    // Timer0
    //--------------------------------
    
    OPTION_REG = 0b01000011;    // CS = Fosc/4 PS = 1:16
    TMR0       = 0b00000000;
    
    //--------------------------------
    // Comparator
    //--------------------------------
    
    CM1CON0 = 0b00000000;   // Comp1 disabled
    CM2CON0 = 0b00000000;   // Comp2 disabled
    
    //--------------------------------
    // ADC
    //--------------------------------
    
    ANSEL  = 0b00000000;    // digital
    ANSELH = 0b00000000;    // digital
    
    ADCON0 = 0b00000000;
    ADCON1 = 0b00000000;
    
    //--------------------------------
    // Interrupt
    //--------------------------------
    
    INTCON = 0b01100000;
    PIR1   = 0b00000000;
    PIR2   = 0b00000000;
    PIE1   = 0b00000000;
    PIE2   = 0b00000000;
    
    //--------------------------------
    // EUSART
    //--------------------------------
    
    // extern file

    //--------------------------------
    // SSP
    //--------------------------------
    
    
    
    //--------------------------------
    // Watch Dog Timer
    //--------------------------------
}

void GIE_set() {
    GIE = 1;
}

void SW_Init() {
    
    
    for(int i = 0; i < 4; i++) print_data[i] = 0x0E;
    
    // inputs
    inp = 0;
    inp_neu = 0;
    inp_pos = 0;
    inp_neg = 0;
    inp_cnt_t = THRESHOLD_TASTER/2;
    
    inp_alt = 0;
    if(Taster) inp_alt |= INP_T;
    if(TCO) inp_alt |= INP_DCF;
    
}

void MC14499(unsigned char d4, unsigned char d3, unsigned char d2,
                                unsigned char d1, unsigned char dp) {

    //GIE = 0;
    
    unsigned int us_tmr;
    
    unsigned char data[5];

    data[0] = d4;
    data[1] = d3;
    data[2] = d2;
    data[3] = d1;
    data[4] = dp;

    us_tmr = 0;

    ANZ__EN = 0;
    us_tmr++;   // tE LEAD

    // Decimal Points
    for(int i=0; i<4; i++) {
        ANZ_CLK = 1;

        if(data[4] & 0x01) {
            ANZ_DATA = 1;
        } else {
            ANZ_DATA = 0;
        }
        data[4] >>= 1;
        us_tmr++;   // tDSUP

        ANZ_CLK = 0;
        us_tmr++;   // tCH & tDH
    }

    // Digit 1-4
    for(int i=3; i>=0; i--) {
        for(int j=0; j<4; j++) {
            ANZ_CLK = 1;

            data[i] <<= 1;
            if(data[i] & 0x10) {
                ANZ_DATA = 1;
            } else {
                ANZ_DATA = 0;
            }
            us_tmr++;   // tDSUP

            ANZ_CLK = 0;
            us_tmr++;   // tCH & tDH
        }
    }

    us_tmr++;   // tE LAG
    ANZ__EN = 1;
    
    //GIE = 1;
}

/*void print() {
    
    if(disp_on) {
    
        if(print_data[0] == 0) {
            MC14499(15 , print_data[1] , print_data[2] , print_data[3] , 0);
        }
        else {
            MC14499(print_data[0] , print_data[1] , print_data[2] , print_data[3] , 0);
        }
    }
    
    else MC14499(15, 15, 15, 15, 0);
}

void uhrzeit_zaehler() {
    if(print_data[3] >= 9) {    // wenn m_e ueberlaufen

                print_data[3] = 0;
                if(print_data[2] >= 5) {    // wenn m_z ueberlaufen

                    print_data[2] = 0;

                    if(print_data[0] < 2) {


                        if(print_data[1] >= 9) {    // wenn h_e ueberlaufen

                            print_data[1] = 0;
                            if(print_data[0] >= 2) {    // wenn h_z ueberlaufen

                                print_data[0] = 0;
                            }
                            else {
                                print_data[0]++;
                            }
                        }
                        else {
                            print_data[1]++;
                        }


                    }
                    else {

                        if(print_data[1] >= 3) {    // wenn h_e ueberlaufen

                            print_data[1] = 0;
                            if(print_data[0] >= 2) {    // wenn h_z ueberlaufen

                                print_data[0] = 0;
                            }
                            else {
                                print_data[0]++;
                            }
                        }
                        else {
                            print_data[1]++;
                        }

                    }



                }
                else {
                    print_data[2]++;
                }
            }
            else {
                print_data[3]++;
            }
}*/

void input_handler() {
    
    // entprellen
    if(Taster == 1 && inp_cnt_t < THRESHOLD_TASTER) inp_cnt_t++;
    if(Taster == 0 && inp_cnt_t > 0) inp_cnt_t--;
    
    if(inp_cnt_t >= THRESHOLD_TASTER) inp |= INP_T;
    if(inp_cnt_t <= 0) inp &= ~INP_T;
    
    // neuwerte
    inp_neu = 0;
    inp_neu |= (inp & INP_T);
    
    if(TCO) inp_neu |= INP_DCF;
    else inp_neu &= ~INP_DCF;
    
    // flanken
    inp_pos = ~inp_alt & inp_neu;
    inp_neg = inp_alt & ~inp_neu;
    
    // input werte in alt speichern
    inp_alt = inp_neu;
}

/*void sync() {
    
    unsigned int dcf_mask;
    
    unsigned char dcf_parity;
    
    // posFl von DCF
    if(inp_pos & INP_DCF) {
        
        // Dopelpunkt
        if(disp_on) ANZ_DP = 1;
        tmr_DP = 500;

        // timer um DCF signal auszuwerten
        tmr_DCF = 150;

        // timer für bit 59
        tmr_bit_detect = 0;

        // bit zähler
        bit_cnt++;
        if(bit_cnt > 60) bit_cnt = 60;  // damit er nicht bei einem überlauf 0 wird und die fehler resettet
        
        // beginn neuer minute
        if(bit_cnt_reset) {
            
            // wenn keine fehler bei letzter synchronisation, zeit übernehmen
            if(fail == 0) take_time();
            
            // resets für die neue minute
            bit_cnt = 0;
            bit_cnt_reset = 0;
            fail = 0;
        }
    }

    // Doppelpunkt nach 1/2s ausschalten
    if(tmr_DP == 0) ANZ_DP = 0;

    // DCF signal in array speichern
    if(tmr_DCF == 0) {

        switch(bit_cnt) {
            
            // anfang minute
            case 0:
                if(TCO) fail = 1;
                break;
            
            // anfang zeit
            case 20:
                if(!TCO) fail = 1;
                break;
            
            // minute einer
            case 21:
                if(TCO) dcf_data |= 0x0001;
                else dcf_data &= ~0x0001;
                break;
            
            case 22:
                if(TCO) dcf_data |= 0x0002;
                else dcf_data &= ~0x0002;
                break;
            
            case 23:
                if(TCO) dcf_data |= 0x0004;
                else dcf_data &= ~0x0004;
                break;
            
            case 24:
                if(TCO) dcf_data |= 0x0008;
                else dcf_data &= ~0x0008;
                break;
            
            // minute zehner
            case 25:
                if(TCO) dcf_data |= 0x0010;
                else dcf_data &= ~0x0010;
                break;
            
            case 26:
                if(TCO) dcf_data |= 0x0020;
                else dcf_data &= ~0x0020;
                break;
            
            case 27:
                if(TCO) dcf_data |= 0x0040;
                else dcf_data &= ~0x0040;
                break;
            
            // parität minute
            case 28:
                
                dcf_parity = 0;
                for(dcf_mask = 0x0001; dcf_mask <= 0x0040; dcf_mask <<= 1)
                    if(dcf_data & dcf_mask) dcf_parity++;
                
                // bit 28 ist 1 wenn ungerade anzahl einser von 21-27
                if(TCO && dcf_parity & 1) fail = 1;
                
                break;
            
            // stunde einer
            case 29:
                if(TCO) dcf_data |= 0x0080;
                else dcf_data &= ~0x0080;
                break;
            
            case 30:
                if(TCO) dcf_data |= 0x0100;
                else dcf_data &= ~0x0100;
                break;
            
            case 31:
                if(TCO) dcf_data |= 0x0200;
                else dcf_data &= ~0x0200;
                break;
            
            case 32:
                if(TCO) dcf_data |= 0x0400;
                else dcf_data &= ~0x0400;
                break;
            
            // stunde zehner
            case 33:
                if(TCO) dcf_data |= 0x0800;
                else dcf_data &= ~0x0800;
                break;
            
            case 34:
                if(TCO) dcf_data |= 0x1000;
                else dcf_data &= ~0x1000;
                break;
            
            // parität stunde
            case 35:
                
                dcf_parity = 0;
                for(dcf_mask = 0x0080; dcf_mask <= 0x1000; dcf_mask <<= 1)
                    if(dcf_data & dcf_mask) dcf_parity++;
                
                // bit 35 ist 1 wenn ungerade anzahl einser von 29-34
                if(TCO && dcf_parity & 1) fail = 1;
                
                break;
                
            default:
                break;
        }
        
        tmr_DCF_fail = 100;
        tmr_DCF = 2000;
    }

    // bit 59
    if(tmr_bit_detect >= 1500 && tmr_bit_detect <= 2200) bit_cnt_reset = 1;
    else bit_cnt_reset = 0;

    // fehlerhaftes signal erkennen, immer LOW
    if(tmr_bit_detect >= 2500) {
        fail = 1;
        tmr_bit_detect = 2499;
    }

    // fehlerhaftes signal erkennen, immer HIGH
    if(tmr_DCF_fail == 0) fail = 1;
}

void take_time() {
    
    print_data[0] = (dcf_data & 0x1800) >> 11;
    print_data[1] = (dcf_data & 0x0780) >> 7;
    print_data[2] = (dcf_data & 0x0070) >> 4;
    print_data[3] = dcf_data & 0x000F;
    
    tmr_1min = 60000;
}

void send_time() {
    
    unsigned char to_send[6];
    
    for(int i = 0; i < 4; i++) to_send[i] = print_data[i] + 0x30;
    
    to_send[4] = ' ';
    
    if(disp_on) to_send[5] = 0x31;
    else to_send[5] = 0x30;
    
    UART_send(to_send);
}*/

//------------------------------------------------------------------------------
// Interrupt
//------------------------------------------------------------------------------

void __interrupt() iupt() {
   
    // Timer0 every ms
    if(T0IE && T0IF) {
        
        //if(tmr_DCF_fail) tmr_DCF_fail--;
        
        TMR0 = 130;
        T0IF = 0;
    }
    
    // UART TX
    if(TXIE && TXIF) TX_interrupt();
    
    // UART RX
    if(RCIE && RCIF) RX_interrupt();
    
}