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

static volatile uint8_t STM_mode = 0;
static volatile bool btn_pressed = false;
static volatile bool sett_mode = true;
static volatile bool frame_flag = false;

int main(void) {
	bool err;

	// System init
	SystemInit();
//	STM_LedInit();
//	STM_ButtonInit();
//	STM_TimerInit();
	USART_RCC_Config();
	MCO1_init();
	SCCB_init();
	DCMI_DMA_init();

	USART_GPIO_Config();
	USART_Initialize();

	USART_Print("Starting OV7670 Demo\r\n");

//	long delay = 100000000;
//	while (delay) {
//		delay--;
//	}

	// OV7670 configuration
	err = OV7670_init();

	if (err == true) {
		USART_Print("Failed to initialize\r\n");
		while (1) {
		}
	} else {
		USART_Print("Successfully initialized\r\n");
		//TODO Print success message
	}

	// Infinite program loop
	while (1) {
//		if (btn_pressed == true) {
//			if (STM_mode == 0) {
//				// MODE 1 - RUN
//				STM_mode = 1;
//				STM_LedOff(LED_RED);
//				STM_LedOn(LED_GREEN);
//
//				DCMI_CaptureCmd(ENABLE);
//			} else {
//				if (frame_flag == false) {
//					// MODE 2 - STOP
//					STM_mode = 0;
//					STM_LedOff(LED_GREEN);
//					STM_LedOn(LED_RED);
//
//					DCMI_CaptureCmd(DISABLE);
//				}
//			}
//			btn_pressed = false;
//		}

		if (frame_flag == true) {
			// TODO Transfer photo
//			LCD_ILI9341_Rotate(LCD_ILI9341_Orientation_Landscape_1);
//			LCD_ILI9341_DisplayImage((uint16_t*) frame_buffer);

			frame_flag = false;
		}
	}
}

//void TIM3_IRQHandler(void) {
//	if (TIM_GetITStatus(TIM3, TIM_IT_Update) != RESET) {
//		static uint8_t old_state = 0xFF;
//		uint8_t new_state = STM_ButtonPressed();
//
//		TIM_ClearITPendingBit(TIM3, TIM_IT_Update);
//
//		// Button state
//		if (new_state > old_state) {
//			if (STM_mode == 0) {
//				TIM_Cmd(TIM4, ENABLE);
//			}
//			sett_mode = false;
//		}
//		if (sett_mode == false) {
//			if (new_state < old_state) {
//				btn_pressed = true;
//				if (STM_mode == 0)
//					TIM_Cmd(TIM4, DISABLE);
//			}
//		}
//		old_state = new_state;
//	}
//}
//
//void TIM4_IRQHandler(void) {
//	if (TIM_GetITStatus(TIM4, TIM_IT_Update) != RESET) {
//		static bool init = false;
//
//		TIM_ClearITPendingBit(TIM4, TIM_IT_Update);
//
//		if (init == true) {
//			// MODE 3 - SETTINGS
//			TIM_Cmd(TIM4, DISABLE);
//
//			sett_mode = true;
//			STM_mode = 3;
//			STM_LedOn(LED_GREEN);
//			STM_LedOn(LED_RED);
//		} else {
//			init = true;
//		}
//	}
//}

void DMA2_Stream1_IRQHandler(void) {
	// DMA complete
	if (DMA_GetITStatus(DMA2_Stream1, DMA_IT_TCIF1) != RESET) {
		DMA_ClearITPendingBit(DMA2_Stream1, DMA_IT_TCIF1);

		DMA_Cmd(DMA2_Stream1, ENABLE);
		frame_flag = true;
	}
}
