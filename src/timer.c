#include <stm32f1xx_hal.h>
#include <timer.h>
#include <gpio.h>

#define TIM1_CLK_ENABLE     __HAL_RCC_TIM1_CLK_ENABLE
#define TIM2_CLK_ENABLE     __HAL_RCC_TIM2_CLK_ENABLE
#define TIM3_CLK_ENABLE     __HAL_RCC_TIM3_CLK_ENABLE
#define AFIO_REMAP_TIM2     __HAL_AFIO_REMAP_TIM2_PARTIAL_2

#define TIM_COUNT           3
#define TIM_PWM_GEN_PERIOD  2879 // Set timer to 25 kHz
#define TIM_PWM_READ_PERIOD 0xFFFF // Set timer to MAX Frequency


static uint8_t dutyCycle[TIM_COUNT - 1] = {100, 100}; // Only two timers are calculating duty cycle
static TimerStatus timerStatus[TIM_COUNT - 1];
static uint8_t timerInt[TIM_COUNT - 1];


static TIM_HandleTypeDef htim[TIM_COUNT] = {
    {
        .Instance = TIM1,
        .Init.Prescaler = 0,
        .Init.CounterMode = TIM_COUNTERMODE_UP,
        .Init.Period = TIM_PWM_GEN_PERIOD,
        .Init.ClockDivision = TIM_CLOCKDIVISION_DIV1,
        .Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_ENABLE,
    },{
        .Instance = TIM2,
        .Init.Prescaler = 0,
        .Init.CounterMode = TIM_COUNTERMODE_UP,
        .Init.Period = TIM_PWM_READ_PERIOD,
        .Init.ClockDivision = TIM_CLOCKDIVISION_DIV1,
        .Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_ENABLE,
    },{
        .Instance = TIM3,
        .Init.Prescaler = 0,
        .Init.CounterMode = TIM_COUNTERMODE_UP,
        .Init.Period = TIM_PWM_READ_PERIOD,
        .Init.ClockDivision = TIM_CLOCKDIVISION_DIV1,
        .Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_ENABLE,
    },
}; 

void timerInit() {
    TIM_ClockConfigTypeDef sClockSourceConfig = {
        .ClockSource = TIM_CLOCKSOURCE_INTERNAL,
    };

    TIM_SlaveConfigTypeDef sSlaveConfig = {
        .SlaveMode = TIM_SLAVEMODE_RESET,
        .InputTrigger = TIM_TS_TI1FP1,
        .TriggerPolarity = TIM_INPUTCHANNELPOLARITY_RISING,
        .TriggerPrescaler = TIM_ICPSC_DIV1,
        .TriggerFilter = 0,
    };

    TIM_MasterConfigTypeDef sMasterConfig = {
        .MasterOutputTrigger = TIM_TRGO_RESET,
        .MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE,
    };

    TIM_IC_InitTypeDef sConfigIC = {
        .ICPrescaler = TIM_ICPSC_DIV1,
        .ICFilter = 0x0,
    };

    TIM_OC_InitTypeDef sConfigOC = {
        .OCMode = TIM_OCMODE_PWM1,
        .Pulse = TIM_PWM_GEN_PERIOD, // Initialize fans at 100 %
        .OCPolarity = TIM_OCPOLARITY_HIGH,
        .OCFastMode = TIM_OCFAST_DISABLE,
    };

    TIM1_CLK_ENABLE();
    TIM2_CLK_ENABLE();
    TIM3_CLK_ENABLE();
    AFIO_REMAP_TIM2();

    for (int i = 0; i < TIM_COUNT; i++) {
        HAL_TIM_Base_Init(&htim[i]);

        HAL_TIM_ConfigClockSource(&htim[i], &sClockSourceConfig);

        HAL_TIMEx_MasterConfigSynchronization(&htim[i], &sMasterConfig);

    }

    for (int i = 1; i < TIM_COUNT; i++) {
        HAL_TIM_IC_Init(&htim[i]);
        HAL_TIM_SlaveConfigSynchro(&htim[i], &sSlaveConfig);

        sConfigIC.ICPolarity    = TIM_INPUTCHANNELPOLARITY_RISING;
        sConfigIC.ICSelection   = TIM_ICSELECTION_DIRECTTI;
        HAL_TIM_IC_ConfigChannel(&htim[i], &sConfigIC, TIM_CHANNEL_1);

        sConfigIC.ICPolarity    = TIM_INPUTCHANNELPOLARITY_FALLING;
        sConfigIC.ICSelection   = TIM_ICSELECTION_INDIRECTTI;
        HAL_TIM_IC_ConfigChannel(&htim[i], &sConfigIC, TIM_CHANNEL_2);

        HAL_TIM_IC_Start_IT(&htim[i], TIM_CHANNEL_1);
        HAL_TIM_IC_Start(&htim[i], TIM_CHANNEL_2);
    }

    HAL_NVIC_SetPriority(TIM2_IRQn, 0, 0);
    HAL_NVIC_EnableIRQ(TIM2_IRQn);

    HAL_NVIC_SetPriority(TIM3_IRQn, 0, 0);
    HAL_NVIC_EnableIRQ(TIM3_IRQn);

    HAL_TIM_PWM_Init(&htim[0]);
    HAL_TIM_PWM_ConfigChannel(&htim[0], &sConfigOC, TIM_CHANNEL_1);
    HAL_TIM_PWM_Start(&htim[0], TIM_CHANNEL_1);
}

void timerSetPWM(uint8_t dutyCycle) {
    __HAL_TIM_SET_COMPARE(&htim[0], TIM_CHANNEL_1, (dutyCycle * (TIM_PWM_GEN_PERIOD + 1))/100);
}

uint8_t timerGetPWMReading(TimerPWMInput pwmInput) {
    return dutyCycle[pwmInput];
}

uint8_t calculateDutyCycle(TIM_HandleTypeDef *htim) {
    uint8_t duty = 0;
    uint32_t timerCounter = HAL_TIM_ReadCapturedValue(htim, TIM_CHANNEL_1);

    if (timerCounter != 0) {
	    duty = (HAL_TIM_ReadCapturedValue(htim, TIM_CHANNEL_2) * 100) / timerCounter;
	}
    if (duty == 0) return 100;
    else return duty + 1;
}

TimerStatus getPWMTimerStatus(TimerPWMInput id) {
    return timerStatus[id];
}

void timerStep() {
    for(int i = 0; i < TIM_COUNT-1; i++) {
        if (timerStatus[i] == TIMER_NOT_STARTED) {
            timerInt[i] = 0;
        } else if (timerInt[i] == 0) {
            timerStatus[i] = TIMER_NO_UPDATES;
        } else {
            timerStatus[i] = TIMER_RUNNING;
            timerInt[i] = 0;
        }
    }
}

void HAL_TIM_IC_CaptureCallback(TIM_HandleTypeDef *htim)
{
    if (htim->Instance == TIM2) {
        if (htim->Channel == HAL_TIM_ACTIVE_CHANNEL_1) {
            dutyCycle[0] = calculateDutyCycle(htim);
        }

    } else if (htim->Instance == TIM3) {
        if (htim->Channel == HAL_TIM_ACTIVE_CHANNEL_1){
		    dutyCycle[1] = calculateDutyCycle(htim);
        }
    }
}

void TIM1_IRQHandler(void) {

  HAL_TIM_IRQHandler(&htim[0]);

}

void TIM2_IRQHandler(void) {
    if(timerStatus[PWM_INPUT_1] == TIMER_NOT_STARTED || timerStatus[PWM_INPUT_1] == TIMER_NO_UPDATES)
        timerStatus[PWM_INPUT_1] = TIMER_RUNNING;

    HAL_TIM_IRQHandler(&htim[1]);
    timerInt[PWM_INPUT_1] = 1; // Bool to check if update happened

}

void TIM3_IRQHandler(void) {
    if(timerStatus[PWM_INPUT_2] == TIMER_NOT_STARTED || timerStatus[PWM_INPUT_2] == TIMER_NO_UPDATES)
        timerStatus[PWM_INPUT_2] = TIMER_RUNNING;

  HAL_TIM_IRQHandler(&htim[2]);
  timerInt[PWM_INPUT_2] = 1; // Bool to check if update happened

}

