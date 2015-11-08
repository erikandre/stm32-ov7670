/*
 * sccb.c
 *
 *  Created on: 7 Nov 2015
 *      Author: Erik Andre
 */

#include "sccb.h"

#define I2C_SDA_PORT GPIOC
#define I2C_SDA_PIN  GPIO_Pin_12
#define I2C_SCL_PORT GPIOB
#define I2C_SCL_PIN  GPIO_Pin_10

#define SDAH GPIO_SetBits(I2C_SDA_PORT,I2C_SDA_PIN)
#define SDAL GPIO_ResetBits(I2C_SDA_PORT,I2C_SDA_PIN)
#define SCLH GPIO_SetBits(I2C_SCL_PORT,I2C_SCL_PIN)
#define SCLL GPIO_ResetBits(I2C_SCL_PORT,I2C_SCL_PIN)
#define SDAR GPIO_ReadInputDataBit(I2C_SDA_PORT,I2C_SDA_PIN)
#define SCLR GPIO_ReadInputDataBit(I2C_SCL_PORT,I2C_SCL_PIN)

void I2C_Delay(void) {
	unsigned int i = 500;
	while (i) {
		i--;
	}
}

uint8_t I2C_Start(void) {
	SDAH;
	SCLH;
	I2C_Delay();
	SDAL;
	I2C_Delay();
	SCLL;
	I2C_Delay();
	return (0x00);
}

void I2C_SendByte(unsigned char SendByte) {
	unsigned char i = 0;
	for (i = 0; i < 8; i++) {
		if ((SendByte << i) & 0x80) {
			SDAH;
		} else {
			SDAL;
		}
		I2C_Delay();
		SCLH;
		I2C_Delay();
		SCLL;
		I2C_Delay();
	}
}

unsigned char I2C_ReceiveByte(void) {
	unsigned char i = 8;
	unsigned char ReceiveByte = 0;

	SDAH;
	while (i--) {
		ReceiveByte <<= 1;
		SCLL;
		I2C_Delay();
		SCLH;
		I2C_Delay();
		if (SDAR) {
			ReceiveByte |= 0x01;
		}
	}
	SCLL;
	return ReceiveByte;
}

uint8_t I2C_WaitAck(void) {
	SDAH;
	I2C_Delay();
	SCLH;
	I2C_Delay();
	if (SDAR) {
		return (0xFF);
	}
	SCLL;
	I2C_Delay();
	return (0x00);
}

void I2C_Stop(void) {
	SDAL;
	I2C_Delay();
	SCLH;
	I2C_Delay();
	SDAH;
	I2C_Delay();
}

uint8_t SCCB_read(uint8_t deviceWriteAddr, uint8_t regAddr, uint8_t* status) {
	unsigned char Recieved_Data = 0;
	I2C_Delay();
	I2C_Start();
	I2C_SendByte(deviceWriteAddr);
	if (I2C_WaitAck()) {
		*status = SCCB_ERROR_START_FAIL;
		return (0xFF);
	}
	I2C_SendByte(regAddr);
	if (I2C_WaitAck()) {
		*status = SCCB_ERROR_ADDR_FAIL;
		return (0xFF);
	}
	I2C_Stop();
	I2C_Start();
	I2C_SendByte(deviceWriteAddr + 1);
	if (I2C_WaitAck()) {
		*status = SCCB_ERROR_ADDR_FAIL;
		return (0xFF);
	}
	Recieved_Data = I2C_ReceiveByte();
	I2C_Stop();
	return Recieved_Data;
}

void SCCB_init(void) {

	GPIO_InitTypeDef GPIO_InitStructure;
	/* Enable GPIOB clocks */
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE);

	/*SDA Pin Configuration*/
	GPIO_InitStructure.GPIO_Pin = I2C_SDA_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_OD;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_Init(I2C_SDA_PORT, &GPIO_InitStructure);
	/*SCL Pin Configuration*/
	GPIO_InitStructure.GPIO_Pin = I2C_SCL_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_Init(I2C_SCL_PORT, &GPIO_InitStructure);

//	// GPIO config
//	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
//	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
//	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
//	GPIO_InitStructure.GPIO_OType = GPIO_OType_OD;
//	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
//	GPIO_Init(GPIOB, &GPIO_InitStructure);
//
//	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12;
//	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
//	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
//	GPIO_InitStructure.GPIO_OType = GPIO_OType_OD;
//	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
//	GPIO_Init(GPIOC, &GPIO_InitStructure);

}

