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

enum addresses
{
    BDCT = 0x00,
    CORE = 0x01,
    SNCD = 0x02
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
