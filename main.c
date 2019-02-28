#include <pic32mx.h>
#include "ili9341.h"
#include "spi.h"
#include "assets.h"
#include "control.h"

#define PADDLE_COLOR MAGENTA
#define PADDLE2_COLOR BLUE
#define BG_COLOR BLACK
#define GAME_SPEED 100
#define PADDLE_Y 270
#define PADDLE2_Y 20
#define PADDLE_THICKNESS 10
#define PADDLE_LENGTH 50
#define BLOCK_THICC 15
#define BLOCK_LEN 30
#define MENUTEXTCOL BLACK
#define MENUTEXTBGCOL WHITE
#define XSTART 120
#define YSTART 260

#define BALL_R 5

int ballX = XSTART;
int ballY = YSTART;
int ballSpeed;
int updateX = 1;
int updateY = -1;
int ballCounter = 0;
int multiPlayer = 0;

uint16_t xCord;
uint16_t x2Cord;
uint16_t lastxCord;
uint16_t lastx2Cord;
int counter = 0;



void updateBall() {
    drawCircle(ballX, ballY, BALL_R, BG_COLOR);
    ballX = ballX + updateX;
    ballY = ballY + updateY;
    drawCircle(ballX, ballY, BALL_R, WHITE);
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
    lastx2Cord = x2Cord;
    x2Cord = calcCord(analogIN2);


}

void advance () {
   
    int i;
    for(i = 0; i < 48; i++){
        if(level1[i][0] == 1){
            if
            (
                ballX >= level1[i][1] - BALL_R &&
                ballX <= (level1[i][1] + BLOCK_LEN + BALL_R) &&
                ballY >= level1[i][2] - BALL_R &&
                ballY <= (level1[i][2] + BLOCK_THICC + BALL_R)
            )
                {   
                    if(ballX + BALL_R == level1[i][1] || ballX - BALL_R == level1[i][1] + BLOCK_LEN)
                    {
                        updateX = updateX * -1;
                        goto over;
                    }
                    updateY = updateY * -1;
                    over:
                    level1[i][0] = 0;
                    drawBlock(level1[i][1], level1[i][2], BG_COLOR);
                    goto out;
                }
        }
    }
    out:
  
    if(
        ballX >= xCord - BALL_R &&
        ballX <= (xCord + PADDLE_LENGTH / 2 + BALL_R) &&
        ballY >= PADDLE_Y - BALL_R - 1 &&
        ballY <= (PADDLE_Y + PADDLE_THICKNESS + BALL_R)
    )
        {

            if(updateX > 0){
                updateX = updateX * -1;
            }
            updateY = updateY * -1;
        }

    if(
        ballX > xCord + PADDLE_LENGTH/2 - BALL_R &&
        ballX <= (xCord + PADDLE_LENGTH + BALL_R) &&
        ballY >= PADDLE_Y - BALL_R - 1 &&
        ballY <= (PADDLE_Y + PADDLE_THICKNESS + BALL_R)
    )
        {
            if(ballX + BALL_R == xCord || ballX - BALL_R == xCord + PADDLE_LENGTH)
                {
                    updateX = updateX * -1;
                    goto yey;
                }
            if(updateX < 0)
            {
                updateX = updateX * -1;
            }
            updateY = updateY * -1;
            yey:;
        }


    if(ballX >= 240 - BALL_R || ballX == BALL_R)
    {
        updateX = updateX * -1;
    }

    if(ballY == BALL_R)
    {
        updateY = updateY * -1;
    }
    if(ballY >= 320 - BALL_R)
    {
        ballX = xCord + PADDLE_LENGTH/2;
        ballY = YSTART;
        updateX = 1;
        updateY = -1;
    }

    updateBall();
}

int calcCord(int c){
    return c > 0 ? ((191 * c)/1024) : 0;
}



void drawLevel(char x){
    int i;
    if(x == 1){
        for(i = 0; i< 48 ;i++){
            drawBlock(level1[i][1], level1[i][2], level1[i][3]);
        }
    }
     if(x == 2){
        for(i = 0; i< 26 ;i++){
            drawBlock(level2[i][1], level2[i][2], level2[i][3]);
        }
    }
     if(x == 3){
        for(i = 0; i< 51 ;i++){
            drawBlock(level3[i][1], level3[i][2], level3[i][3]);
        }
    }
}

void updatePaddle(uint16_t x, uint16_t last, uint16_t height, uint32_t color){
    int diff = 0;
    diff = x - last;
    if ((diff < 50) | (diff > -50)){
        if(diff > 0){
            fillRect(last,
                    height,
                    diff,
                    10,
                    BG_COLOR);

            fillRect(x + (50 - diff),
                    height,
                    diff,
                    10,
                    color);
        }
        else{
            fillRect(last + (50+diff),
                    height,
                    (diff * -1),
                    10,
                    BG_COLOR);
            
            fillRect(x,
                    height,
                    (diff * -1),
                    10,
                    color);
        }
    }
    else{
        drawPaddle(last, height, BG_COLOR);
        drawPaddle(x, height, color);
    }
}


void splash(){
	fillSceen(WHITE);
	setCursor(10,50);
  setTextSize(4);
  setTextColor(BLACK, WHITE);
  setWrap(0);
  writeString("BREAKOUT!");
	while(1){
		if(getbtns() & 0x4)
			break;
		setCursor(50, 260);
		setTextSize(1);
		setTextColor(BLACK, WHITE);
		writeString("Press BTN4 to START!");
		delay_ms(500);
		if(getbtns() & 0x4)
			break;
		setCursor(50, 260);
		setTextSize(1);
		setTextColor(WHITE, WHITE);
		writeString("Press BTN4 to START!");
		delay_ms(500);
		}
}
void drawMenuSPtext(uint32_t tcol, uint32_t bcol){
    setTextColor(tcol, bcol);
    setCursor(45,50);
    writeString("Single-Player");
}
void drawMenuMPtext(uint32_t tcol, uint32_t bcol){
    setTextColor(tcol, bcol);
    setCursor(52, 125);
	writeString("Multi-Player");
}
void drawMenuSMtext(uint32_t tcol, uint32_t bcol){
    setTextColor(tcol, bcol);
    setCursor(90, 200);
	writeString("Stuff");
}
void drawLevelText1(uint32_t tcol, uint32_t bcol){
    setTextColor(tcol, bcol);
    setCursor(45,50);
    writeString("Level 1");
}
void drawLevelText2(uint32_t tcol, uint32_t bcol){
    setTextColor(tcol, bcol);
    setCursor(52, 125);
	writeString("Level 2");
}
void drawLevelText3(uint32_t tcol, uint32_t bcol){
    setTextColor(tcol, bcol);
    setCursor(90, 200);
	writeString("Level 3");
}

void levelSelect(){
    fillSceen(WHITE);
    setTextSize(2);
    drawLevelText1(MENUTEXTCOL,MENUTEXTBGCOL);
    drawLevelText2(MENUTEXTCOL,MENUTEXTBGCOL);
    drawLevelText3(MENUTEXTCOL,MENUTEXTBGCOL);
    while(1){
        inputRead();
        if(xCord >= 160 && xCord <=240){
            drawLevelText3(MENUTEXTCOL,MENUTEXTBGCOL);
            drawLevelText2(MENUTEXTCOL,MENUTEXTBGCOL);
            drawLevelText1(MENUTEXTBGCOL,MENUTEXTCOL);
            while(xCord >= 160 && xCord <=240){
                inputRead();
                if (getbtns() & 0x4){
                    ballSpeed = 50;
                    fillSceen(BG_COLOR);
                    drawCircle(ballX, ballY, BALL_R, WHITE);
                    drawLevel(1);
                    IECSET(1)=0x2;
                    enableTimer2(3, 0x18, 0x111, 1);
                    enable_interrupt();
                    goto done;
                }
            }
        }
        else if(xCord >= 80 && xCord <=160){
            drawLevelText1(MENUTEXTCOL,MENUTEXTBGCOL);
            drawLevelText3(MENUTEXTCOL,MENUTEXTBGCOL);
            drawLevelText2(MENUTEXTBGCOL,MENUTEXTCOL);
            while(xCord >= 80 && xCord <=160){
                inputRead();
                if (getbtns() & 0x4){
                    ballSpeed = 25;
                    fillSceen(BG_COLOR);
                    drawCircle(ballX, ballY, BALL_R, WHITE);
                    drawLevel(2);
                    IECSET(1)=0x2;
                    enableTimer2(3, 0x18, 0x111, 1);
                    enable_interrupt();
                    goto done;
                }
            }
        }
		else if(xCord >= 0 && xCord <=80){
            drawLevelText1(MENUTEXTCOL,MENUTEXTBGCOL);
            drawLevelText2(MENUTEXTCOL,MENUTEXTBGCOL);
            drawLevelText3(MENUTEXTBGCOL,MENUTEXTCOL);
            while(xCord >= 0 && xCord <=80){
                inputRead();
                if (getbtns() & 0x4){
                    ballSpeed = 10;
                    fillSceen(BG_COLOR);
                    drawCircle(ballX, ballY, BALL_R, WHITE);
                    drawLevel(3);
                    IECSET(1)=0x2;
                    enableTimer2(3, 0x18, 0x111, 1);
                    enable_interrupt();
                    goto done;
                }
            }
        }
	}	
    done:;
}






void mainMenu(){
	fillSceen(WHITE);
    setTextSize(2);
    drawMenuSPtext(MENUTEXTCOL, MENUTEXTBGCOL);
    drawMenuMPtext(MENUTEXTCOL, MENUTEXTBGCOL);
    drawMenuSMtext(MENUTEXTCOL, MENUTEXTBGCOL);
	while(1){
        inputRead();
        if(xCord >= 160 && xCord <=240){
            drawMenuMPtext(MENUTEXTCOL, MENUTEXTBGCOL);
            drawMenuSMtext(MENUTEXTCOL, MENUTEXTBGCOL);
            drawMenuSPtext(MENUTEXTBGCOL, MENUTEXTCOL);
            while(xCord >= 160 && xCord <=240){
                inputRead();
                if (getbtns() & 0x4){
                    multiPlayer = 0;
                    levelSelect();
                }
            }
        }
        else if(xCord >= 80 && xCord <=160){
            inputRead();
            drawMenuSPtext(MENUTEXTCOL, MENUTEXTBGCOL);
            drawMenuSMtext(MENUTEXTCOL, MENUTEXTBGCOL);
            drawMenuMPtext(MENUTEXTBGCOL, MENUTEXTCOL);
            while(xCord >= 80 && xCord <=160){
                inputRead();
                if (getbtns() & 0x4){
                    multiPlayer = 1;
                    levelSelect();
                }
            }
        }
		else if(xCord >= 0 && xCord <=80){
            drawMenuSPtext(MENUTEXTCOL, MENUTEXTBGCOL);
            drawMenuMPtext(MENUTEXTCOL, MENUTEXTBGCOL);
            drawMenuSMtext(MENUTEXTBGCOL, MENUTEXTCOL);
            while(xCord >= 0 && xCord <=80){
                inputRead();
                if (getbtns() & 0x4){
                    levelSelect();
                }
            }
        }
	}	
}











int main(void){
    display_init();
    rotate(1);
    enablePots();
    splash();
    mainMenu();
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
            //btn4
        }
        if(getbtns() & 2)
        {
            //btn3
        }
        if(getbtns() & 1)
        {
            //stuff for BTN2
        }
        
    }
        inputRead();
        updatePaddle(xCord, lastxCord, PADDLE_Y, PADDLE_COLOR);
        if (multiPlayer == 1){
        updatePaddle(x2Cord, lastx2Cord, PADDLE2_Y, PADDLE2_COLOR);
        }

    ballCounter++;

    if(ballCounter == ballSpeed){
        ballCounter = 0;
        advance();

    }

}