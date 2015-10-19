/*
 * usart.h
 *
 *  Created on: 17 Oct 2015
 *      Author: erikandre
 */

#ifndef USART_H_
#define USART_H_

void USART_RCC_Config(void);

void USART_GPIO_Config(void);

void USART_Initialize(void);

void USART_Print(char *s);

void Serial_logi(int val);

void Serial_sendHexByte(uint8_t val);

#endif /* USART_H_ */
