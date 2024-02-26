#ifndef __UART_H__
#define __UART_H__

#include <stdbool.h>
#include "stm32f10x.h"              				// Device header
#include "Utils.h"

void debug_idicator_light_init(void);
void debug_idicator_light_level_set(bool Flag);
void debug_idicator_light_flash(uint16_t nms, uint16_t times);

#endif
