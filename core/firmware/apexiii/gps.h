/**
 * gps.h
 *
 * Part of the Apex III project
 * http://www.apexhab.org/apex-iii/
 *
 * Priyesh Patel
 *
 * (c) Copyright ApexHAB 2011
 * team@apexhab.org
 */

#ifndef GPS_H
#define GPS_H

#include "Arduino.h"
#include "SoftwareSerial.h"

#define GPS_RX 3
#define GPS_TX 4

char* gps_get();
void gps_strCopy(char* str, char* dest, int pos, int len);

#endif
