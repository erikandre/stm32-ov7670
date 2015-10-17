/*
*	==========================================================================
*   system_control.h	
*   (c) 2014, Petr Machala
*
*   Description:
*   System control library for various functions.
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

// Includes
#include "stm32f4xx.h"
#include "stm32f4xx_rcc.h"
#include "stm32f4xx_tim.h"
#include "stm32f4xx_gpio.h"

// LED
#define LED_GREEN					GPIO_Pin_13
#define LED_RED						GPIO_Pin_14
#define LED_PORT					GPIOG

// Button
#define BUTTON_PIN				GPIO_Pin_0
#define BUTTON_PORT				GPIOA

/*
* Initialize LED signalization
*/
extern void STM_LedInit(void);

/*
* Initialize USER button
*/
extern void STM_ButtonInit(void);

/*
* Turn on LED
*/
extern void STM_LedOn(uint16_t led);

/*
* Turn off LED
*/
extern void STM_LedOff(uint16_t led);

/*
* State of USER button
*/
extern uint8_t STM_ButtonPressed(void);

/*
* Initialize Timer 3, Timer 4
*/
extern void STM_TimerInit(void);
