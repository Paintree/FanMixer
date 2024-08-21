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

void gpioInit() {
    gpioLedInit();

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