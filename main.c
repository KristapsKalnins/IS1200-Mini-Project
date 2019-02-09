#include <pic32mx.h>
int main(void){
    spi_initalize();
    wake();
    printf("%d\n", 5);



    return 0;
}