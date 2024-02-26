#ifndef __USART_H__
#define __USART_H__

#include "stm32f10x.h"                  // Device header
#include "Driver_USART.h"               // ::CMSIS Driver:USART
#define USART_DRV_NUM           1
#define USART_BAUDRATE          38400
#define _USART_Driver_(n)  Driver_USART##n
#define  USART_Driver_(n) _USART_Driver_(n)
extern ARM_DRIVER_USART  USART_Driver_(USART_DRV_NUM);
#define ptrUSART       (&USART_Driver_(USART_DRV_NUM))

int hc06_init(void);
int stdin_getchar(void);
int stdout_putchar(int ch);

#endif
