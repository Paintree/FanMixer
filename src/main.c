#include <stm32f1xx_hal.h>


static void systemClockInit();

int main() {
    HAL_Init();
    systemClockInit();
    
    while(1) {
    }
}

void systemClockInit()
{

}

// Uncomment when using arm-gcc-none-eabi above 10.3
void _close_r(void) {}
void _lseek_r(void) {}
void _read_r(void) {}
void _write_r(void) {}