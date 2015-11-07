/*
 * sccb.c
 *
 *  Created on: 7 Nov 2015
 *      Author: Erik Andre
 */

#include "sccb.h"

uint8_t SCCB_waitForIdle() {
	uint32_t timeout = 0x7FFFFF;
	while (I2C_GetFlagStatus(I2C2, I2C_FLAG_BUSY)) {
		if ((timeout--) == 0) {
			return 1;
		}
	}
	return 0;
}

uint8_t SCCB_start() {
	I2C_GenerateSTART(I2C2, ENABLE);
	uint32_t timeout = 0x7FFFFF;
	while (!I2C_CheckEvent(I2C2, I2C_EVENT_MASTER_MODE_SELECT)) {
		if ((timeout--) == 0) {
			return 1;
		}
	}
	return 0;
}

void SCCB_stop() {
	I2C_GenerateSTOP(I2C2, ENABLE);
}

uint8_t SCCB_sendWriteAddress(uint8_t deviceWriteAddr) {
	I2C_Send7bitAddress(I2C2, deviceWriteAddr, I2C_Direction_Transmitter);
	uint32_t timeout = 0x7FFFFF;
	while (!I2C_CheckEvent(I2C2, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED)) {
		if ((timeout--) == 0) {
			return 1;
		}
	}
	return 0;
}

uint8_t SCCB_writeByte(uint8_t value) {
	I2C_SendData(I2C2, value);
	uint32_t timeout = 0x7FFFFF;
	while (!I2C_CheckEvent(I2C2, I2C_EVENT_MASTER_BYTE_TRANSMITTED)) {
		if ((timeout--) == 0) {
			return 1;
		}
	}
	return 0;
}

uint8_t SCCB_readByte() {
	uint32_t timeout = 0x7FFFFF;
	while (!I2C_CheckEvent(I2C2, I2C_EVENT_MASTER_BYTE_RECEIVED)) {
		if ((timeout--) == 0) {
			return 0xff;
		}
	}
	return I2C_ReceiveData(I2C2);
}

uint8_t SCCB_write(uint8_t deviceWriteAddr, uint8_t regAddr, uint8_t data) {
	// Wait for idle before we can start
	if (SCCB_waitForIdle()) {
		return SCCB_ERROR_BUSY;
	}
	// Send the start bit to initiate the transfer
	if (SCCB_start()) {
		return SCCB_ERROR_START_FAIL;
	}
	// Send the slave write address
	if (SCCB_sendWriteAddress(deviceWriteAddr)) {
		return SCCB_ERROR_ADDR_FAIL;
	}
	// Send the address of the register we are updating
	if (SCCB_writeByte(regAddr)) {
		return SCCB_ERROR_DATA_FAIL;
	}
	// Send the updated register value
	if (SCCB_writeByte(data)) {
		return SCCB_ERROR_DATA_FAIL;
	}
	// End by sending the stop bit
	SCCB_stop();
	return 0;
}

uint8_t SCCB_read(uint8_t deviceWriteAddr, uint8_t regAddr, uint8_t* status) {
	// Wait for idle before we can start
	if (SCCB_waitForIdle()) {
		*status = SCCB_ERROR_BUSY;
		return 0;
	}
	// Send the start bit to initiate the transfer
	if (SCCB_start()) {
		*status = SCCB_ERROR_START_FAIL;
		return 0;
	}
	// Send the slave (write) address
	if (SCCB_sendWriteAddress(deviceWriteAddr)) {
		*status = SCCB_ERROR_ADDR_FAIL;
		return 0;
	}
	// Send the address of the register we are updating
	if (SCCB_writeByte(regAddr)) {
		*status = SCCB_ERROR_DATA_FAIL;
		return 0;
	}
	// Send stop before we start reading the registry value
	SCCB_stop();
	// Send the start bit to start the read
	if (SCCB_start()) {
		*status = SCCB_ERROR_START_FAIL;
		return 0;
	}
	// Send the slave (read) address
	if (SCCB_sendWriteAddress(deviceWriteAddr)) {
		*status = SCCB_ERROR_ADDR_FAIL;
		return 0;
	}
	uint8_t data = SCCB_readByte();
	// Send stop again to finish the transfer
	SCCB_stop();
	return data;
}

void SCCB_init(void) {
	GPIO_InitTypeDef GPIO_InitStructure;
	I2C_InitTypeDef I2C_InitStructure;

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_I2C2, ENABLE);
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE);

	// GPIO config
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_OD;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_Init(GPIOB, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_OD;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_Init(GPIOC, &GPIO_InitStructure);

	// GPIO AF config
	GPIO_PinAFConfig(GPIOB, GPIO_PinSource10, GPIO_AF_I2C2);
	GPIO_PinAFConfig(GPIOC, GPIO_PinSource12, GPIO_AF_I2C2);

	// I2C config
	I2C_DeInit(I2C2);
	I2C_InitStructure.I2C_Mode = I2C_Mode_I2C;
	I2C_InitStructure.I2C_DutyCycle = I2C_DutyCycle_2;
	I2C_InitStructure.I2C_OwnAddress1 = 0x00;
	I2C_InitStructure.I2C_Ack = I2C_Ack_Enable;
	I2C_InitStructure.I2C_AcknowledgedAddress = I2C_AcknowledgedAddress_7bit;
	I2C_InitStructure.I2C_ClockSpeed = 100000;
	I2C_ITConfig(I2C2, I2C_IT_ERR, ENABLE);
	I2C_Init(I2C2, &I2C_InitStructure);
	I2C_Cmd(I2C2, ENABLE);
}

