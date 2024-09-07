#include <stm32f1xx_hal.h>
#include <FreeRTOS.h>
#include <task.h>

#include <gpio.h>
#include <timer.h>
#include <fan.h>


static void systemClockInit();

void status_task() {
    __HAL_RCC_CLEAR_RESET_FLAGS();

    IWDG_HandleTypeDef hIWDG = {
        .Instance           = IWDG,
        .Init.Prescaler     = IWDG_PRESCALER_64,
        .Init.Reload        = 2000,
    };
    HAL_IWDG_Init(&hIWDG);

    const gpio_t statusLed = {
        .port = GPIO_STATUS_LED_PORT,
        .pin  = GPIO_STATUS_LED_PIN,
    };
    
    while(1) {
        HAL_IWDG_Refresh(&hIWDG);
        gpioToggle(statusLed);

        vTaskDelay(100 / portTICK_PERIOD_MS);
    }
}

void fan_task() {
    fanInit();
    vTaskDelay(100 / portTICK_PERIOD_MS);

    while(1) {
        fanStep();
        vTaskDelay(100 / portTICK_PERIOD_MS);
    }
}

void timer_task() {

    while(1) {
        timerStep();
        vTaskDelay(1000 / portTICK_PERIOD_MS);
    }
}

int main() {
    HAL_Init();
    systemClockInit();
    gpioInit();
    timerInit();

    xTaskCreate(status_task, "status_task", 128, NULL, 0, NULL);
    xTaskCreate(fan_task, "fan_task", 128, NULL, 1, NULL);
    xTaskCreate(timer_task, "timer_task", 128, NULL, 2, NULL);

    vTaskStartScheduler();

    while(1) {
    }
}

void systemClockInit() {
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.HSEPredivValue = RCC_HSE_PREDIV_DIV1;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLMUL = RCC_PLL_MUL9;
  HAL_RCC_OscConfig(&RCC_OscInitStruct);

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;
  HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2);

}

void HAL_MspInit(void) {

  __HAL_RCC_AFIO_CLK_ENABLE();
  __HAL_RCC_PWR_CLK_ENABLE();

  HAL_NVIC_SetPriority(PendSV_IRQn, 15, 0);

  __HAL_AFIO_REMAP_SWJ_NOJTAG();

}
