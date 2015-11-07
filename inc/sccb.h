/*
 * sccb.h
 *
 *  Created on: 7 Nov 2015
 *      Author: erikandre
 */

#ifndef SCCB_H_
#define SCCB_H_

#include "stm32f4xx.h"
#include "stm32f4xx_rcc.h"
#include "stm32f4xx_gpio.h"
#include "stm32f4xx_i2c.h"
#include "stm32f4xx_dma.h"
#include "stm32f4xx_dcmi.h"

#define SCCB_ERROR_BUSY			0x01
#define SCCB_ERROR_START_FAIL	0x02
#define SCCB_ERROR_ADDR_FAIL	0x03
#define SCCB_ERROR_DATA_FAIL	0x04

/**
 * Write a single register value to the specified SCCB device
 *
 * @param deviceWriteAddr
 * @param regAddr
 * @param data
 */
uint8_t SCCB_write(uint8_t deviceWriteAddr, uint8_t regAddr, uint8_t data);

uint8_t SCCB_read(uint8_t deviceWriteAddr, uint8_t regAddr, uint8_t* status);

/**
 * Initialize the SCCB (I2C) peripheral interface
 */
void SCCB_init();

#endif /* SCCB_H_ */
