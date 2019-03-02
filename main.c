#include <pic32mx.h>
#include <stdlib.h>
#include "ili9341.h"
#include "spi.h"
#include "assets.h"
#include "control.h"

#define PADDLE_COLOR MAGENTA
#define PADDLE2_COLOR BLUE
#define BG_COLOR BLACK
// #define SCORE_COLOR YELLOW
#define GAME_SPEED 100
#define PADDLE_Y 270
#define PADDLE2_Y 40
#define PADDLE_THICKNESS 10
#define PADDLE_LENGTH 50
#define BLOCK_THICC 15
#define BLOCK_LEN 30
#define MENUTEXTCOL BLACK
#define MENUTEXTBGCOL WHITE
#define XSTART 120
#define YSTART 260
#define X2START 120
#define Y2START 60 

#define BALL_R 5

int scoreInd = 0;
int scoreIndprev = -1;
int scoreInd2 = 0;
int scoreIndprev2 = -1;
int lifecount = 2;
int lifecount2 = 2;
int maxYHit;
int paddle1Hit = 0;
int paddle2Hit = 0;
char hit = 0;

int ballX = XSTART;
int ballY = YSTART;
int ballSpeed;
int updateX = 1;
int updateY = -1;
int ballCounter = 0;
int multiPlayer = 0;
int hitInt = 0;

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
    xCord = calcCord(1024 -analogIN1);
    lastx2Cord = x2Cord;
    x2Cord = calcCord(1024 -analogIN2);


}


void gameOver(){
    fillSceen(RED);
    setCursor(50, 150);
    setTextColor(BLACK, RED);
    writeString("GAME OVER!");
    for(;;);
}


void advance () {

   if(multiPlayer == 0)
   {
        if(hitInt == 1)
        {
            int i;
            for(i = 0; i < 48; i++){
                if(level1[i][0] == 1){
                    if(i == 7 || i == 15 || i == 23 || i == 31 || i == 39 || i == 47)
                        {
                            if
                            (
                                ballX >= level1[i][1] &&
                                ballX <= (level1[i][1] + BLOCK_LEN + BALL_R) &&
                                ballY >= level1[i][2] &&
                                ballY <= (level1[i][2] + BLOCK_THICC + BALL_R) 
                            )
                            goto yeet;
                        }
                    if
                    (
                        ballX >= level1[i][1] - BALL_R &&
                        ballX <= (level1[i][1] + BLOCK_LEN + BALL_R) &&
                        ballY >= level1[i][2] - BALL_R &&
                        ballY <= (level1[i][2] + BLOCK_THICC + BALL_R)
                    )
                        {   
                            yeet: ;
                            if(ballX + BALL_R == level1[i][1] || ballX - BALL_R == level1[i][1] + BLOCK_LEN)
                            {
                                updateX = updateX * -1;
                                goto over;
                            }
                            updateY = updateY * -1;
                            over:
                            level1[i][0] = 0;
                            drawBlock(level1[i][1], level1[i][2], BG_COLOR);
                            scoreIndprev = scoreInd;
                            scoreInd++;

                            goto out;
                        }
                }
            }
            out: ;
        }

        if(hitInt == 2)
        {
            int i;
            for(i = 0; i < 26; i++){
                if(level2[i][0] == 1){
                    if
                    (
                        ballX >= level2[i][1] - BALL_R &&
                        ballX <= (level2[i][1] + BLOCK_LEN + BALL_R) &&
                        ballY >= level2[i][2] - BALL_R &&
                        ballY <= (level2[i][2] + BLOCK_THICC + BALL_R)
                    )
                        {   
                            if(ballX + BALL_R == level2[i][1] || ballX - BALL_R == level2[i][1] + BLOCK_LEN)
                            {
                                updateX = updateX * -1;
                                goto over2;
                            }
                            updateY = updateY * -1;
                            over2:
                            level2[i][0] = 0;
                            drawBlock(level2[i][1], level2[i][2], BG_COLOR);
                            scoreIndprev = scoreInd;
                            scoreInd++;

                            goto out2;
                        }
                }
            }
            out2: ;
        }

        if(hitInt == 3)
        {
            int i;
            for(i = 0; i < 51; i++){
                if(level3[i][0] == 1){
                    if
                    (
                        ballX >= level3[i][1] - BALL_R &&
                        ballX <= (level3[i][1] + BLOCK_LEN + BALL_R) &&
                        ballY >= level3[i][2] - BALL_R &&
                        ballY <= (level3[i][2] + BLOCK_THICC + BALL_R)
                    )
                        {   
                            if(ballX + BALL_R == level3[i][1] || ballX - BALL_R == level3[i][1] + BLOCK_LEN)
                            {
                                updateX = updateX * -1;
                                goto over3;
                            }
                            updateY = updateY * -1;
                            over3:
                            level3[i][0] = 0;
                            drawBlock(level3[i][1], level3[i][2], BG_COLOR);
                            scoreIndprev = scoreInd;
                            scoreInd++;

                            goto out3;
                        }
                }
            }
            out3: ;
        }
    }

    if(multiPlayer == 1)
   {
        if(hitInt == 1)
        {
            int i;
            for(i = 0; i < 48; i++){
                if(mullevel1[i][0] == 1){
                    if(i == 7 || i == 15 || i == 23 || i == 31 || i == 39 || i == 47)
                        {
                            if
                            (
                                ballX >= mullevel1[i][1] &&
                                ballX <= (mullevel1[i][1] + BLOCK_LEN + BALL_R) &&
                                ballY >= mullevel1[i][2] &&
                                ballY <= (mullevel1[i][2] + BLOCK_THICC + BALL_R) 
                            )
                            goto yeetm;
                        }
                    if
                    (
                        ballX >= mullevel1[i][1] - BALL_R &&
                        ballX <= (mullevel1[i][1] + BLOCK_LEN + BALL_R) &&
                        ballY >= mullevel1[i][2] - BALL_R &&
                        ballY <= (mullevel1[i][2] + BLOCK_THICC + BALL_R)
                    )
                        {   
                            yeetm: ;
                            if(ballX + BALL_R == mullevel1[i][1] || ballX - BALL_R == mullevel1[i][1] + BLOCK_LEN)
                            {
                                updateX = updateX * -1;
                                goto overm;
                            }
                            updateY = updateY * -1;
                            overm:
                            mullevel1[i][0] = 0;
                            drawBlock(mullevel1[i][1], mullevel1[i][2], BG_COLOR);
                            if(hit == 0){
                            scoreIndprev = scoreInd;
                            scoreInd++;
                            }
                            else{
                            scoreIndprev2 = scoreInd2;
                            scoreInd2++;
                            }

                            goto outm;
                        }
                }
            }
            outm: ;
        }

        if(hitInt == 2)
        {
            int i;
            for(i = 0; i < 26; i++){
                if(mullevel2[i][0] == 1){
                    if
                    (
                        ballX >= mullevel2[i][1] - BALL_R &&
                        ballX <= (mullevel2[i][1] + BLOCK_LEN + BALL_R) &&
                        ballY >= mullevel2[i][2] - BALL_R &&
                        ballY <= (mullevel2[i][2] + BLOCK_THICC + BALL_R)
                    )
                        {   
                            if(ballX + BALL_R == mullevel2[i][1] || ballX - BALL_R == mullevel2[i][1] + BLOCK_LEN)
                            {
                                updateX = updateX * -1;
                                goto over2m;
                            }
                            updateY = updateY * -1;
                            over2m:
                            mullevel2[i][0] = 0;
                            drawBlock(mullevel2[i][1], mullevel2[i][2], BG_COLOR);
                            if(hit == 0){
                            scoreIndprev = scoreInd;
                            scoreInd++;
                            }
                            else{
                            scoreIndprev2 = scoreInd2;
                            scoreInd2++;
                            }

                            goto out2m;
                        }
                }
            }
            out2m: ;
        }

        if(hitInt == 3)
        {
            int i;
            for(i = 0; i < 51; i++){
                if(mullevel3[i][0] == 1){
                    if
                    (
                        ballX >= mullevel3[i][1] - BALL_R &&
                        ballX <= (mullevel3[i][1] + BLOCK_LEN + BALL_R) &&
                        ballY >= mullevel3[i][2] - BALL_R &&
                        ballY <= (mullevel3[i][2] + BLOCK_THICC + BALL_R)
                    )
                        {   
                            if(ballX + BALL_R == mullevel3[i][1] || ballX - BALL_R == mullevel3[i][1] + BLOCK_LEN)
                            {
                                updateX = updateX * -1;
                                goto over3m;
                            }
                            updateY = updateY * -1;
                            over3m:
                            mullevel3[i][0] = 0;
                            drawBlock(mullevel3[i][1], mullevel3[i][2], BG_COLOR);
                            if(hit == 0){
                            scoreIndprev = scoreInd;
                            scoreInd++;
                            }
                            else{
                            scoreIndprev2 = scoreInd2;
                            scoreInd2++;
                            }
                            goto out3m;
                        }
                }
            }
            out3m: ;
        }
    }

    if(multiPlayer == 0 || multiPlayer == 1){
        if(paddle1Hit == 0)
        {
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
                    paddle1Hit = 2;
                    hit = 0;
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
                    paddle1Hit = 2;
                    hit = 0;
                    yey:;
                }
        }


        if(ballX >= 240 - BALL_R || ballX == BALL_R)
        {
            if(maxYHit == 0){
                updateX = updateX * -1;
                maxYHit = 1;
            }
        }

        if(ballY == BALL_R)
        {
            
                updateY = updateY * -1;
            
            
        }
        if(ballY >= 320 - BALL_R)
        {
            fillRect(ballX, ballY , BALL_R*2, BALL_R*2, BG_COLOR);
            if(multiPlayer == 0)
            {
                ballX = XSTART;
                ballY = YSTART;
                updateX = 1;
                updateY = -1;
                hit = 0;
            }
            if(multiPlayer == 1)
            {
                ballX = X2START;
                ballY = Y2START;
                updateX = -1;
                updateY = 1;
                hit = 1;
            }
            if (lifecount == 2)
                fillRect(196, 0,14,14,BG_COLOR);
            else if (lifecount == 1)
                fillRect(211, 0, 14, 14, BG_COLOR);
            else
                gameOver();
            lifecount--;
            
        }
    }

/*************************paddle2**********************************/

    if(multiPlayer == 1){
        if(paddle2Hit == 0)
        {
            if(
                ballX >= x2Cord - BALL_R &&
                ballX <= (x2Cord + PADDLE_LENGTH / 2 + BALL_R) &&
                ballY >= PADDLE2_Y - BALL_R - 1 &&
                ballY <= (PADDLE2_Y + PADDLE_THICKNESS + BALL_R)
            )
                {
                    if(updateX > 0){
                        updateX = updateX * -1;
                    }
                    updateY = updateY * -1;
                    paddle2Hit = 2;
                    hit = 1;      //score meme Kristaps
                }

            if(
                ballX >= x2Cord + PADDLE_LENGTH/2 - BALL_R &&
                ballX <= (x2Cord + PADDLE_LENGTH + BALL_R) &&
                ballY >= PADDLE2_Y - BALL_R - 1 &&
                ballY <= (PADDLE2_Y + PADDLE_THICKNESS + BALL_R)
            )
                {
                    
                    if(ballX + BALL_R == x2Cord || ballX - BALL_R == x2Cord + PADDLE_LENGTH)
                        {
                            updateX = updateX * -1;
                            hit = 1; // score meme Kristaps
                            goto yey2;
                        }
                    if(updateX < 0)
                    {
                        updateX = updateX * -1;
                    }
                    updateY = updateY * -1;
                    paddle2Hit = 2;
                    hit = 1;
                    yey2:;
                }
        }

        if(ballX >= 240 - BALL_R || ballX == BALL_R)
        {
            if(maxYHit == 0){
                updateX = updateX * -1;
                maxYHit = 1;
            }
        }

        if(ballY == BALL_R)
        {
            
                updateY = updateY * -1;
                
            
        }
        if(ballY <= 0 + BALL_R)
        {
            fillRect(ballX, ballY , BALL_R*2, BALL_R*2, BG_COLOR);
            ballX = XSTART;
            ballY = YSTART;
            hit = 0;
            updateX = 1;
            updateY = -1;
            if (lifecount2 == 2)
                fillRect(30, 306,14,14,BG_COLOR);
            else if (lifecount2 == 1)
                fillRect(15, 306, 14, 14, BG_COLOR);
            else
                gameOver();
            lifecount2--;
        }
    }
    paddle1Hit = paddle1Hit - 1;
    paddle2Hit = paddle2Hit - 1;
    maxYHit = 0;

    updateBall();
}

int calcCord(int c){
    return c > 0 ? ((191 * c)/1024) : 0;
}



void drawLevel(char x){
    int i;
    if(multiPlayer == 0){
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

    if(multiPlayer == 1){
        if(x == 1){
            for(i = 0; i< 48 ;i++){
                drawBlock(mullevel1[i][1], mullevel1[i][2], mullevel1[i][3]);
            }
        }
        if(x == 2){
            for(i = 0; i< 26 ;i++){
                drawBlock(mullevel2[i][1], mullevel2[i][2], mullevel2[i][3]);
            }
        }
        if(x == 3){
            for(i = 0; i< 51 ;i++){
                drawBlock(mullevel3[i][1], mullevel3[i][2], mullevel3[i][3]);
            }
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
void drawScoreText(){
    setCursor(34,0);
    setTextSize(2);
    setTextColor(PADDLE_COLOR, BG_COLOR);
    writeString("SCORE: ");
}
void drawScoreText2(){
    setCursor(156,306);
    setTextSize(2);
    setTextColor(PADDLE2_COLOR, BG_COLOR);
    writeString("SCORE: ");
}

void drawLife1(){
    fillRect(226,0,14,14,PADDLE_COLOR);
    fillRect(211,0,14,14,PADDLE_COLOR);
    fillRect(196,0,14,14,PADDLE_COLOR);
}

void drawLife2(){
    fillRect(0,306,14,14, PADDLE2_COLOR);
    fillRect(15,306,14,14,PADDLE2_COLOR);
    fillRect(30,306,14,14, PADDLE2_COLOR);
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
                    drawScoreText();
                    if(multiPlayer == 1){
                    drawScoreText2();
                    drawLife2();
                    }
                    drawLife1();
                    drawCircle(ballX, ballY, BALL_R, WHITE);
                    hitInt = 1;
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
                    drawScoreText();
                    if(multiPlayer == 1){
                    drawScoreText2();
                    drawLife2();
                    }
                    drawLife1();
                    drawCircle(ballX, ballY, BALL_R, WHITE);
                    hitInt = 2;
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
                    drawScoreText();
                    if(multiPlayer == 1){
                    drawScoreText2();
                    drawLife2();
                    }
                    drawLife1();
                    drawCircle(ballX, ballY, BALL_R, WHITE);
                    hitInt = 3;
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


void drawScore(uint32_t color, uint32_t bg_color, int index){
    setCursor(1,0);
    setTextSize(2);
    setTextColor(color, bg_color);
    writeString(scoreOut[index]);
}

void drawScore2(uint32_t color, uint32_t bg_color, int index){
    setCursor(123,306);
    setTextSize(2);
    setTextColor(color, bg_color);
    writeString(scoreOut[index]);
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
        if (scoreInd != scoreIndprev){
            drawScore(BG_COLOR, BG_COLOR, scoreIndprev);
            drawScore(PADDLE_COLOR, BG_COLOR, scoreInd);
            scoreIndprev = scoreInd;
        }
        else if(scoreInd2 != scoreIndprev2){
            drawScore2(BG_COLOR, BG_COLOR, scoreIndprev2);
            drawScore2(PADDLE2_COLOR, BG_COLOR, scoreInd2);
            scoreIndprev2 = scoreInd2;
        }

    ballCounter++;

    if(ballCounter == ballSpeed){
        ballCounter = 0;
        
        advance();

    }

}