#ifndef __TIMER_H
#define __TIMER_H

#include <stdint.h>

typedef enum {
    PWM_OUTPUT_0,
    PWM_OUTPUT_1,
} TimerPWMOutput;

void timerInit();
void setPWM(TimerPWMOutput timerOutput, uint8_t pwm);



#endif // __TIMER_H