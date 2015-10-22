/*
 * usart.c
 *
 *  Created on: 17 Oct 2015
 *      Author: erikandre
 */

#include <stm32f4xx.h>
#include <stm32f4xx_gpio.h>
#include <stm32f4xx_rcc.h>
#include <stm32f4xx_usart.h>

void Serial_init(void) {

	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE);
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2 | GPIO_Pin_3; // PA.2 USART2_TX, PA.3 USART2_RX
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);

	// Set Alternate Function for RX and TX pins
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource2, GPIO_AF_USART2);
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource3, GPIO_AF_USART2);

	USART_InitStructure.USART_BaudRate = 230400; //230400 works fine for me but 115200 might be a safer option
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	USART_InitStructure.USART_Parity = USART_Parity_No;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
	USART_Init(USART2, &USART_InitStructure);
	USART_ITConfig(USART2, USART_IT_RXNE, ENABLE);

	// Enable interrupts for USART2 so that we can detect
	NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);

	USART_Cmd(USART2, ENABLE);
}

void Serial_log(char *s) {
	while (*s) {
		while (USART_GetFlagStatus(USART2, USART_FLAG_TXE) == RESET)
			; // Wait for Empty

		USART_SendData(USART2, *s++); // Send Char
	}
}

void Serial_sendHexByte_half(uint8_t val) {
	char c = 'Q'; // Just in case
	switch (val) {
	case 0:
		c = '0';
		break;
	case 1:
		c = '1';
		break;
	case 2:
		c = '2';
		break;
	case 3:
		c = '3';
		break;
	case 4:
		c = '4';
		break;
	case 5:
		c = '5';
		break;
	case 6:
		c = '6';
		break;
	case 7:
		c = '7';
		break;
	case 8:
		c = '8';
		break;
	case 9:
		c = '9';
		break;
	case 0xa:
		c = 'a';
		break;
	case 0xb:
		c = 'b';
		break;
	case 0xc:
		c = 'c';
		break;
	case 0xd:
		c = 'd';
		break;
	case 0xe:
		c = 'e';
		break;
	case 0xf:
		c = 'f';
		break;
	}
	while (USART_GetFlagStatus(USART2, USART_FLAG_TXE) == RESET)
		; // Wait for Empty
	USART_SendData(USART2, c); // Send Char
}

void Serial_log_hexb(uint8_t val) {
	Serial_sendHexByte_half(val >> 4);
	Serial_sendHexByte_half(val & 0xf);
}

uint8_t Serial_read(void) {
	return USART_ReceiveData(USART2);
}

void Serial_sendb(uint8_t *val) {
	while (USART_GetFlagStatus(USART2, USART_FLAG_TXE) == RESET)
		; // Wait for Empty
	USART_SendData(USART2, *val);
}

void Serial_logi(int val) {
	char buffer[10];
	itoa(val, buffer, 10);
	Serial_log(&buffer[0]);
}

void Serial_logih(int val) {
	char buffer[10];
	itoa(val, buffer, 16);
	Serial_log(&buffer[0]);
}
