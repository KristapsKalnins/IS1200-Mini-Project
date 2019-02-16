#include <pic32mx.h>
#include "ili9341.h"
#include "spi.h"

#define PADDLE_COLOR BLUE
#define BG_COLOR BLACK
int main(void){
    display_init();
    rotate(1);
    fillSceen(BG_COLOR);
    while(1){
        int i;
        for(i = 0; i < 190; i++){
            fillRect(i+50, 250, 1, 10, PADDLE_COLOR);
            delay_us(2000);
            fillRect(i, 250, 1, 10, BG_COLOR);
        }
        for(i = 190; i > 0; i--){
            fillRect(i, 250, 1, 10, PADDLE_COLOR);
            delay_us(2000);
            fillRect(i + 50, 250, 1, 10, BG_COLOR);
        }
    }
    return 0;
}