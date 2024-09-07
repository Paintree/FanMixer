#ifndef __TIMER_H
#define __TIMER_H

#include <stdint.h>

typedef enum {
    PWM_INPUT_1,
    PWM_INPUT_2,
} TimerPWMInput;

typedef enum {
    TIMER_NOT_STARTED,
    TIMER_RUNNING,
    TIMER_NO_UPDATES,
} TimerStatus;

void timerInit();
void timerSetPWM(uint8_t dutyCycle);
uint8_t timerGetPWMReading(TimerPWMInput pwmInput);
TimerStatus getPWMTimerStatus(TimerPWMInput pwmInput);
void timerStep();




#endif // __TIMER_H