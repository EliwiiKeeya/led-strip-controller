#include "Driver.h"

static uint16_t static_pixel_buf[PIXEL_NUM][GRB];
static const uint16_t static_table_breath_wave[] = {
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 2, 2, 2,
	2, 2, 2, 2, 2, 2, 3, 3, 3, 3, 3, 3, 3, 4, 4, 4, 4, 4, 5, 5, 5, 5, 5, 6, 6, 6, 6, 7,
	7, 7, 8, 8, 8, 9, 9, 9, 10, 10, 11, 11, 12, 12, 13, 13, 14, 14, 15, 15, 16, 17, 17,
	18, 19, 19, 20, 21, 22, 23, 24, 24, 25, 26, 27, 29, 30, 31, 32, 33, 35, 36, 37, 39,
	40, 42, 44, 45, 47, 49, 51, 53, 55, 57, 59, 61, 64, 66, 69, 71, 74, 77, 80, 83, 86,
	89, 93, 96, 100, 104, 108, 112, 116, 121, 125, 130, 135, 140, 145, 151, 157, 163, 169,
	175, 182, 189, 196, 204, 212, 220, 228, 237, 246, 255, 255, 246, 237, 228, 220, 212, 204,
	196, 189, 182, 175, 169, 163, 157, 151, 145, 140, 135, 130, 125, 121, 116, 112, 108, 104,
	100, 96, 93, 89, 86, 83, 80, 77, 74, 71, 69, 66, 64, 61, 59, 57, 55, 53, 51, 49, 47,
	45, 44, 42, 40, 39, 37, 36, 35, 33, 32, 31, 30, 29, 27, 26, 25, 24, 24, 23, 22, 21,
	20, 19, 19, 18, 17, 17, 16, 15, 15, 14, 14, 13, 13, 12, 12, 11, 11, 10, 10, 9, 9, 9,
	8, 8, 8, 7, 7, 7, 6, 6, 6, 6, 5, 5, 5, 5, 5, 4, 4, 4, 4, 4, 3, 3, 3, 3, 3, 3, 3, 2, 2,
	2, 2, 2, 2, 2, 2, 2, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
};

void ws2812b_init(void)
{
	// 定义外设初始化结构体
	DMA_InitTypeDef DMA_InitStructure;					// DMAx 初始化结构体
	GPIO_InitTypeDef GPIO_InitStructure;				// GPIOx 初始化结构体
	TIM_TimeBaseInitTypeDef TIM_TimeBase_InitStructure; // TIMx 时间基数单位初始化结构体
	TIM_OCInitTypeDef TIM_OC_InitStructure;				// TIM_OCx 初始化结构体

	// 系统时钟使能
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);	  // DMAx 系统时钟使能
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);  // TIMx 系统时钟使能
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE); // GPIOx 系统时钟使能

	// 功能输入输出配置
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;		  // 选中引脚
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; // 最高输出速率50MHz
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;	  // 复用推挽输出

	// 通用定时器配置
	TIM_TimeBase_InitStructure.TIM_Prescaler = 0;	 					// TIMx_PSC 预分频值
	TIM_TimeBase_InitStructure.TIM_CounterMode = TIM_CounterMode_Up; 	// 向上计数模式
	TIM_TimeBase_InitStructure.TIM_Period = 90 - 1;					 	// TIMx_ARR 自动加载值
	TIM_TimeBase_InitStructure.TIM_ClockDivision = TIM_CKD_DIV1;

	// 通用定时器预装载寄存器配置
	TIM_OC_InitStructure.TIM_OCMode = TIM_OCMode_PWM1;			   // 计数器 PWM 模式
	TIM_OC_InitStructure.TIM_OutputState = TIM_OutputState_Enable; // 比较输出使能
	TIM_OC_InitStructure.TIM_Pulse = 0;							   // 占空比
	TIM_OC_InitStructure.TIM_OCPolarity = TIM_OCPolarity_High;	   // 定义高极性为有效极性
	TIM_OC_InitStructure.TIM_OCIdleState = TIM_OCIdleState_Reset;  // 空闲状态输出低电平

	// DMA 配置
	DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t) & (TIM2 -> CCR1);		// 定义 CCRx 为外设基地址
	DMA_InitStructure.DMA_MemoryBaseAddr = (uint32_t) static_pixel_buf;			// 定义 内存基地址
	DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralDST;							// 定义外设作为数据传输的目的地
	DMA_InitStructure.DMA_BufferSize = PIXEL_NUM * GRB;							// 定义指定 DMA 通道的 DMA 缓存的大小
	DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;			// 外设地址寄存器不递增
	DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;						// 内存地址寄存器递增
	DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord; // 外设数据宽度为 16 位
	DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord;			// 存储器数据宽度为 16 位
	DMA_InitStructure.DMA_Mode = DMA_Mode_Normal;								// 工作在正常缓存模式
	DMA_InitStructure.DMA_Priority = DMA_Priority_Medium;						// DMA 通道 x 拥有中优先级
	DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;								// DMA 通道 x 未设置为内存到内存传输

	// 初始化
	DMA_Init(DMA1_Channel5, &DMA_InitStructure);		 // DMAx 初始化
	GPIO_Init(GPIOA, &GPIO_InitStructure);				 // GPIOx 寄存器初始化
	TIM_TimeBaseInit(TIM2, &TIM_TimeBase_InitStructure); // TIMx 时间基数单位初始化
	TIM_OC1Init(TIM2, &TIM_OC_InitStructure);			 // TIMx_OCx 初始化

	// 使能 / 失能
	TIM_CtrlPWMOutputs(TIM2, ENABLE);				  // TIMx_OCx 使能
	TIM_OC1PreloadConfig(TIM2, TIM_OCPreload_Enable); // TIMx_CCRx 使能
	TIM_Cmd(TIM2, DISABLE);							  // TIMx 失能
	TIM_DMACmd(TIM2, TIM_DMA_CC1, ENABLE);			  // TIMx_CHx DMA 请求使能

	DMA_Cmd(DMA1_Channel5, DISABLE); // DMAx 失能

	ws2812b_close_all();
	delay_ms(100);
}

uint32_t ws2812b_color(uint8_t red, uint8_t green, uint8_t blue)
{
	return green << 16 | red << 8 | blue;
}

// Input a value 0 to 255 to get a color value.
// The colours are a transition r - g - b - back to r.
uint32_t ws2812b_wheel(uint8_t wheelPos)
{
	wheelPos = 255 - wheelPos;
	if (wheelPos < 85)
		return ws2812b_color(255 - wheelPos * 3, 0, wheelPos * 3);
	if (wheelPos < 170)
	{
		wheelPos -= 85;
		return ws2812b_color(0, wheelPos * 3, 255 - wheelPos * 3);
	}
	wheelPos -= 170;
	return ws2812b_color(wheelPos * 3, 255 - wheelPos * 3, 0);
}

// Fill the dots one after the other with a color     
void ws2812b_color_wipe(uint32_t GRBcolor, uint8_t wait)
{
	for (uint16_t i = 0; i < PIXEL_NUM; i++)
		ws2812b_set_pixel_color(i, GRBcolor);
	ws2812b_show();
	delay_ms(wait);
}

void ws2812b_set_pixel_color(uint16_t pixel_num, uint32_t GRBColor)
{
	if (pixel_num < PIXEL_NUM)
		for (uint8_t i = 0; i < GRB; i++)
			static_pixel_buf[pixel_num][i] = ((GRBColor << i) & 0x800000) ? WS_HIGH : WS_LOW;
}

void ws2812b_show(void)
{
	DMA_SetCurrDataCounter(DMA1_Channel5, PIXEL_NUM * GRB); // 设置写入 CCRx 的数据长度
	TIM_Cmd(TIM2, ENABLE);									// TIMx 使能
	DMA_Cmd(DMA1_Channel5, ENABLE);							// DMAx 使能
	while (DMA_GetFlagStatus(DMA1_FLAG_TC5) != SET);		// 等待数据写入完成
	TIM_Cmd(TIM2, DISABLE); 								// TIMx 失能
	TIM_SetCompare1(TIM2, 0);
	DMA_Cmd(DMA1_Channel5, DISABLE); 						// DMAx 失能

	DMA_ClearFlag(DMA1_FLAG_TC5); 							// TCIFx 复位
}

void ws2812b_close_all(void)
{
	uint16_t i;
	uint8_t j;

	for (i = 0; i < PIXEL_NUM; ++i)
		for (j = 0; j < 24; ++j)
			static_pixel_buf[i][j] = WS_LOW;
	ws2812b_show();
}

void ws2812b_rainbow(uint8_t wait)
{
	for (uint8_t i = 0; i <= 255; i++)
	{
		for (uint8_t j = 0; j < PIXEL_NUM; j++)
			ws2812b_set_pixel_color(j, ws2812b_wheel((j + i) & 255));
		ws2812b_show();
		delay_ms(wait);
	}
}

void ws2812b_meteor(uint32_t color, uint8_t len, uint8_t interval, uint8_t wait)
{
	uint8_t red = color >> 16, green = color << 16 >> 24, blue = color << 24 >> 24;
	float hue, saturation, value;
	ws2812b_rgb_to_hsv(red, green, blue, &hue, &saturation, &value);
	uint8_t shade_offset = 150 / len;
	uint8_t shade_begin = 150 % shade_offset;
	for (int i = -(len + interval); i < 0; i++)
		for (int j = i; j < PIXEL_NUM; j += len + interval)
		{
			for (uint8_t k = 0; k < len && j + k < PIXEL_NUM; k++)
				ws2812b_set_pixel_color(j + k, ws2812b_hsv_to_rgb(hue, saturation, static_table_breath_wave[shade_begin + shade_offset * k]));
			for (uint8_t k = len; k < len + interval && j + k < PIXEL_NUM; k++)
				ws2812b_set_pixel_color(j + k, 0);
			ws2812b_show();
			delay_ms(wait);
		}
}

void ws2812b_rainbow_with_static(uint8_t wait)
{
	static uint8_t i;
	for (uint8_t j = 0; j < PIXEL_NUM; j++)
		ws2812b_set_pixel_color(j, ws2812b_wheel((j + i) & 255));
	if (i == 255)
		i = 0;
	else
		i++;
	
	ws2812b_show();
	delay_ms(wait);
}

// Slightly different, this makes the rainbow equally distributed throughout
void ws2812b_rainbow_cycle(uint8_t wait)
{
	for (uint16_t i = 0; i < 256; i++)
	{
		for (uint8_t j = 0; j < PIXEL_NUM; j++)
			ws2812b_set_pixel_color(j, ws2812b_wheel(((j * 256 / PIXEL_NUM) + i) & 255));
		ws2812b_show();
		delay_ms(wait);
	}
}

// Theatre-style crawling lights.
void ws2812b_theater_chase(uint32_t color, uint8_t wait)
{
	for (int i = 0; i < 10; i++) // do 10 cycles of chasing
		for (int j = 0; j < 3; j++)
		{
			for (uint8_t k = 0; k < PIXEL_NUM; k += 3)
				ws2812b_set_pixel_color(k + j, color); // turn every third pixel on
			ws2812b_show();
			delay_ms(wait);
			for (uint16_t k = 0; k < PIXEL_NUM; k += 3)
				ws2812b_set_pixel_color(k + j, 0); // turn every third pixel off
		}
}

// Theatre-style crawling lights with rainbow effect
void ws2812b_theater_chase_rainbow(uint8_t wait, uint8_t interval)
{
	for (uint8_t i = 0; i <= 255; i++) // cycle all 256 colors in the wheel
		for (uint8_t j = 0; j < interval; j++)
		{
			for (uint8_t k = 0; k < PIXEL_NUM; k += 3)
				ws2812b_set_pixel_color(k + j, ws2812b_wheel((i + j) % 255)); // turn every third pixel on
			ws2812b_show();
			delay_ms(wait);
			for (uint8_t k = 0; k < PIXEL_NUM; k += 3)
				ws2812b_set_pixel_color(k + j, 0); // turn every third pixel off
		}
}

static float static_min(float a, float b, float c)
{
  float m;
  
  m = a < b ? a : b;
  return (m < c ? m : c); 
}

static float static_max(float a, float b, float c)
{
  float m;
  
  m = a > b ? a : b;
  return (m > c ? m : c); 
}
 
// H: [0, 360] S[0%, 100%], V[0, 255]
void ws2812b_rgb_to_hsv(uint8_t red, uint8_t green, uint8_t blue, float *hue, float *saturation, float *value)
{
	uint8_t rgb_max = static_max(red, green, blue);
	uint8_t rgb_min = static_min(red, green, blue);
	float delta = rgb_max - rgb_min;

	if (rgb_max == rgb_min) 					*hue = 0;
	else if (rgb_max == red && green >= blue)	*hue = 60 * (green - blue) / delta;
	else if (rgb_max == red && green < blue) 	*hue = 60 * (green - blue) / delta + 360;
	else if (rgb_max == green) 					*hue = 60 * (blue - red)   / delta + 120;
	else if (rgb_max == blue)					*hue = 60 * (red - green)  / delta + 240;
    if (rgb_max == 0)                           *saturation = 0;
    else                                        *saturation = 1 - (float)rgb_min / rgb_max;
	*value = rgb_max;
}
  
uint32_t ws2812b_hsv_to_rgb(float hue, float saturation, float value)
{
	uint16_t h_i = (int)(hue / 60) % 6;
	float f = hue / 60 - h_i;
	float p = value * (1 - saturation);
	float q = value * (1 - f * saturation);
	float t = value * (1 - (1 - f) * saturation);
	switch (h_i)
	{
		case 0: return ws2812b_color(value, t, p); break;
		case 1: return ws2812b_color(q, value, p); break;
		case 2: return ws2812b_color(p, value, t); break;
		case 3: return ws2812b_color(p, q, value); break;
		case 4: return ws2812b_color(t, p, value); break;
		case 5: return ws2812b_color(value, p, q); break;
		default: return ERROR;
    }
}

void ws2812b_breath(uint16_t pixel_num, uint8_t red, uint8_t green, uint8_t blue, uint8_t wait)
{
	float hue, saturation, value;
	ws2812b_rgb_to_hsv(red, green, blue, &hue, &saturation, &value);
	for (int i = 80; i < 220; i++) // iterate the table of breath wave.
	{
		for(int j = 0; j < PIXEL_NUM; j++)
			ws2812b_set_pixel_color(j, ws2812b_hsv_to_rgb(hue, saturation, static_table_breath_wave[i]));
		ws2812b_show();
		delay_ms(wait);
	}
}

uint32_t ws2812b_rgb_to_grb(uint32_t val_rgb)
{
    uint16_t val_r = val_rgb >> 16;
    uint16_t val_g = val_rgb << 16 >> 24;
    uint16_t val_b = val_rgb << 24 >> 24;
    return val_g << 16 | val_r << 8 | val_b;
}
