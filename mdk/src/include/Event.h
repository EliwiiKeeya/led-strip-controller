#ifndef __EVENT_H__
#define __EVENT_H__

#include <stdbool.h>
#include "stm32f10x.h"                  // Device header

extern bool flag_breath;
extern bool flag_default;
extern bool flag_meteor;
extern bool flag_rainbow;
extern bool flag_rainbow_flow;
extern unsigned int val_color;
extern uint8_t val_len;
extern uint8_t val_interval;
extern uint8_t val_wait;

void flag_init(void);
void flag_reset_all(void);
void flag_storage(void);
uint32_t flash_read(uint32_t address);
uint16_t flash_read_half(uint32_t address);
void flash_read_multi(uint32_t address_start, uint16_t *data_read, uint16_t countToRead);
void flash_write_multi(uint32_t address_start, uint16_t *data_write, uint16_t length_data_write);

#endif
