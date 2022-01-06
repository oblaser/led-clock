/* 
 * Author: Oliver Blaser
 *
 * Date: 02.11.2015
 * 
 */

#include "hardware_16F690.h"

#define TIME_OFF_0  0000

int main() {
    
    unsigned int time = 0;
    unsigned char day = 0;
    
    unsigned char rx_index;
    
    unsigned int time_off = 2200; //[7] = {
//        2200,   /* 0 Mo */
//        2200,   /* 1 Di */
//        2200,   /* 2 Mi */
//        2200,   /* 3 Do */
//        2300,   /* 4 Fr */
//        2300,   /* 5 Sa */
//        2200};  /* 6 So */
    
    unsigned int time_on[7] = {
        550,   /* 0 Mo */
        550,   /* 1 Di */
        550,   /* 2 Mi */
        550,   /* 3 Do */
        550,   /* 4 Fr */
        955,   /* 5 Sa */
        955};  /* 6 So */
    
    HW_Init();
    SW_Init();
    EUSART_init();
    GIE_set();
    
    ANZ_DP = 0;
    print();
    
    while(1) {
        
        input_handler();
        
        // Display ein-/ausschalten mit Taster
        if(inp_pos & INP_T) {
            disp_on = ~disp_on;
            print_en = 1;
        }
        
        // empfangene UART daten auswerten
        if(inp_pos & INP_RX) {
            
            switch(rx_data[0]) {
                
                // Uhrzeit uebernehmen
                case 't':
                    
                    // zeit uebernehmen
                    print_data[0] = rx_data[1] - 0x30;
                    print_data[1] = rx_data[2] - 0x30;
                    print_data[2] = rx_data[4] - 0x30;
                    print_data[3] = rx_data[5] - 0x30;
                    
                    time = (rx_data[1] - 0x30) * 1000 + 
                           (rx_data[2] - 0x30) * 100 + 
                           (rx_data[4] - 0x30) * 10 + 
                           (rx_data[5] - 0x30) * 1;
                    
                    day = rx_data[10] - 0x30;
                    
                    // doppelpunkt syncen
                    ANZ_DP = 1;
                    tmr_DP = 500;
                    
                    // display einschalten
                    if(time == time_on[day]) disp_on = 1;
                    
                    // display ausschalten
                    if(time == time_off ||
                       ((time > time_off || time < time_on[day]) && time % 10 == 0) || 
                       time == TIME_OFF_0) {
                        disp_on = 0;
                    }
                    
                    print_en = 1;
                   
                    break;
                    
                /*// einschaltzeiten uebernehmen
                case 'n':
                    
                    // aktuelle zeiten loeschen und neue speichern
                    rx_index = 1;
                    
                    for(int i = 0; i < 7; i++)
                    {
                        time_on[i] = 0;
                        
                        time_on[i] += (rx_data[rx_index] - 0x30) * 1000;
                        rx_index++;
                        
                        time_on[i] += (rx_data[rx_index] - 0x30) * 100;
                        rx_index++;
                        
                        time_on[i] += (rx_data[rx_index] - 0x30) * 10;
                        rx_index++;
                        
                        time_on[i] += (rx_data[rx_index] - 0x30) * 1;
                        rx_index += 2;
                    }
                    
                    break;*/
                    
                default:
                    break;
            }
        }
        
        // Doppelpunkt blink
        if(disp_on) {
            if(tmr_DP == 0 && print_data[0] != 0x0E) {
                ANZ_DP = ~ANZ_DP;
                tmr_DP = 500;
            }
        }
        else ANZ_DP = 0;
        
        // print
        if(print_en) {
            print();
            print_en = 0;
        }
    }
    
    return 0;
}

/*
    while(1) {
        
        input_handler();
        
        sync();
        
        if(tmr_1min == 0) {
            
            uhrzeit_zaehler();
            
            send_time();
            
            print_en = 1;
            tmr_1min = 60000;
        }
        
        // Display ein-/ausschalten
        if(inp_pos & INP_T) {
            disp_on = ~disp_on;
            print_en = 1;
        }
        
        if(RX_READY) disp_on = rx_data[0] - 0x30;
        
        if(print_en) {
                print();
                print_en = 0;
            }
        
        
    }
    
    return 0;
}
*/
