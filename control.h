#ifndef control
#define control

#include <stdint.h>
void enablePots();
void enableTimer2(int period, int priority, int prescaling, int interrupts);
#endif