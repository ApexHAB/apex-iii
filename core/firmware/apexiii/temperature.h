/**
 * temperature.h
 *
 * Part of the Apex III project
 * http://www.apexhab.org/apex-iii/
 *
 * Priyesh Patel
 *
 * (c) Copyright ApexHAB 2011
 * team@apexhab.org
 */

#ifndef TEMPERATURE_H
#define TEMPERATURE_H

#include "WProgram.h"
#include "OneWire.h"

#define TEMPERATURE_PIN 2

float temperature_get(int pin, uint8_t* addr);

#endif
