#include <pic32mx.h>
#include <stdint.h>

void enablePots() {
    // reset
    AD1CON1 = 0x0000;
    AD1CON2 = 0x0000;
    AD1CON3 = 0x0000;

    // ASAM: ADC Sample Auto-Start bit
    AD1CON1SET = 0x4;
    // SSRC: auto convert
    AD1CON1SET = 0xE0;
    // FORM: uint32, 0 - 1024
    AD1CON1SET = 0x400;

    // BUFM: Result buffer mode, split in 2
    AD1CON2SET = 0x2;
    // SMPI: 2 sample/convert sequences between interrupts
    AD1CON2SET = 0x4;
    // CSCNA: Scan inputs
    AD1CON2SET = 0x400;

    // SAMC: 31 TAD
    AD1CON3SET = 0xF00;

    // CSSL: ADC Input Pin Scan Selection bits
    AD1CSSLSET = 0x0110;

    // start
    AD1CON1SET = 0x8000;
}