#ifndef __TIMER_H
#define __TIMER_H

#include <stdint.h>

typedef enum {
    PWM_INPUT_1,
    PWM_INPUT_2,
} TimerPWMInput;

void timerInit();
void timerSetPWM(uint8_t dutyCycle);
uint8_t timerGetPWMReading(TimerPWMInput pwmInput);




#endif // __TIMER_H