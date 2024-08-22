#include <gpio.h>

void gpioLedInit() {
    GPIO_STATUS_LED_CLK_ENABLE();

    GPIO_InitTypeDef GPIO_InitStruct = {0};
    GPIO_InitStruct.Mode = GPIO_STATUS_LED_MODE;
    GPIO_InitStruct.Pull = GPIO_STATUS_LED_PULL;
    GPIO_InitStruct.Speed = GPIO_STATUS_LED_SPEED;
    GPIO_InitStruct.Pin = GPIO_STATUS_LED_PIN;
    HAL_GPIO_Init(GPIO_STATUS_LED_PORT, &GPIO_InitStruct);
}

void gpioTimersICInit() {
    GPIO_InitTypeDef GPIO_InitStruct = {0};

    FAN_INPUT_1_CLK_ENABLE();
    FAN_INPUT_2_CLK_ENABLE();
    FAN_INPUT_3_CLK_ENABLE();

    GPIO_InitStruct.Pin     = FAN_INPUT_1_PIN;
    GPIO_InitStruct.Pull    = FAN_INPUT_1_PULL;
    GPIO_InitStruct.Mode    = FAN_INPUT_1_MODE;
    GPIO_InitStruct.Speed   = FAN_INPUT_1_SPEED;
    HAL_GPIO_Init(FAN_INPUT_1_PORT, &GPIO_InitStruct);

    GPIO_InitStruct.Pin     = FAN_INPUT_2_PIN;
    GPIO_InitStruct.Pull    = FAN_INPUT_2_PULL;
    GPIO_InitStruct.Mode    = FAN_INPUT_2_MODE;
    GPIO_InitStruct.Speed   = FAN_INPUT_2_SPEED;
    HAL_GPIO_Init(FAN_INPUT_2_PORT, &GPIO_InitStruct);

    GPIO_InitStruct.Pin     = FAN_INPUT_3_PIN;
    GPIO_InitStruct.Pull    = FAN_INPUT_3_PULL;
    GPIO_InitStruct.Mode    = FAN_INPUT_3_MODE;
    GPIO_InitStruct.Speed   = FAN_INPUT_3_SPEED;
    HAL_GPIO_Init(FAN_INPUT_3_PORT, &GPIO_InitStruct);

    __HAL_AFIO_REMAP_TIM2_PARTIAL_2();
}

void gpioTimersPWMInit() {
    GPIO_InitTypeDef GPIO_InitStruct = {0};

    FAN_OUTPUT_1_CLK_ENABLE();

    GPIO_InitStruct.Pin     = FAN_OUTPUT_1_PIN;
    GPIO_InitStruct.Pull    = FAN_OUTPUT_1_PULL;
    GPIO_InitStruct.Mode    = FAN_OUTPUT_1_MODE;
    GPIO_InitStruct.Speed   = FAN_OUTPUT_1_SPEED;
    HAL_GPIO_Init(FAN_OUTPUT_1_PORT, &GPIO_InitStruct);

    __HAL_AFIO_REMAP_TIM2_PARTIAL_2();
}

void gpioInit() {
    gpioLedInit();
    gpioTimersPWMInit();
    gpioTimersICInit();
}

void gpioEnable(gpio_t output) {
    HAL_GPIO_WritePin(output.port, output.pin, GPIO_PIN_SET);
}

void gpioDisable(gpio_t output) {
    HAL_GPIO_WritePin(output.port, output.pin, GPIO_PIN_RESET);

}

void gpioToggle(gpio_t output) {
    HAL_GPIO_TogglePin(output.port, output.pin);
}