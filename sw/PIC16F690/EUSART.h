/* 
 * Author: Oliver Blaser
 *
 * Date: 29.02.2016
 * 
 * Enthält alle Funktionen die notwendig
 * sind, um ein EUSART Modul zu benüzen.
 * 
 */

/*
 * TODO:
 * 
 * interrupt funktion erweritern:
// UART TX
if(TXIE && TXIF) TX_interrupt();

// UART RX
if(RCIE && RCIF) RX_interrupt();
 * 
 * RX/TX pins als digitalen eingang sezten
 * 
 * EUSART_init(); vor GIE_en();
 * 
 */

#ifndef EUSART_H
#define	EUSART_H

#include <xc.h>

#define RX_LENGTH       40
#define TX_LENGTH       20

#define RX_C_END_STRING   0x0D    // \r CR
#define TX_C_END_STRING   0x0D    // \r CR

#define RX_READY    rx_idle

unsigned char rx_data[RX_LENGTH];
unsigned char tx_data[TX_LENGTH];

unsigned int cnt_rx_byte;
unsigned int cnt_tx_byte;

bit rx_idle;

/**
 * Initialisiert und konfiguriert das EUSART Modul
 */
void EUSART_init();

/**
 * Startet das Senden eines Strings.
 * Gibt 1 zurück, wenn noch ein string gesendet wird.
 * Sonst 0.
 * @param f_send
 * @return 
 */
int UART_send(unsigned char f_send[]);

/**
 * TX interrupt Routine
 */
void TX_interrupt();

/**
 * RX interrupt Routine
 */
void RX_interrupt();

/**
 * Vergleicht zwei Strings auf die ersten "f_num_of_elements" Zeichen.
 * Wenn sie gleich sind wird 1 zurückgegeben, sonst 0.
 * @param f_str_a
 * @param f_str_b
 * @param f_num_of_elements
 * @return 
 */
int string_comp(unsigned char f_str_a[], unsigned char f_str_b[],
        unsigned int f_num_of_elements);

#endif	/* EUSART_H */
