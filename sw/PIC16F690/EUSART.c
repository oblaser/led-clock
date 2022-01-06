/* 
 * Author: Oliver Blaser
 *
 * Date: 29.02.2016
 * 
 */

#include "EUSART.h"

void EUSART_init() {
    
    //TX & RX must be set as Input first
    //Set Baudrate 9600 with an Error of 0.16%
    SPBRGH = 0;
    SPBRG = 51;
    BRGH = 1;
    BRG16 = 0;

    //Setup EUSART as Asynchronous Receiver - Transmiter 
    SYNC = 0;
    SPEN = 1;
    TXEN = 1;
    RCIE = 1;
    CREN = 1;
    
    PEIE = 1;
    
    for(int i = 0; i < RX_LENGTH; i++) rx_data[i] = RX_C_END_STRING;
    for(int i = 0; i < TX_LENGTH; i++) tx_data[i] = TX_C_END_STRING;
    
    cnt_rx_byte = 0;
    cnt_tx_byte = 0;
    
    rx_idle = 1;
}

int UART_send(unsigned char f_send[]) {
    
    unsigned int f_cnt_char;
    
    if(TXIE) return 1;
    
    else {
    
        f_cnt_char = 0;
        while(f_send[f_cnt_char] != '\0') f_cnt_char++; 
        
        for(int i = 0; i < TX_LENGTH; i++) tx_data[i] = TX_C_END_STRING;
        
        for(int i = 0; i < f_cnt_char; i++) {
            tx_data[i] = TX_C_END_STRING;
            tx_data[i] = f_send[i];
        }

        cnt_tx_byte = 0;
        TXIE = 1;
    }
    
    return 0;
}

void TX_interrupt() {
    
    TXREG = tx_data[cnt_tx_byte];

    if(tx_data[cnt_tx_byte] == TX_C_END_STRING) {
        TXIE = 0;   // TXIF is automatically setted by EUSART module
                    // and allways 1 if the TX reg is emty
    }
    
    cnt_tx_byte++;
}

void RX_interrupt() {
    
    rx_idle = 0;
        
    rx_data[cnt_rx_byte] = RCREG;

    if(cnt_rx_byte >= RX_LENGTH || rx_data[cnt_rx_byte] == RX_C_END_STRING) {
        rx_idle = 1;
        cnt_rx_byte = 0;
    }
    else cnt_rx_byte++;

    RCIF = 0;
}

int string_comp(unsigned char f_str_a[], unsigned char f_str_b[],
        unsigned int f_num_of_elements) {
    
    for(int i = 0; i < f_num_of_elements; i++) {
        if(f_str_a[i] != f_str_b[i]) return 0;
    }
    
    return 1;
}
