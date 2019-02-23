#include<stdio.h>
#include <stdint.h>
#include <pic32mx.h>
#include <string.h>
#include "ili9341.h"
#include "spi.h"
#include "assets.h"

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

// Bloku un raketes garumi un biezumi
#define BLOCK_THICC 15
#define BLOCK_LEN 30
#define PADDLE_LENGTH 50
#define PADDLE_THICKNESS 10

#define ILI9341_COLUMN_ADDR			0x2A
#define ILI9341_PAGE_ADDR			0x2B
#define ILI9341_GRAM				0x2C
#define ILI9341_MAC					0x36
#define ILI9341_WRITE_CONTINUE		0x3C 
#define ILI9341_WDB					0x51
#define ILI9341_WCD					0x53
#define ILI9341_RGB_INTERFACE		0xB0
#define ILI9341_FRC					0xB1
#define ILI9341_BPC					0xB5
#define ILI9341_DFC					0xB6

#define pgm_read_byte(addr) (*(const uint8_t*)(addr))

volatile uint16_t LCD_W=240;
volatile uint16_t LCD_H=320;
uint8_t textsize, textwrap;
uint16_t Xcursor, Ycursor;
uint32_t textcolor, bgcolor;


void delay_us(int c){
    int con = c * 5;
    int i;
    for(i = con; i > 0; i--);
}

void delay_ms(int c){
    int con = c * 4444;
    int i;
    for(i = con; i > 0; i--);
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
    DISPLAY_VDD_PORT |= DISPLAY_VDD_MASK;
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
	write_data_8(0x13);

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
	delay_ms(100);
	write_cmd_8(ILI9341_GRAM);

	Mode16();
}

void setAddress(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2) {
	write_cmd_16(0x2A);//col
	write_data_16(x1);
	write_data_16(x2);
	write_cmd_16(0x2B);//row
	write_data_16(y1);
	write_data_16(y2);
	write_cmd_16(ILI9341_GRAM);
}

void rotate(char c) {
	write_cmd_16(ILI9341_MAC);
	if (c == 1) {
		write_data_16(0x48);
	} else if (c == 2) {
		write_data_16(0x28);
	} else if (c == 3) {
		write_data_16(0x88);
	} else if (c == 4) {
		write_data_16(0xE8);
	}
}

void drawBlock(uint16_t x, uint16_t y, uint32_t color){
	fillRect(x, y, BLOCK_LEN, BLOCK_THICC,color);
}


void drawPixel(uint16_t x1, uint16_t y1, uint32_t color) {
	setAddress(x1,y1, x1+1, y1+1);
	write_data_16(color);
}

void fillSceen(uint16_t color) {
	fillRect(0,0,240,320,color);
}

void fillRect(uint16_t x, uint16_t y, uint16_t w, uint16_t h,
  uint16_t color){
/* Draw a filled rectangle with starting top-left vertex (x,y),
 *  width w and height h with given color
 * Parameters:
 *      x:  x-coordinate of top-left vertex; top left of screen is x=0
 *              and x increases to the right
 *      y:  y-coordinate of top-left vertex; top left of screen is y=0
 *              and y increases to the bottom
 *      w:  width of rectangle
 *      h:  height of rectangle
 *      color:  16-bit color value
 * Returns:     Nothing
 */
  // rudimentary clipping (drawChar w/big text requires this)
  if((x >= LCD_W) || (y >= LCD_H)) return;
  if((x + w - 1) >= LCD_W)  w = LCD_W  - x;
  if((y + h - 1) >= LCD_H) h = LCD_H - y;

  setAddress(x, y, x+w-1, y+h-1);



  for(y=h; y>0; y--) {
    for(x=w; x>0; x--) {
        write_data_16(color);
    }
  }
}

void drawPaddle(uint16_t x, uint16_t y, uint16_t color){
	fillRect(x, y, PADDLE_LENGTH, PADDLE_THICKNESS, color);
}



void drawCircle(int16_t x0, int16_t y0, int16_t r, uint32_t color) {
	int16_t f = 1 - r;
	int16_t ddF_x = 1;
	int16_t ddF_y = -2 * r;
	int16_t x = 0;
	int16_t y = r;

    drawPixel(x0, y0 + r, color);
    drawPixel(x0, y0 - r, color);
    drawPixel(x0 + r, y0, color);
    drawPixel(x0 - r, y0, color);

    while (x < y) {
        if (f >= 0) {
            y--;
            ddF_y += 2;
            f += ddF_y;
        }
        x++;
        ddF_x += 2;
        f += ddF_x;

        drawPixel(x0 + x, y0 + y, color);
        drawPixel(x0 - x, y0 + y, color);
        drawPixel(x0 + x, y0 - y, color);
        drawPixel(x0 - x, y0 - y, color);

        drawPixel(x0 + y, y0 + x, color);
        drawPixel(x0 - y, y0 + x, color);
        drawPixel(x0 + y, y0 - x, color);
        drawPixel(x0 - y, y0 - x, color);
    }
}

void drawChar(uint16_t x, uint16_t y, uint8_t c, uint32_t color, uint32_t bg_color, uint8_t size){
	char i, j;
	if ((x >= LCD_W) ||
		(y >= LCD_H) ||
		((x + 6 * size - 1) < 0) ||
		((y + 8 * size - 1) < 0))
	return;

	for (i = 0; i < 6; i++){
		uint8_t line;
		if (i == 5)
			line = 0x0;
		else
			line = pgm_read_byte(font + (c*5) + i);
		for(j = 0; j < 8; j++){
			if (line & 0x1){
				if(size == 1)
					drawPixel(x+(-i), y+j, color);
				else
					fillRect(x+(-i*size), y+(j*size), size, size, color);
			}
			else if (bg_color != color) {
				if(size == 1)
					drawPixel(x+(-i), y+j, bg_color);
				else
					fillRect(x+(-i*size), y+(j*size), size, size, bg_color);
			}
			line >>= 1;
		}
	}
}

void drawSymbol(uint16_t x, uint16_t y, uint8_t c, uint32_t color, uint32_t bg_color, uint8_t size){
	drawChar(4*size+x, y, c, color, bg_color, size);
}

void setCursor(uint16_t x, uint16_t y){
	Xcursor = x;
	Ycursor = y;
}
void setTextColor(uint32_t color, uint32_t bg_color){
	textcolor = color;
	bgcolor = bg_color;
}
void setTextSize(uint8_t size){
	textsize = size;
}
void setWrap(uint8_t wrap){
	textwrap = wrap;
}

void writeScreen( uint8_t c){
	if (c == '\n'){
		Xcursor += textsize*8;
		Ycursor = 0;
	}
	else if(c== '\r'){
		//do nothing
	}
	else if (c == '\t'){
		int new = Xcursor + 4; //for tabbing
		if (new < LCD_W){
			Xcursor = new;
		}
	}
	else {
		drawSymbol(Xcursor, Ycursor, c, textcolor, bgcolor, textsize);
		Xcursor += textsize*6;
		if (textwrap && (Xcursor > (LCD_W - textsize*6))){
			Ycursor += textsize*8;
			Xcursor = 0;
		}
	}
}
void writeString(char* string){
	int j;
	int count = 0;
	for(j = 0; string[0] != '\0'; j++){
		count++;
	}
	int i; 
	for(i = count; i >= 0; i--){
		writeScreen(string[i]);
	}
}


void drawBitmap(uint16_t x, uint16_t y,
			      const uint8_t *bitmap, uint16_t w, uint16_t h,
			      uint32_t color) {

  short i, j, byteWidth = (w + 7) / 8;//block size

  for(j=0; j<h; j++) {
    for(i=0; i<w; i++ ) {
      if(pgm_read_byte(bitmap + j * byteWidth + i / 8) & (128 >> (i & 7))) {
			drawPixel(x+i, y+j, color);
      }
    }
  }
}