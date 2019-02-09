#include <pic32mx.h>
#include "ili9341.h"
int main(void){
    display_init();
    while(1){
   drawPixel(100,100,RED);
}
    return 0;
}