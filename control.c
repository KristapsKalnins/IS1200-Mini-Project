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

    AD1CON3SET = 0x8000; //'clock thing

    // CSSL: ADC Input Pin Scan Selection bits
    AD1CSSLSET = 0x0010;   // external pot

    // start
    AD1CON1SET = 0x8000;
}

void enableTimer2(int period, int priority, int prescaling, int interrupts)
{
    T2CON = 0x0;            //stop timer
    TMR2 = 0;               //clear timer
    PR2 = period;           //set period to parameter value
    IPCSET(2) = priority;   //set priority to parameter value

    IFSCLR(0) = 0x100;      //reset flag

    if(interrupts == 1)
    {
        IECSET(0) = 0x100;  //if interrupts 1 then enable timer interrupts
    }

    T2CONSET = prescaling << 4; //set presecaling

    T2CONSET = 0x8000;      //enable
}

int getbtns(void)
{
    return ((PORTD & 0x00e0) >> 5); // return button values  
}

int getsw(void)
{
    return ((PORTD & 0x0f00) >> 8); //retrun switch values
}