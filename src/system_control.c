/*
*	==========================================================================
*   system_control.c	
*   (c) 2014, Petr Machala
*
*   Description:
*   System control file for various functions.
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

#include "system_control.h"

void STM_LedInit(void){
	GPIO_InitTypeDef GPIO_InitDef;	
	
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOG, ENABLE);
  	
	GPIO_InitDef.GPIO_Pin = LED_RED | LED_GREEN;
  GPIO_InitDef.GPIO_Mode = GPIO_Mode_OUT;
  GPIO_InitDef.GPIO_OType = GPIO_OType_PP;
  GPIO_InitDef.GPIO_PuPd = GPIO_PuPd_NOPULL;
  GPIO_InitDef.GPIO_Speed = GPIO_Speed_100MHz;
  
  GPIO_Init(GPIOG, &GPIO_InitDef);	
}

void STM_ButtonInit(void){
	GPIO_InitTypeDef GPIO_InitDef;
	
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);

	GPIO_InitDef.GPIO_Pin = BUTTON_PIN;
  GPIO_InitDef.GPIO_Mode = GPIO_Mode_IN;
  GPIO_InitDef.GPIO_OType = GPIO_OType_PP;
  GPIO_InitDef.GPIO_PuPd = GPIO_PuPd_DOWN;
  GPIO_InitDef.GPIO_Speed = GPIO_Speed_100MHz;
	
  GPIO_Init(GPIOA, &GPIO_InitDef);	
}

void STM_LedOn(uint16_t led){
	GPIO_SetBits(LED_PORT, led);
}

void STM_LedOff(uint16_t led){
	GPIO_ResetBits(LED_PORT, led);
}

uint8_t STM_ButtonPressed(void){
	return GPIO_ReadInputDataBit(BUTTON_PORT, BUTTON_PIN);
}

void STM_TimerInit(void){
	TIM_TimeBaseInitTypeDef TIM_BaseStruct;
	NVIC_InitTypeDef NVIC_InitStructure;
    
	// Timer3 - push button
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);
	TIM_BaseStruct.TIM_Period = 500-1;
	TIM_BaseStruct.TIM_Prescaler = 4800-1;
	TIM_BaseStruct.TIM_ClockDivision = 0;
	TIM_BaseStruct.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_BaseStruct.TIM_RepetitionCounter = 0;	
	TIM_TimeBaseInit(TIM3, &TIM_BaseStruct);
	TIM_ITConfig(TIM3, TIM_IT_Update, ENABLE);
	TIM_Cmd(TIM3, ENABLE);		
	
	NVIC_InitStructure.NVIC_IRQChannel = TIM3_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);
	
	//Timer4 - countdown
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE);
	TIM_BaseStruct.TIM_Period = 50000-1;
	TIM_BaseStruct.TIM_Prescaler = 9600-1;
	TIM_BaseStruct.TIM_ClockDivision = 0;
	TIM_BaseStruct.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_BaseStruct.TIM_RepetitionCounter = 0;	
	TIM_TimeBaseInit(TIM4, &TIM_BaseStruct);
	TIM_ITConfig(TIM4, TIM_IT_Update, ENABLE);
	TIM_Cmd(TIM4, DISABLE);
	
	NVIC_InitStructure.NVIC_IRQChannel = TIM4_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);
}
