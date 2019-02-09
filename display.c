#include <stdint.h>
#include <pic32mx.h>
#include <ili9341.h>

#define DISPLAY_VDD PORTFbits.RF4
#define DISPLAY_VLED PORTFbits.RD2
#define DISPLAY_COMMAND_DATA PORTbits.RD1
#define DISPLAY_RESET   PORTbits.RD9
#define DISPLAY_SELECT PORTbits.RF5

#define DISPLAY_VDD_PORT PORTF
#define DISPLAY_VDD_MASK 0x10
#define DISPLAY_VLED_PORT PORTD
#define DISPLAT_VLED_MASK 0x4
#define DISPLAY_COMMAND_DATA_PORT PORTD
#define DISPLAY_COMMAND_DATA_MASK 0x2
#define DISPLAY_RESET_PORT PORTD
#define DISPLAY_RESET_MASK 0x200
#define DISPLAY_SELECT_PORT PORTF
#define DISPLAY_SELECT_MASK 0x20

volatile uint16_t LCD_W=ILI9341_TFTWIDTH;
volatile uint16_t LCD_H=ILI9341_TFTHEIGHT;

void delay_us(int c){
    int con = c * 80;
    for(int i = con; i > 0; i--);
}

void spi_initialize(){
/* Set up peripheral bus clock COPIED*/
    OSCCON &= ~0x180000;
    OSCCON |= 0x080000;

/* PIC to display port setup */
    PORTD = 0x206;
    PORTF = 0x210;
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


void spi_send_recieve(unsigned char data){
    while(!(SPI1STAT & 0x08));
    SPI1BUF = data;
    while(!(SPI1STAT & 0x01));
}

void write_cmd_8(uint8_t com){
    DISPLAY_COMMAND_DATA_PORT &= ~DISPLAY_COMMAND_DATA_MASK;
    DISPLAY_SELECT_PORT &= ~DISPLAY_SELECT_PORT;
    delay_us(5);
    spi_send_recieve(com);
    DISPLAY_SELECT_PORT |= DISPLAY_SELECT_PORT;
}

void write_data_8(uint8_t data)//data write
{
	DISPLAY_COMMAND_DATA_PORT |= DISPLAY_COMMAND_DATA_MASK;//set dc high for data
	delay_us(1);//delay
	DISPLAY_SELECT_PORT &= ~DISPLAY_SELECT_PORT;//set cs low for operation
	spi_send_recieve(data);
	DISPLAY_SELECT_PORT |= DISPLAY_SELECT_PORT;
}

void ili9341_setaddress(uint16_t x1,uint16_t y1,uint16_t x2,uint16_t y2)//set coordinate for print or other function
{
	write_cmd_8(0x2A);
	write_data_8(x1>>8);
	write_data_8(x1);
	write_data_8(x2>>8);
	write_data_8(x2);

	write_cmd_8(0x2B);
	write_data_8(y1>>8);
	write_data_8(y1);
	write_data_8(y2);
	write_data_8(y2);

	write_cmd_8(0x2C);//meory write
}







