#include "Utils.h"
#define arg_width 32
#define arg_count 8
// #include <stdio.h>
static bool static_cmd_flag_initilized_ = false;
static char static_cmd_receive_buf_[100] = {}; 
static char static_cmd_analized_buf_[arg_count][arg_width] = {};

void cmd_analisis_(void);
	
void cmd_capture(void)
{
	if (ptrUSART -> Receive(static_cmd_receive_buf_, 100) == ARM_DRIVER_OK)		// 若无监听事件 则 开始监听 并 占用端口
		memset(static_cmd_receive_buf_, 0, 100);
	else if (strchr(static_cmd_receive_buf_, '\n') != NULL)						// 接收到完整命令
	{
		Driver_USART1.Control(ARM_USART_ABORT_RECEIVE, 0);						// 释放端口
		cmd_analisis_();														// 解析命令并写入内存
//		for(int i = 0; i < arg_count; i++)
//			printf("[%s], ", *(static_cmd_analized_buf_ + i));
//		printf("\n");
	}
}

void cmd_analisis_(void)
{
    int bufsize = strlen(static_cmd_receive_buf_) * 2;
    char *__cmd_src_opt_buf = (char*) malloc(sizeof(char) * bufsize);
    char *cptr_anchor = NULL;
	for (int i = 0; i < arg_count; i++)
		memset(*(static_cmd_analized_buf_ + i), 0, arg_width);
	
    strcpy(__cmd_src_opt_buf, static_cmd_receive_buf_);
	int index;
    for (index = 0; (cptr_anchor = strchr(__cmd_src_opt_buf, ' ')); index ++)
    {
        strncpy(*(static_cmd_analized_buf_ + index), __cmd_src_opt_buf, cptr_anchor - __cmd_src_opt_buf);
        strcpy(__cmd_src_opt_buf, cptr_anchor + 1);
    }
    strncpy(*(static_cmd_analized_buf_ + index), __cmd_src_opt_buf, strchr(__cmd_src_opt_buf, '\n') - __cmd_src_opt_buf);

    free(__cmd_src_opt_buf);
}

int cmd_field_value(uint8_t field_index)
{
	return strtol(*(static_cmd_analized_buf_ + field_index), NULL, 10);
}

int cmd_field_rgb_value(uint8_t field_index)
{
	if (strlen(*(static_cmd_analized_buf_ + field_index)) != 6)
		return 0;
	else
		return strtol(*(static_cmd_analized_buf_ + field_index), NULL, 16);
}

bool cmd_field_check(uint8_t field_index, const char* str)
{
//	printf("[STR1][%s], [STR2][%s]\n", *(static_cmd_analized_buf_ + field_index), str);
//	printf("%d\n", strcmp(*(static_cmd_analized_buf_ + field_index), str));
	if (strcmp(*(static_cmd_analized_buf_ + field_index), str) == 0 ) // || strcmp(*(static_cmd_analized_buf_ + field_index), str) == 13)
		return true;
	else
		return false;
}

void cmd_head_clear(void)
{
	memset(*(static_cmd_analized_buf_), 0, arg_width);
}

void delay_ms(uint16_t nms)
{
	if (nms)
	{
		u32 temp;
		SysTick -> LOAD = 9000 * nms;
		SysTick -> VAL = 0X00; 							//清空计数器
		SysTick -> CTRL = 0X01; 						//使能，减到零是无动作，采用外部时钟源
		do
			temp = SysTick -> CTRL;						//读取当前倒计数值
		while((temp & 0x01) && !(temp & (1 << 16)));	//等待时间到达
		SysTick -> CTRL = 0x00; 						//关闭计数器
		SysTick -> VAL = 0X00; 							//清空计数器
	}
}
