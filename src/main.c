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

volatile uint8_t temp_buffer[IMG_ROWS * IMG_COLUMNS];

void dumpFrame(void) {

	uint8_t *buffer = (uint8_t *) frame_buffer;
	int length = IMG_ROWS * IMG_COLUMNS * 2;
	// Copy every other byte from the main frame buffer to our temporary buffer (this converts the image to grey scale)
	int i;
	for (i = 1; i < length; i += 2) {
		temp_buffer[i / 2] = buffer[i];
	}
	// We only send the sync frame if it has been requested
	if (send_sync_frame) {
		for (i = 0x7f; i > 0; i--) {
			uint8_t val = i;
			Serial_sendb(&val);
		}
		send_sync_frame = false;
	}

	for (i = 0; i < (length / 2); i++) {
		if (i > 100) {
			Serial_sendb(&temp_buffer[i]);
		} else {
			uint8_t val = 0xff;
			Serial_sendb(&val); // Change first 100 pixels to white to provide a reference for where the frame starts
		}
	}
	// Enable capture and DMA after we have sent the photo. This is a workaround for the timing issues I've been having where
	// the DMA transfer is not in sync with the frames being sent
	DMA_Cmd(DMA2_Stream1, ENABLE);
	DCMI_Cmd(ENABLE);
	DCMI_CaptureCmd(ENABLE);
}

int main(void) {
	bool err;

	SystemInit();
	MCO1_init();
	SCCB_init();
	DCMI_DMA_init();
	Serial_init();

	// Initialize camera over SCCB
	err = OV7670_init();

	if (err == true) {
		Serial_log("Failed to initialize\r\n");
		while (1) {
		}
	}

	// Infinite program loop
	while (1) {
		if (frame_flag == true) {
			frame_flag = false;
			dumpFrame();
		}
	}
}

void DMA2_Stream1_IRQHandler(void) {
	// DMA complete
	if (DMA_GetITStatus(DMA2_Stream1, DMA_IT_TCIF1) != RESET) { // Transfer complete
		DMA_ClearITPendingBit(DMA2_Stream1, DMA_IT_TCIF1);
		frame_flag = true;
	} else if (DMA_GetITStatus(DMA2_Stream1, DMA_IT_TEIF1) != RESET) { // Transfer error
		// Not used, just for debug
		DMA_ClearITPendingBit(DMA2_Stream1, DMA_IT_TEIF1);
	}
}

void DCMI_IRQHandler(void) {
	if (DCMI_GetFlagStatus(DCMI_FLAG_FRAMERI) == SET) { // Frame received
		DCMI_ClearFlag(DCMI_FLAG_FRAMERI);
		// After receiving a full frame we disable capture and the DMA transfer. This is probably a very inefficient way of capturing and sending frames
		// but it's the only way I've gotten to reliably work.
		DMA_Cmd(DMA2_Stream1, DISABLE);
		DCMI_Cmd(DISABLE);
		DCMI_CaptureCmd(DISABLE);
	}
	if (DCMI_GetFlagStatus(DCMI_FLAG_OVFRI) == SET) { // Overflow
		// Not used, just for debug
		DCMI_ClearFlag(DCMI_FLAG_OVFRI);
	}
	if (DCMI_GetFlagStatus(DCMI_FLAG_ERRRI) == SET) { // Error
		// Not used, just for debug
		DCMI_ClearFlag(DCMI_FLAG_ERRRI);
	}
}

void USART2_IRQHandler(void) {
	// Wait until the host sends us a S before sending the first sync frame
	if (Serial_read() == 0xbb) {
		send_sync_frame = true;
	}
	USART_ClearFlag(USART2, USART_FLAG_RXNE);

}
