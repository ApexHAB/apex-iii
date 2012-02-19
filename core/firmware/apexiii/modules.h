/**
 * modules.h
 *
 * Part of the Apex III project
 * http://www.apexhab.org/apex-iii/
 *
 * Priyesh Patel
 *
 * (c) Copyright ApexHAB 2011
 * team@apexhab.org
 */

#ifndef MODULES_H
#define MODULES_H

#include "Arduino.h"
#include "SoftwareSerial.h"
#include "protocol.h"

#define MODULES_RX 19
#define MODULES_TX 18

void modules_broadcast(uint16_t altitude, commands command = NONE);
void modules_request(char* buffer, addresses address, uint16_t altitude, commands command = NONE);
void modules_send(uint16_t altitude, addresses address, commands command);
void modules_receive(char* buffer);

#endif
