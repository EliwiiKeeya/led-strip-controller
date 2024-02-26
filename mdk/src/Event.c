#include "Event.h"
#define FLASH_SIZE 64
#define SECTOR_SIZE 1024

bool flag_breath = false;
bool flag_default = true;
bool flag_meteor = false;
bool flag_rainbow = false;
bool flag_rainbow_flow = false;
unsigned int val_color = 0;
uint8_t val_len = 0;
uint8_t val_interval = 0;
uint8_t val_wait = 0;

void flag_init(void)
{
	uint16_t data_read_buf[16] = {};
	flash_read_multi(FLASH_BASE + SECTOR_SIZE * 63, data_read_buf, 16);
	flag_breath 		= 	data_read_buf[0];
	flag_default		=	data_read_buf[1];
	flag_meteor			=	data_read_buf[2];
	flag_rainbow 		= 	data_read_buf[3];
	flag_rainbow_flow 	= 	data_read_buf[4];
	val_color 		   |= 	data_read_buf[5] << 16;
	val_color		   |=	data_read_buf[6];
	val_len 			= 	data_read_buf[7];
	val_interval 		= 	data_read_buf[8];
	val_wait 			= 	data_read_buf[9];
}

void flag_reset_all(void)
{
	flag_breath = false;
	flag_default = false;
	flag_meteor = false;
	flag_rainbow = false;
	flag_rainbow_flow = false;
}

void flag_storage(void)
{
	uint16_t data_write_buf[16] = {};
	data_write_buf[0] = flag_breath;
	data_write_buf[1] = flag_default;
	data_write_buf[2] = flag_meteor;
	data_write_buf[3] = flag_rainbow;
	data_write_buf[4] = flag_rainbow_flow;
	data_write_buf[5] = val_color >> 16;
	data_write_buf[6] = val_color << 16 >> 16;
	data_write_buf[7] = val_len;
	data_write_buf[8] = val_interval;
	data_write_buf[9] = val_wait;
	flash_write_multi(FLASH_BASE + SECTOR_SIZE * 63, data_write_buf, 16);
}

uint32_t flash_read(uint32_t address)
{
	uint32_t tmp1, tmp2;
	tmp1 = *(__IO uint16_t*) address; 
	tmp2 = *(__IO uint16_t*) (address + 2); 
	return (tmp2 << 16) & tmp1;
}

uint16_t flash_read_half(uint32_t address)
{
	return *(__IO uint16_t*)address; 
}


void flash_read_multi(uint32_t address_start, uint16_t *data_read, uint16_t length_data_read)
{
	for(uint16_t index = 0; index < length_data_read; index++)
		*(data_read + index) = flash_read_half(address_start + index * 2);
}

void flash_write_multi(uint32_t address_start, uint16_t *data_write, uint16_t length_data_write)
{
	if(address_start < FLASH_BASE || ((address_start + length_data_write * 2) >= (FLASH_BASE + SECTOR_SIZE * FLASH_SIZE)))
		return;																	// 非法地址
	FLASH_Unlock();         													// 写保护解锁
	uint32_t address_offset = address_start - FLASH_BASE;               		// 计算去掉 0X08000000 后的实际偏移地址
	uint32_t index_sector = address_offset / SECTOR_SIZE;            			// 计算扇区序号 [0, 255]
	uint32_t address_sector = index_sector * SECTOR_SIZE + FLASH_BASE;    		// 对应扇区的首地址
	FLASH_ErasePage(address_sector);											// 擦除这个扇区
	for(uint16_t index=0; index < length_data_write; index++)
		FLASH_ProgramHalfWord(address_start + index * 2, *(data_write + index));
	FLASH_Lock();																// 写保护锁定
}
