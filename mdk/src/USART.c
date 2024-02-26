#include "USART.h"

int hc06_init(void)
{
	int32_t status;
 
	status = ptrUSART -> Initialize(NULL);
	if (status != ARM_DRIVER_OK) return (-1);

	status = ptrUSART -> PowerControl(ARM_POWER_FULL);
	if (status != ARM_DRIVER_OK) return (-1);

	status = ptrUSART -> Control(ARM_USART_MODE_ASYNCHRONOUS |
							 ARM_USART_DATA_BITS_8       |
							 ARM_USART_PARITY_NONE       |
							 ARM_USART_STOP_BITS_1       |
							 ARM_USART_FLOW_CONTROL_NONE,
							 USART_BAUDRATE);
	if (status != ARM_DRIVER_OK) return (-1);

	status = ptrUSART -> Control(ARM_USART_CONTROL_TX, 1);
	if (status != ARM_DRIVER_OK) return (-1);
	
	status = ptrUSART -> Control(ARM_USART_CONTROL_RX, 1);
	if (status != ARM_DRIVER_OK) return (-1);
	
	return (0);
}

/**
  Get a character from stdin
 
  \return     The next character from the input, or -1 on read error.
*/
int stdin_getchar (void) {
	uint8_t buf[1];

	if (ptrUSART->Receive(buf, 1) != ARM_DRIVER_OK) {
		return (-1);
	}
	while (ptrUSART->GetRxCount() != 1);
	return (buf[0]);
}

/**
  Put a character to the stdout
 
  \param[in]   ch  Character to output
  \return          The character written, or -1 on write error.
*/
int stdout_putchar (int ch) {
	uint8_t buf[1];

	buf[0] = ch;
	if (ptrUSART->Send(buf, 1) != ARM_DRIVER_OK) {
		return (-1);
	}
	while (ptrUSART->GetTxCount() != 1);
	return (ch);
}
