/*
 * ov9655.c
 *
 *  Created on: 7 Nov 2015
 *      Author: erikandre
 */

#define OV9655_DEVICE_ADDRESS    0x60
//#define OV9655_DEVICE_ADDRESS 0x42
//#define OV9655_DEVICE_ADDRESS 0x3c

#define OV9655_MIDH       0x1C
#define OV9655_MIDL       0x1D

#include "ov9655.h"

void tryRead(uint8_t addr, uint8_t reg) {
	Serial_log("Reading from device 0x");
	Serial_log_hexb(addr);
	Serial_log(", register 0x");
	Serial_log_hexb(reg);
	Serial_log("\r\n");
	uint8_t status = 0;
	uint8_t data;
	data = SCCB_read(addr, reg, &status);
	if (status) {
		Serial_log("Error: ");
		Serial_logi(status);
		Serial_log("\r\n");
	} else {
		Serial_log("Value: 0x");
		Serial_log_hexb(data);
		Serial_log("\r\n");
	}
}

void OV9655_init() {
//	uint8_t i;
//	for (i = 1; i < 100; i++) {
//		tryRead(i);
//	}
	tryRead(OV9655_DEVICE_ADDRESS, 0x0a); // First read fails (?)
	tryRead(OV9655_DEVICE_ADDRESS, 0x0a);
	tryRead(OV9655_DEVICE_ADDRESS, 0x0b);
//	uint8_t status = 0;
//	uint8_t data;
//	data = SCCB_read(OV9655_DEVICE_ADDRESS, 0x0a, &status);
//	if (status) {
//		Serial_log("Error ");
//		Serial_logi(status);
//		Serial_log("\r\n");
//		return;
//	}
//	Serial_log("h=");
//	Serial_log_hexb(data);
//	Serial_log(",");
//	data = SCCB_read(OV9655_DEVICE_ADDRESS, 0x0b, &status);
//	if (status) {
//		Serial_log("Error ");
//		Serial_logi(status);
//		Serial_log("\r\n");
//		return;
//	}
//	Serial_log("l=");
//	Serial_log_hexb(data);
//	Serial_log("\r\n");
}

