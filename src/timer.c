#include <timer.h>
#include <stm32f1xx_hal.h>

#define TIM2_CLK_ENABLE     __HAL_RCC_TIM2_CLK_ENABLE

TIM_HandleTypeDef htim = {
    .Instance = TIM2,
    .Init.Prescaler = 0,
    .Init.CounterMode = TIM_COUNTERMODE_UP,
    .Init.Period = 65535,
    .Init.ClockDivision = TIM_CLOCKDIVISION_DIV1,
    .Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE,
}; 

void timerInit() {
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
        .Pulse = 0,
        .OCPolarity = TIM_OCPOLARITY_HIGH,
        .OCFastMode = TIM_OCFAST_DISABLE,
    };

    TIM2_CLK_ENABLE();

    HAL_TIM_IC_Init(&htim);

    HAL_TIM_PWM_Init(&htim);

    HAL_TIMEx_MasterConfigSynchronization(&htim, &sMasterConfig);

    HAL_TIM_IC_ConfigChannel(&htim, &sConfigIC, TIM_CHANNEL_1);
    HAL_TIM_IC_ConfigChannel(&htim, &sConfigIC, TIM_CHANNEL_2);
    HAL_TIM_IC_ConfigChannel(&htim, &sConfigIC, TIM_CHANNEL_3);

    HAL_TIM_PWM_ConfigChannel(&htim, &sConfigOC, TIM_CHANNEL_4);
}