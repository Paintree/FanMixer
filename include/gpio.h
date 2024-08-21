#ifndef __GPIO_H
#define __GPIO_H

#include <stm32f1xx_hal.h>

#define GPIO_STATUS_LED_PORT            GPIOC
#define GPIO_STATUS_LED_PIN             GPIO_PIN_13
#define GPIO_STATUS_LED_MODE            GPIO_MODE_OUTPUT_PP
#define GPIO_STATUS_LED_PULL            GPIO_NOPULL
#define GPIO_STATUS_LED_SPEED           GPIO_SPEED_FREQ_LOW
#define GPIO_STATUS_LED_CLK_ENABLE      __HAL_RCC_GPIOC_CLK_ENABLE

typedef struct {
    GPIO_TypeDef *port;
    uint32_t pin;
} gpio_t;




void gpioInit();
void gpioEnable(gpio_t output);
void gpioDisable(gpio_t output);
void gpioToggle(gpio_t output);


#endif // __GPIO_H