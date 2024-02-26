#ifndef __DEVICE_H__
#define __DEVICE_H__

#include <math.h>
#include "stm32f10x.h"              // Device header
#include "Utils.h"

#define PIXEL_NUM  36				// 灯珠个数
#define GRB  24   					// 3 * 8

#define WS_HIGH 45					// 1 码占空比
#define WS_LOW  30					// 0 码占空比
#define WS_RESET 0					// Reset 码占空比

void ws2812b_init(void);												// 初始化
uint32_t ws2812b_color(uint8_t red, uint8_t green, uint8_t blue);		// 返回预高位先发，按照GRB顺序的预发送数据
uint32_t ws2812b_wheel(uint8_t wheelPos);
void ws2812b_color_wipe(uint32_t GRBcolor, uint8_t wait);				// 填充内存中的数据
void ws2812b_set_pixel_color(uint16_t pixel_num, uint32_t GRBColor);	// 写入数据到内存
void ws2812b_show(void);												// 应用内存数据 (DMA)
void ws2812b_close_all(void);
void ws2812b_meteor(uint32_t color, uint8_t len, uint8_t interval, uint8_t wait);
void ws2812b_rainbow(uint8_t wait);
void ws2812b_rainbow_with_static(uint8_t wait);
void ws2812b_rainbow_cycle(uint8_t wait);			
void ws2812b_theater_chase(uint32_t c, uint8_t wait);					// Theatre-style crawling lights.
void ws2812b_theater_chase_rainbow(uint8_t wait, uint8_t interval);
void ws2812b_rgb_to_hsv(uint8_t red, uint8_t green, uint8_t blue, float *hue, float *saturation, float *value);
uint32_t ws2812b_hsv_to_rgb(float hue, float saturation, float value);
void ws2812b_breath(uint16_t pixel_num, uint8_t red, uint8_t green, uint8_t blue, uint8_t wait);
uint32_t ws2812b_rgb_to_grb(uint32_t val_rgb);

#endif
