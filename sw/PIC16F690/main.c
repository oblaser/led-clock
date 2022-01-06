/* 
 * Author: Oliver Blaser
 *
 * Date: 02.11.2015
 * 
 */

#include "hardware_16F690.h"
#include "EUSART.h"

#define SW_VERSION  "2.00"

int main()
{
    HW_Init();
    SW_Init();
    EUSART_init();
    GIE_set();
    
    ANZ_DP = 0;
    print();
    
    while(1)
    {
        input_handler();
        
        // Display ein-/ausschalten mit Taster
        if(inp_pos & INP_T)
        {
            disp_on = ~disp_on;
            print_en = 1;
        }
        
        // empfangene UART daten auswerten
        if(RX_READY)
        {
            RX_READY = 0;
            
            // get time
            if(rx_data[0] == 't')
            {
                // copy time to display buffer
                print_data[0] = rx_data[1] - 0x30;
                print_data[1] = rx_data[2] - 0x30;
                print_data[2] = rx_data[4] - 0x30;
                print_data[3] = rx_data[5] - 0x30;

                // doppelpunkt syncen
                ANZ_DP = 1;
                tmr_DP = 500;

                print_en = 1;
            }
            
            // get display state
            else if(rx_data[0] == 'd')
            {
                if(rx_data[1] == '1') disp_on = 1;
                else disp_on = 0;
                
                print_en = 1;
            }
            
            // protocoll error
            else
            {
                UART_send("Protocoll Error\n");
            }
        }
        
        // Doppelpunkt blink
        if(disp_on)
        {
            if(tmr_DP == 0 && print_data[0] != 0x0E)
            {
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
