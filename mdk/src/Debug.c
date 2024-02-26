#include "Debug.h"

void debug_idicator_light_init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;				// GPIOx 初始化结构体
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE); // GPIOx 系统时钟使能
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13;		  // 选中引脚
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; // 最高输出速率50MHz
	GPIO_InitStructure.GPIO_Mode =  GPIO_Mode_Out_PP; // 推挽输出
	GPIO_Init(GPIOC, &GPIO_InitStructure);			  // GPIOx 寄存器初始化
}

void debug_idicator_light_level_set(bool Flag)
{
	if (Flag)
		GPIO_ResetBits(GPIOC, GPIO_Pin_13);
	else
		GPIO_SetBits(GPIOC, GPIO_Pin_13);
}

void debug_idicator_light_flash(uint16_t nms, uint16_t times)
{
	uint16_t freq = nms / 2;
	for (int i = 0; i < times; i++)
	{
		GPIO_ResetBits(GPIOC, GPIO_Pin_13);
		delay_ms(freq);
		GPIO_SetBits(GPIOC, GPIO_Pin_13);
		delay_ms(freq);
	}
}
