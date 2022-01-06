/* 
 * Author: Oliver Blaser
 *
 * Date:    02.11.2015
 * Date e:  03.12.2020
 * 
 */

#include "hardware_16F690.h"
#include "EUSART.h"

#define SW_VERSION  "3.0.0"

int refreshDisp = 0;
int dispOn = 1;
int colonOn = 1;

int main()
{
    HW_Init();
    SW_Init();
    EUSART_init();
    GIE_set();
    
    ANZ_DP = 0;
    
    refreshDisp = 1;
    
    while(1)
    {
        input_handler();
        
        // Display ein-/ausschalten mit Taster
        if(inp_pos & INP_T)
        {
            if(dispOn) dispOn = 0;
            else dispOn = 1;
            
            refreshDisp = 1;
        }
        
        // UART protocol:
        // AD-xxxxx\n   writes the display buffers (four digits and colon)
        // ADS-x\n      display state (on/off)
        //
        if(RX_READY)
        {
            int rxError = 0;
            
            RX_READY = 0;
            
            if((rx_data[0] == 'A') &&
                    (rx_data[1] == 'D') &&
                    (rx_data[2] == '-') &&
                    (rx_data[8] == RX_C_END_STRING))
            {
                for(int i = 0; i < 4; ++i)
                {
                    unsigned char db = rx_data[3 + i];
                    
                    if((db >= 0x30) && (db <= 0x39)) print_data[i] = db - 0x30;
                    else if((db >= 'A') && (db <= 'F')) print_data[i] = db - 55;
                    else if(db == 0x20) print_data[i] = 0x0F;
                    else rxError = 1;
                }
                
                if(rx_data[7] == 0x30) colonOn = 0;
                else if(rx_data[7] == 0x31) colonOn = 1;
                else rxError = 1;
            }
            else if((rx_data[0] == 'A') &&
                    (rx_data[1] == 'D') &&
                    (rx_data[2] == 'S') &&
                    (rx_data[3] == '-') &&
                    (rx_data[5] == RX_C_END_STRING))
            {
                if(rx_data[4] == 0x30) dispOn = 0;
                else if(rx_data[4] == 0x31) dispOn = 1;
                else rxError = 1;
            }
            else rxError = 1;
            
            if(!rxError)
            {
                refreshDisp = 1;
                //while(UART_send("A-OK\n"));
            }
            //else while(UART_send("A-ERROR\n"));
        }
        
        if(refreshDisp)
        {
            refreshDisp = 0;
            
            if(dispOn)
            {
                MC14499(print_data[0] , print_data[1] , print_data[2] , print_data[3] , 0);

                if(colonOn) ANZ_DP = 1;
                else ANZ_DP = 0;
            }
            else
            {
                MC14499(15, 15, 15, 15 , 0);
                ANZ_DP = 0;
            }
        }
    }
    
    return 0;
}
