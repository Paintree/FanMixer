#ifndef __GPIO_H
#define __GPIO_H

#include <stm32f1xx_hal.h>

#define GPIO_STATUS_LED_PORT            GPIOC
#define GPIO_STATUS_LED_PIN             GPIO_PIN_13
#define GPIO_STATUS_LED_MODE            GPIO_MODE_OUTPUT_PP
#define GPIO_STATUS_LED_PULL            GPIO_NOPULL
#define GPIO_STATUS_LED_SPEED           GPIO_SPEED_FREQ_LOW
#define GPIO_STATUS_LED_CLK_ENABLE      __HAL_RCC_GPIOC_CLK_ENABLE

#define FAN_INPUT_1_PORT                GPIOA
#define FAN_INPUT_1_PIN                 GPIO_PIN_0
#define FAN_INPUT_1_MODE                GPIO_MODE_INPUT
#define FAN_INPUT_1_PULL                GPIO_NOPULL
#define FAN_INPUT_1_SPEED               GPIO_SPEED_FREQ_LOW
#define FAN_INPUT_1_CLK_ENABLE          __HAL_RCC_GPIOA_CLK_ENABLE

#define FAN_INPUT_2_PORT                GPIOA
#define FAN_INPUT_2_PIN                 GPIO_PIN_6
#define FAN_INPUT_2_MODE                GPIO_MODE_INPUT
#define FAN_INPUT_2_PULL                GPIO_NOPULL
#define FAN_INPUT_2_SPEED               GPIO_SPEED_FREQ_LOW
#define FAN_INPUT_2_CLK_ENABLE          __HAL_RCC_GPIOA_CLK_ENABLE

#define FAN_OUTPUT_1_PORT               GPIOA
#define FAN_OUTPUT_1_PIN                GPIO_PIN_8
#define FAN_OUTPUT_1_MODE               GPIO_MODE_AF_PP
#define FAN_OUTPUT_1_PULL               GPIO_NOPULL
#define FAN_OUTPUT_1_SPEED              GPIO_SPEED_FREQ_LOW
#define FAN_OUTPUT_1_CLK_ENABLE         __HAL_RCC_GPIOA_CLK_ENABLE

typedef struct {
    GPIO_TypeDef *port;
    uint32_t pin;
} gpio_t;


void gpioInit();
void gpioEnable(gpio_t output);
void gpioDisable(gpio_t output);
void gpioToggle(gpio_t output);
void gpioTimersPWMInit();


#endif // __GPIO_H