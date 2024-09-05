#ifndef __FAN_H
#define __FAN_H

#include <stdint.h>

void fanInit();
uint8_t fanCalculateOutputSpeed(uint8_t fan1Speed, uint8_t fan2Speed);
void fanStep();

#endif // __FAN_H