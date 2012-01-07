/**
 * counter.h
 *
 * Part of the Apex III project
 * http://www.apexhab.org/apex-iii/
 *
 * Priyesh Patel
 *
 * (c) Copyright ApexHAB 2011
 * team@apexhab.org
 */

#ifndef COUNTER_H
#define COUNTER_H

#include "WProgram.h"
#include "EEPROM.h"

#define EEPROM_LOW_BYTE 0
#define EEPROM_HIGH_BYTE 1

uint16_t counter_get();
void counter_set(uint16_t new_counter);
void counter_inc();
void counter_reset();

#endif
