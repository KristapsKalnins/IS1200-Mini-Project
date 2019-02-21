#include <pic32mx.h>
#include "ili9341.h"
#include "spi.h"
#include "assets.h"
#include "control.h"

#define PADDLE_COLOR RED
#define BG_COLOR WHITE
#define GAME_SPEED 100
int xCord;
int last;

int calcCord(int c){
    return c > 0 ? ((190 * c)/1024) : 0;
}

void inputRead() { 
    int analogIN1;
    int analogIN2;

    IFSCLR(1) = 0x2; //clears the ADC interrupt flag
    AD1CON1SET = 0x4; //Start sampling automatically
    while(!IFS(1) & 0x2); //While sampling, do nothing
            
    if (AD1CON2 & 0x80){
        analogIN1 = ADC1BUF0;
        analogIN2 = ADC1BUF1;   
    }
    else{
        analogIN1 = ADC1BUF8;
        analogIN2 = ADC1BUF9;
    }
    
    last = xCord;
    xCord = calcCord(analogIN1);


}

int counter = 0;


int main(void){
    display_init();
    rotate(1);
    fillSceen(BG_COLOR);
    IECSET(1)=0x2;
    enablePots();

    enableTimer2(312500, 0x18, 0x111, 1);
    enable_interrupt();

   
    return 0;
}

void timer2_interrupt_handler(void)
{
    IFSCLR(0) = 0x100;
    counter++;

    if(counter != GAME_SPEED ){return;}
    counter = 0;

    if(getbtns() != 0)
    {
        if(getbtns() & 4)
        {
            //stuff for BTN4
        }
        if(getbtns() & 2)
        {
            //stuff for BTN3
        }
        if(getbtns() & 1)
        {
            //stuff for BTN2
        }
        
    }
        inputRead();

        drawPaddle(last, 250, BG_COLOR);
        drawPaddle(xCord, 250, PADDLE_COLOR);
      //  delay_us(100);

        //for(i  = 20; i < 220; i++){
            //drawCircle(i,i,20,RED);
            //delay_ms(10);
            //drawCircle(i-1,i-1,20,BG_COLOR);
        //}
        // for(i = 0; i < 190; i++){
        //     fillRect(i+50, 250, 1, 10, PADDLE_COLOR);
        //     delay_ms(20);
        //     fillRect(i, 250, 1, 10, BG_COLOR);
        // }
        // for(i = 190; i > 0; i--){
        //     fillRect(i, 250, 1, 10, PADDLE_COLOR);
        //     delay_ms(20);
        //     fillRect(i + 50, 250, 1, 10, BG_COLOR);
        // }
        //drawBitmap(icon);
        // for (i = 0; i<=240 ; i++){
        // drawCircle(240,160,i,RED);
        // drawCircle(0, 160, i, BLUE);
        // }
}