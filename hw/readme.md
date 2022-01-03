# LED Clock Hardware

## PCB #1 Display
A MC14499 ([datasheet](http://pdf.datasheetcatalog.com/datasheet/motorola/MC14499DW.pdf)) drives the LEDs.

#### Data Cable Pinout
```
1 GND
2 Vcc
3 !EN
4 CLK
5 DATA
6 Colon LEDs
```

## PCB #2 Main Unit
#### PIC16F690
|  | Pin | Pin |  |
|---:|:---|---:|:---|
| 5V | 1 - Vdd | GND - 20 | GND |
| - | 2 | RA0/ICSPDAT - 19 | PROG_DAT |
| - | 3 | RA1/ICSPCLK - 18 | PROG_CLK |
| PROG_NRST | 4 - RA3/!MCLR | 17 | - |
| - | 5 | RC0 - 16 | Display !Enable |
| - | 6 | RC1 - 15 | Soft SPI CLK |
| - | 7 | RC2 - 14 | Soft SPI DAT |
| Colon LEDs | 8 - RC6 | 13 | - |
| - | 9 | RB5/RX - 12 | UART VoCode |
| UART VoCore | 10 - RB7/TX | RB6 - 11 | Push Button |


## PCB #3 VoCore2
UART1 is connected to the μC
