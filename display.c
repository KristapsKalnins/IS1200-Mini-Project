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

void delay_ms(int c){
    int con = c * 80000 ;
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

void setAddress(uint16_t x1,uint16_t y1,uint16_t x2,uint16_t y2)//set coordinate for print or other function
{
	write_cmd_8(0x2A);     //SET COLUMN ADDRESS
	write_data_8(x1>>8);
	write_data_8(x1);
	write_data_8(x2>>8);
	write_data_8(x2);

	write_cmd_8(0x2B);     //SET ROW ADDRESS
	write_data_8(y1>>8);
	write_data_8(y1);
	write_data_8(y2);
	write_data_8(y2);

	write_cmd_8(0x2C);//meory write
}

void hard_reset(void)//hard reset display
{
	DISPLAY_RESET_PORT |=  DISPLAY_RESET_MASK;//pull high if low previously
	delay_ms(200);
	DISPLAY_RESET_PORT &= ~DISPLAY_RESET_MASK;//low for reset
	delay_ms(200);
	DISPLAY_RESET_PORT |=  DISPLAY_RESET_MASK;//again pull high for normal operation
	delay_ms(200);
}

void display_init(void)//set up display using predefined command sequence
{
	spi_initialize();
	hard_reset();
	write_cmd_8(0x01);//soft reset
	delay_ms(1000);

	//power control A
	write_cmd_8(0xCB);
	write_data_8(0x39);
	write_data_8(0x2C);
	write_data_8(0x00);
	write_data_8(0x34);
	write_data_8(0x02);

	//power control B
	write_cmd_8(0xCF);
	write_data_8(0x00);
	write_data_8(0xC1);
	write_data_8(0x30);

	//driver timing control A
	write_cmd_8(0xE8);
	write_data_8(0x85);
	write_data_8(0x00);
	write_data_8(0x78);

	//driver timing control B
	write_cmd_8(0xEA);
	write_data_8(0x00);
	write_data_8(0x00);

	//power on sequence control
	write_cmd_8(0xED);
	write_data_8(0x64);
	write_data_8(0x03);
	write_data_8(0x12);
	write_data_8(0x81);

	//pump ratio control
	write_cmd_8(0xF7);
	write_data_8(0x20);

	//power control,VRH[5:0]
	write_cmd_8(0xC0);
	write_data_8(0x23);

	//Power control,SAP[2:0];BT[3:0]
	write_cmd_8(0xC1);
	write_data_8(0x10);

	//vcm control
	write_cmd_8(0xC5);
	write_data_8(0x3E);
	write_data_8(0x28);

	//vcm control 2
	write_cmd_8(0xC7);
	write_data_8(0x86);

	//memory access control
	write_cmd_8(0x36);
	write_data_8(0x48);

	//pixel format
	write_cmd_8(0x3A);
	write_data_8(0x55);

	//frameration control,normal mode full colours
	write_cmd_8(0xB1);
	write_data_8(0x00);
	write_data_8(0x18);

	//display function control
	write_cmd_8(0xB6);
	write_data_8(0x08);
	write_data_8(0x82);
	write_data_8(0x27);

	//3gamma function disable
	write_cmd_8(0xF2);
	write_data_8(0x00);

	//gamma curve selected
	write_cmd_8(0x26);
	write_data_8(0x01);

	//set positive gamma correction
	write_cmd_8(0xE0);
	write_data_8(0x0F);
	write_data_8(0x31);
	write_data_8(0x2B);
	write_data_8(0x0C);
	write_data_8(0x0E);
	write_data_8(0x08);
	write_data_8(0x4E);
	write_data_8(0xF1);
	write_data_8(0x37);
	write_data_8(0x07);
	write_data_8(0x10);
	write_data_8(0x03);
	write_data_8(0x0E);
	write_data_8(0x09);
	write_data_8(0x00);

	//set negative gamma correction
	write_cmd_8(0xE1);
	write_data_8(0x00);
	write_data_8(0x0E);
	write_data_8(0x14);
	write_data_8(0x03);
	write_data_8(0x11);
	write_data_8(0x07);
	write_data_8(0x31);
	write_data_8(0xC1);
	write_data_8(0x48);
	write_data_8(0x08);
	write_data_8(0x0F);
	write_data_8(0x0C);
	write_data_8(0x31);
	write_data_8(0x36);
	write_data_8(0x0F);

	//exit sleep
	write_cmd_8(0x11);
	delay_ms(120);
	//display on
	write_cmd_8(0x29);
}

//set color for drawing pixels
void setPixelColor(uint16_t color){
    write_data_8(color >> 8);
    write_data_8(color);
}

//clear screen and fill with color
void clear(uint16_t color){
    uint16_t i,j;
    setAddress(0,0, LCD_W-1, LCD_H-1);

    for(i - 0; i<LCD_W; i++){
        for(j = 0; j<LCD_H; j++){
            setPixelColor(color);
        }
    }
}


