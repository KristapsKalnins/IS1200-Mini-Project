#ifndef control
#define control

#include <stdint.h>
void enablePots();
void enableTimer2(int period, int priority, int prescaling, int interrupts);
int getbtns(void);
int getsw(void);
void enable_interrupt(void);
void disable_interrupt(void);
#endif