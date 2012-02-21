/**
 * protocol.h
 *
 * Part of the Apex III project
 * http://www.apexhab.org/apex-iii/
 *
 * Priyesh Patel
 *
 * (c) Copyright ApexHAB 2011
 * team@apexhab.org
 */

#ifndef PROTOCOL_H
#define PROTOCOL_H

#define MODULE_DATA_LENGTH 4
#define MODULE_BUFFER_LENGTH MODULE_DATA_LENGTH + 2

enum addresses
{
    BDCT = 0x00,
    CORE = 0x01,
    SNCD = 0x02,
    LGHT = 0x03,
    PRES = 0x04,
    TILT = 0x05,
    ATOD = 0x06,
    GASS = 0x07,
    RADA = 0x08,
    RADB = 0x09
};

enum commands
{
    NONE = 0x00,
    PING = 0x01,
    CDWN = 0x02,
    SRN0 = 0x03,
    SRN1 = 0x04,
    ALTR = 0x05
};

#endif
