/*
 * ov9655.c
 *
 *  Created on: 7 Nov 2015
 *      Author: erikandre
 */

#define OV9655_DEVICE_ADDRESS    0x60

#define OV9655_MIDH       0x1C
#define OV9655_MIDL       0x1D

#include "ov9655.h"

void OV9655_init() {
	uint8_t status = 0;
	uint8_t data;
	data = SCCB_read(OV9655_DEVICE_ADDRESS, 0xff, &status);
	if (status) {
		Serial_log("Error ");
		Serial_logi(status);
		Serial_log("\r\n");
		return;
	}
	Serial_log("h=");
	Serial_log_hexb(data);
	Serial_log(",");
	data = SCCB_read(OV9655_DEVICE_ADDRESS, OV9655_MIDL, &status);
	if (status) {
		Serial_log("Error ");
		Serial_logi(status);
		Serial_log("\r\n");
		return;
	}
	Serial_log("l=");
	Serial_log_hexb(data);
	Serial_log("\r\n");
}

