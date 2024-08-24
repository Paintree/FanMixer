#include <stm32f1xx_hal.h>
#include <timer.h>
#include <gpio.h>

#define TIM2_CLK_ENABLE     __HAL_RCC_TIM2_CLK_ENABLE
#define AFIO_CLK_ENABLE     __HAL_RCC_AFIO_CLK_ENABLE
#define AFIO_REMAP_TIM2     __HAL_AFIO_REMAP_TIM2_PARTIAL_2

#define TIM_COUNT           1
#define TIM_PERIOD          2879


static TIM_HandleTypeDef htim[TIM_COUNT] = {
    {
        .Instance = TIM2,
        .Init.Prescaler = 0,
        .Init.CounterMode = TIM_COUNTERMODE_UP,
        .Init.Period = TIM_PERIOD,
        .Init.ClockDivision = TIM_CLOCKDIVISION_DIV1,
        .Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_ENABLE,
    },
}; 

void timerInit() {
    TIM_ClockConfigTypeDef sClockSourceConfig = {
        .ClockSource = TIM_CLOCKSOURCE_INTERNAL,
    };

    TIM_MasterConfigTypeDef sMasterConfig = {
        .MasterOutputTrigger = TIM_TRGO_RESET,
        .MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE,
    };

    TIM_IC_InitTypeDef sConfigIC = {
        .ICPolarity = TIM_INPUTCHANNELPOLARITY_RISING,
        .ICSelection = TIM_ICSELECTION_DIRECTTI,
        .ICPrescaler = TIM_ICPSC_DIV1,
        .ICFilter = 0,
    };

    TIM_OC_InitTypeDef sConfigOC = {
        .OCMode = TIM_OCMODE_PWM1,
        .Pulse = 500,
        .OCPolarity = TIM_OCPOLARITY_HIGH,
        .OCFastMode = TIM_OCFAST_DISABLE,
    };

    TIM2_CLK_ENABLE();
    AFIO_CLK_ENABLE();
    AFIO_REMAP_TIM2();

    for (int i = 0; i < TIM_COUNT; i++) {
        HAL_TIM_Base_Init(&htim[i]);

        HAL_TIM_ConfigClockSource(&htim[i], &sClockSourceConfig);

        HAL_TIM_IC_Init(&htim[i]);

        HAL_TIM_PWM_Init(&htim[i]);

        HAL_TIMEx_MasterConfigSynchronization(&htim[i], &sMasterConfig);
 
        HAL_TIM_IC_ConfigChannel(&htim[i], &sConfigIC, TIM_CHANNEL_1);
        HAL_TIM_IC_ConfigChannel(&htim[i], &sConfigIC, TIM_CHANNEL_2);
        HAL_TIM_IC_ConfigChannel(&htim[i], &sConfigIC, TIM_CHANNEL_3);

        HAL_TIM_PWM_ConfigChannel(&htim[i], &sConfigOC, TIM_CHANNEL_4);

        // HAL_TIM_Base_Start(&htim[i]);

        HAL_TIM_PWM_Start(&htim[i], TIM_CHANNEL_4);
    }

}

void setPWM(TimerPWMOutput timerOutput, uint8_t dutyCycle) {
    __HAL_TIM_SET_COMPARE(&htim[timerOutput], TIM_CHANNEL_4, TIM_PERIOD/dutyCycle);
}


