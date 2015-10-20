/*
 *	==========================================================================
 *   main.c
 *   (c) 2014, Petr Machala
 *
 *   Description:
 *   OptRec sensor system main file.
 *   Optimized for 32F429IDISCOVERY board.
 *
 *   This program is free software: you can redistribute it and/or modify
 *   it under the terms of the GNU General Public License as published by
 *   the Free Software Foundation, either version 3 of the License, or
 *   any later version.
 *
 *   This program is distributed in the hope that it will be useful,
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *   GNU General Public License for more details.
 *
 *   You should have received a copy of the GNU General Public License
 *   along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *	==========================================================================
 */

#include "stm32f4xx.h"
#include "system_control.h"
#include "OV7670_control.h"
#include <stdbool.h>
#include "usart.h"

static volatile bool frame_flag = false;
static volatile bool send_sync_frame = false;
static volatile int state = 0;

volatile uint8_t temp_buffer[IMG_ROWS * IMG_COLUMNS];

void dumpFrame(void) {

	uint8_t *buffer = (uint8_t *) frame_buffer;
	int length = IMG_ROWS * IMG_COLUMNS * 2;
	int i;
	for (i = 1; i < length; i += 2) {
		temp_buffer[i / 2] = buffer[i];
	}
//	USART_Print("QQQQ");
//	for (i = 0; i < (length / 2); i++) {
//		Serial_sendHexByte(temp_buffer[i]);
//	}
//	USART_Print("XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX");
//	USART_Print("Now for something different\r\n");
//	for (i = 0; i < 100; i++) {
//		while (USART_GetFlagStatus(USART2, USART_FLAG_TXE) == RESET)
//					; // Wait for Empty
//				USART_SendData(USART2, 0xff);
//	}
	if (send_sync_frame) {
		for (i = 0x7f; i > 0; i--) {
			while (USART_GetFlagStatus(USART2, USART_FLAG_TXE) == RESET)
				; // Wait for Empty
			USART_SendData(USART2, i);
		}
		send_sync_frame = false;
	}

	for (i = 0; i < (length / 2); i++) {
		while (USART_GetFlagStatus(USART2, USART_FLAG_TXE) == RESET)
			; // Wait for Empty
		if (i > 100) {
			USART_SendData(USART2, temp_buffer[i]);
		} else {
			USART_SendData(USART2, 0xff);
		}
	}
	DMA_Cmd(DMA2_Stream1, ENABLE);
	DCMI_Cmd(ENABLE);
	DCMI_CaptureCmd(ENABLE);
}

int main(void) {
	bool err;

	// System init
	SystemInit();
	USART_RCC_Config();
	MCO1_init();
	SCCB_init();
	DCMI_DMA_init();

	USART_GPIO_Config();
	USART_Initialize();

	USART_Print("Starting OV7670 Demo\r\n");

	// OV7670 configuration
	err = OV7670_init();

	if (err == true) {
		USART_Print("Failed to initialize\r\n");
		while (1) {
		}
	} else {
		USART_Print("Successfully initialized\r\n");
	}

	// Infinite program loop
	while (1) {
		if (frame_flag == true) {
			frame_flag = false;
			dumpFrame();
		} else if (state != 0) {
			USART_Print("Interrupt=");
			Serial_logi(state);
			USART_Print("\r\n");
			state = 0;
		}
	}
}

void DMA2_Stream1_IRQHandler(void) {
	// DMA complete
	if (DMA_GetITStatus(DMA2_Stream1, DMA_IT_TCIF1) != RESET) {
		DMA_ClearITPendingBit(DMA2_Stream1, DMA_IT_TCIF1);
		frame_flag = true;
		state = 1;
	} else if (DMA_GetITStatus(DMA2_Stream1, DMA_IT_TEIF1) != RESET) {
		state = 2;
		DMA_ClearITPendingBit(DMA2_Stream1, DMA_IT_TEIF1);
	}
}

void DCMI_IRQHandler(void) {
	if (DCMI_GetFlagStatus(DCMI_FLAG_FRAMERI) == SET) {
		state = 3;
		DCMI_ClearFlag(DCMI_FLAG_FRAMERI);

		DMA_Cmd(DMA2_Stream1, DISABLE);
		DCMI_Cmd(DISABLE);
		DCMI_CaptureCmd(DISABLE);
	}
	if (DCMI_GetFlagStatus(DCMI_FLAG_OVFRI) == SET) {
		state = 4;
		DCMI_ClearFlag(DCMI_FLAG_OVFRI);
	}
	if (DCMI_GetFlagStatus(DCMI_FLAG_ERRRI) == SET) {
		state = 5;
		DCMI_ClearFlag(DCMI_FLAG_ERRRI);
	}
}

void USART2_IRQHandler(void) {
//	if (Serial_readChar() == 'S') {
	send_sync_frame = true;
//	}
	USART_ClearFlag(USART2, USART_FLAG_RXNE);

}
