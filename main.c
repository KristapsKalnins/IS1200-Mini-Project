#include <pic32mx.h>
#include "ili9341.h"
#include "spi.h"
#include "assets.h"
#include "control.h"


#define PADDLE_COLOR RED
#define BG_COLOR BLACK
#define GAME_SPEED 100
#define PADDLE_Y 270
#define BLOCK_COUNT1 25
#define BLOCK_COUNT2 13
#define BLOCK_COUNT3 16

int xCord;
int lastxCord;
int counter = 0;



int calcCord(int c){
    return c > 0 ? ((191 * c)/1024) : 0;
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
    
    lastxCord = xCord;
    xCord = calcCord(analogIN1);


}

void drawLevel(char x){
    int i;
    if(x == 1){
        for(i = 0; i< 25 ;i++){
            drawBlock(level1[i][1], level1[i][2], level1[i][3]);
        }
    }
}

void updatePaddle(){
    int diff;
    inputRead();
    diff = xCord - lastxCord;
    if ((diff < 50) | (diff > -50)){
        if(diff > 0){
            fillRect(lastxCord,
                    PADDLE_Y,
                    diff,
                    10,
                    BG_COLOR);

            fillRect(xCord + (50 - diff),
                    PADDLE_Y,
                    diff,
                    10,
                    PADDLE_COLOR);
        }
        else{
            fillRect(lastxCord + (50+diff),
                    PADDLE_Y,
                    diff,
                    10,
                    BG_COLOR);
            
            fillRect(xCord,
                    PADDLE_Y,
                    (diff * -1),
                    10,
                    PADDLE_COLOR);
        }
    }
    else{
        drawPaddle(lastxCord, PADDLE_Y, BG_COLOR);
        drawPaddle(xCord, PADDLE_Y, PADDLE_COLOR);
    }
}




int main(void){
    display_init();
    fillSceen(BG_COLOR);
    rotate(1);
    drawLevel(1);
    drawSymbol(0, 0,'G', WHITE, YELLOW, 5);
    IECSET(1)=0x2;
    enablePots();
    enableTimer2(3, 0x18, 0x111, 1);
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
        updatePaddle();
         //drawChar(0,0,'a', BLUE, BLUE, 5);



}