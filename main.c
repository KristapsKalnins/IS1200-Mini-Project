#include <pic32mx.h>
#include "ili9341.h"
#include "spi.h"
#include "assets.h"

#define PADDLE_COLOR YELLOW
#define BG_COLOR BLACK
int main(void){
    display_init();
    rotate(1);
    fillSceen(BG_COLOR);
    while(1){
        int i;
        for(i = 0; i < 240; i++){
        drawCircle(i, i, 10, WHITE);
        delay_ms(50);
        drawCircle(i-1, i-1, 10, BG_COLOR);
        }
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
    }
    return 0;
}