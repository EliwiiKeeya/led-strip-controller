#ifndef __UTILS_H__
#define __UTILS_H__

#include <errno.h>
#include <stdlib.h>
#include <string.h>
#include "stm32f10x.h"                  // Device header
#include "USART.h"

void cmd_capture(void);
int cmd_field_value(uint8_t field_index);
int cmd_field_rgb_value(uint8_t field_index);
bool cmd_field_check(uint8_t field_index, const char* str);
void cmd_head_clear(void);
void delay_ms(uint16_t nms);

#endif
