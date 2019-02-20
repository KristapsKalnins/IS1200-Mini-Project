#include <stdint.h>
#include <pic32mx.h>
#include "ili9341.h"

#define DISPLAY_VDD PORTFbits.RF1
#define DISPLAY_VLED PORTFbits.RD2
#define DISPLAY_COMMAND_DATA PORTbits.RD1
#define DISPLAY_RESET   PORTbits.RD9
#define DISPLAY_SELECT PORTbits.RD0

#define DISPLAY_VDD_PORT PORTF
#define DISPLAY_VDD_MASK 0x2
#define DISPLAY_VLED_PORT PORTD
#define DISPLAT_VLED_MASK 0x4
#define DISPLAY_COMMAND_DATA_PORT PORTD
#define DISPLAY_COMMAND_DATA_MASK 0x2
#define DISPLAY_RESET_PORT PORTD
#define DISPLAY_RESET_MASK 0x200
#define DISPLAY_SELECT_PORT PORTD
#define DISPLAY_SELECT_MASK 0x1

void spi_initialize(){
/* Set up peripheral bus clock COPIED*/
    SYSKEY = 0xAA996655;  /* Unlock OSCCON, step 1 */
	SYSKEY = 0x556699AA;  /* Unlock OSCCON, step 2 */
	while(OSCCON & (1 << 21)); /* Wait until PBDIV ready */
	OSCCON |= 0x100000; /* set PBDIV bit <0,1> */
	while(OSCCON & (1 << 21));  /* Wait until PBDIV ready */
	SYSKEY = 0x0; /* Lock OSCCON */


/* PIC to display port setup */
    PORTD = 0x207;
    PORTF = 0x2;
    ODCD = 0x0;
    ODCF = 0x0;
    TRISDCLR = 0x207;
    TRISFCLR = 0x2;


	/* Set up SPI as master */
	SPI2CON = 0;
	SPI2BRGSET = 0;
	/* SPI2STAT bit SPIROV = 0; */
	SPI2STATCLR = 0x40;
	/* SPI2CON bit CKP = 1; */
       SPI2CONSET = 0x40;
	/* SPI2CON bit MSTEN = 1; */
	SPI2CONSET = 0x20;
	/* SPI2CON bit ON = 1; */
	SPI2CONSET = 0x8400;
}

inline void Mode16(void){  // configure SPI1 for 16-bit mode
    SPI2CONSET = 0x400;
}

inline void Mode8(void){  // configure SPI1 for 8-bit mode
    SPI2CONCLR = 0x400;
}

uint8_t spi_send_8(uint8_t data){
	Mode8();
    while(!(SPI2STAT & 0x08));
    SPI2BUF = data;
    while(!(SPI2STAT & 1));
	return SPI2BUF;
	Mode16();
}

uint16_t spi_send(uint16_t data){
 	while(!(SPI2STAT & 0x08));
    SPI2BUF = data;
    while(!(SPI2STAT & 1));
	return SPI2BUF;
}

void write_cmd_8(uint8_t com){
    DISPLAY_COMMAND_DATA_PORT &= ~DISPLAY_COMMAND_DATA_MASK;
    DISPLAY_SELECT_PORT &= ~DISPLAY_SELECT_MASK;
 	spi_send_8(com);
    DISPLAY_SELECT_PORT |= DISPLAY_SELECT_MASK;
}

void write_data_8(uint8_t data)//data write
{
	DISPLAY_COMMAND_DATA_PORT |= DISPLAY_COMMAND_DATA_MASK;//set dc high for data
	DISPLAY_SELECT_PORT &= ~DISPLAY_SELECT_MASK;//set cs low for operation
 	spi_send_8(data);
	DISPLAY_SELECT_PORT |= DISPLAY_SELECT_MASK;
}

void write_cmd_16(uint16_t com){
    DISPLAY_COMMAND_DATA_PORT &= ~DISPLAY_COMMAND_DATA_MASK;
    DISPLAY_SELECT_PORT &= ~DISPLAY_SELECT_MASK;
 	spi_send(com);
    DISPLAY_SELECT_PORT |= DISPLAY_SELECT_MASK;
}

void write_data_16(uint16_t data)//data write
{
	DISPLAY_COMMAND_DATA_PORT |= DISPLAY_COMMAND_DATA_MASK;//set dc high for data
	DISPLAY_SELECT_PORT &= ~DISPLAY_SELECT_MASK;//set cs low for operation
 	spi_send(data);
	DISPLAY_SELECT_PORT |= DISPLAY_SELECT_MASK;
}

uint16_t spi_multi(uint16_t* dataOut, uint16_t count){
    uint16_t i;
	while(!(SPI2STAT & 0x08));
	for(i = 0; i < count; i++){
		SPI2BUF = dataOut[i];
		while(!(SPI2STAT & 1));
		return SPI2BUF;
	}
}

// uint16_t spi_dma(uint16_t* dataOut, uint16_t count){
// uint16_t i;
// uint32_t dmaflag;
// dmaflag = DMASTAT;
// uint16_t dummy[] = {0x08f0};







// }
void write_data_multi(uint16_t* out,uint16_t count)
{
	DISPLAY_COMMAND_DATA_PORT |= DISPLAY_COMMAND_DATA_MASK;//set dc high for data
	DISPLAY_SELECT_PORT &= ~DISPLAY_SELECT_MASK;//set cs low for operation
 	spi_multi(out, count);
	DISPLAY_SELECT_PORT |= DISPLAY_SELECT_MASK;
}





