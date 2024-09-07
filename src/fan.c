#include <fan.h>
#include <timer.h>

#define INPUT_FANS_COUNT  2
#define OUTPUT_FANS_COUNT 1

/***************************************************************
    Logic for fan controller is implemented here. Change it so
    it suites your needs.

    IMPORTANT!
    DON'T SET PWM TO 0% FOR SOURCE SIGNAL AS THIS BOARD WILL
    INTREPRETE IT AS 100%. FIRMWARE WORKS BETWEEN 5 - 100% DUTY
    CYCLE.
****************************************************************/

// Default 100. Higher means bigger impact.
#define INPUT_FAN1_WEIGHT 100
#define INPUT_FAN2_WEIGHT 100
#define STARTUP_SPEED     30
#define MAX_SPEED         100

void fanInit() {
    timerSetPWM(STARTUP_SPEED); // Set Output fan to 30% at startup
}

uint8_t fanCalculateOutputSpeed(uint8_t fan1Speed, uint8_t fan2Speed) {
    uint8_t fanSpeed = ((fan1Speed * INPUT_FAN1_WEIGHT) 
                     + (fan2Speed * INPUT_FAN2_WEIGHT)) / 100;

    if (fanSpeed >= MAX_SPEED || fanSpeed < 1) {
        return MAX_SPEED;
    }
    return fanSpeed;
}

void fanStep() {
    TimerStatus status[INPUT_FANS_COUNT];
    uint8_t inputSingalDutyCycle[INPUT_FANS_COUNT] = {0};
    for (int i = 0; i < INPUT_FANS_COUNT; i++) {
        status[i] = getPWMTimerStatus(i);
        inputSingalDutyCycle[i] = timerGetPWMReading(i);
    }
    if (status[PWM_INPUT_1] == TIMER_NOT_STARTED && status[PWM_INPUT_2] == TIMER_NOT_STARTED) {
        timerSetPWM(STARTUP_SPEED);
        return;
    } else if (status[PWM_INPUT_1] == TIMER_RUNNING && status[PWM_INPUT_2] == TIMER_NOT_STARTED) {
        timerSetPWM((inputSingalDutyCycle[PWM_INPUT_1] * INPUT_FAN1_WEIGHT)/100);
        return;
    } else if (status[PWM_INPUT_1] == TIMER_NOT_STARTED && status[PWM_INPUT_2] == TIMER_RUNNING) {
        timerSetPWM((inputSingalDutyCycle[PWM_INPUT_2] * INPUT_FAN2_WEIGHT)/100);
        return;
    } else if (status[PWM_INPUT_1] == TIMER_NO_UPDATES || status[PWM_INPUT_2] == TIMER_NO_UPDATES) {
        timerSetPWM(MAX_SPEED);
        return;
    }
    timerSetPWM(fanCalculateOutputSpeed(inputSingalDutyCycle[PWM_INPUT_1],
                                        inputSingalDutyCycle[PWM_INPUT_2]));
}