/*
 * usart.h
 *
 *  Created on: 17 Oct 2015
 *      Author: erikandre
 */

#ifndef USART_H_
#define USART_H_

void Serial_init(void);

/**
 * Log a null terminated string
 */
void Serial_log(char *s);

/*
 * Log an integer (as a string)
 */
void Serial_logi(int val);

/*
 * Log an integer (as a hex string)
 */
void Serial_logih(int val);

/**
 * Log a byte (as a hex string)
 */
void Serial_log_hexb(uint8_t val);

/**
 * Send a raw byte
 */
void Serial_sendb(uint8_t *val);

/**
 * Read a byte
 */
uint8_t Serial_read(void);

#endif /* USART_H_ */
