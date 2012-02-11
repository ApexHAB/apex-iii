/**
 * rtty.h
 *
 * Part of the Apex III project
 * http://www.apexhab.org/apex-iii/
 *
 * Priyesh Patel
 *
 * (c) Copyright ApexHAB 2011
 * team@apexhab.org
 */

/*
 * Bauds:
 *  default = 50 baud
 *  0 = 50 baud
 *  1 = 300 baud
 */

#ifndef RTTY_H
#define RTTY_H

#include "Arduino.h"
#include "util/crc16.h"

#define TX_0 5
#define TX_1 6
#define NTR2_EN 7

void rtty_init();
char* rtty_prepare(char* sentence);
void rtty_tx(char* sentence, int baud);
void rtty_preamble(int baud);
void rtty_tx_byte(char c, int baud);
void rtty_tx_bit(int b, int baud);
uint16_t rtty_crc16_ccitt_checksum(char* sentence);

#endif
