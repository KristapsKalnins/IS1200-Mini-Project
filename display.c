#include <stdint.h>
#include <pic32mx.h>

#define DISPLAY_VDD PORTFbits.RF4
#define DISPLAY_VLED PORTFbits.RD2
#define DISPLAY_COMMAND_DATA PORTbits.RD1
#define DISPLAY_RESET   PORTbits.RD9

#define DISPLAY_VDD_PORT PORTF
#define DISPLAY_VDD_MASK 0x10
#define DISPLAY_VLED_PORT PORTD
#define DISPLAT_VLED_MASK 0x4
#define DISPLAY_COMMAND_DATA_PORT PORTD
#define DISPLAY_COMMAND_DATA_MASK 0x2
#define DISPLAY_RESET_PORT PORTD
#define DISPLAY_RESET_MASK 0x200

void delay(int c){
    for(int i = c; i > 0; i--);
}

uint8_t spi_send_recieve(uint8_t data){
    while(!(SPI1STAT & 0x08));
    SPI1BUF = data;
    while(!(SPI1STAT & 0x01));
    return (SPI1BUF);
}

void spi_initialize(){
/* Set up peripheral bus clock COPIED*/
    OSCCON &= ~0x180000;
    OSCCON |= 0x080000;

/* PIC to display port setup */
    PORTD = 0x206;
    PORTF = 0x200;
    ODCD = 0x0;
    ODCF = 0x0;
    TRISDCLR = 0x206;
    TRISFCLR = 0x200;

/* Set SPI to master*/
    SPI1CON = 0;
    SPI1BRG = 4;

/* Clearing SPIROV (Overflow flag bit)*/
    SPI1CON &= ~0x40;

/* CKP = 1 MSTEN = 1(Selecting clock edge and turning on master mode)*/
    SPI1CON |= 0x60;

/* Turn on SPI bus */
    SPI2CONSET = 0x8000;
}

void display_wake(){
    DISPLAY_COMMAND_DATA_PORT &= ~DISPLAY_COMMAND_DATA_MASK;
    delay(10);
    DISPLAY_VDD_PORT &= ~DISPLAY_VDD_MASK;
    delay(1000000);

    //Display off
    spi_send_recieve(0x28);


}