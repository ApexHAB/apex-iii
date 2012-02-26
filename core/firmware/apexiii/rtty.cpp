/**
 * rtty.cpp
 *
 * Part of the Apex III project
 * http://www.apexhab.org/apex-iii/
 *
 * Priyesh Patel
 *
 * (c) Copyright ApexHAB 2011
 * team@apexhab.org
 */

#include "rtty.h"

void rtty_init()
{
    pinMode(TX_1,OUTPUT);
    pinMode(TX_0,OUTPUT);

    // Enable the NTR2
    pinMode(NTR2_EN,OUTPUT);
    digitalWrite(NTR2_EN,LOW);
}

void rtty_prepare(char* sentence)
{
    uint16_t checksum = rtty_crc16_ccitt_checksum(sentence);

    char checksum_string[6];
    sprintf(checksum_string,"*%04X",checksum);

    strcat(sentence,checksum_string);
    strcat(sentence,"\n");
}

void rtty_tx(char* sentence, int baud)
{
    // Disable interrupts
    noInterrupts();

    int i=0;
    while(sentence[i] != 0)
    {
        rtty_tx_byte(sentence[i], baud);
        i++;
    }

    // Re-enable interrupts
    interrupts();
}

void rtty_preamble(int baud)
{
    char sentence[] = "UUUUUUUUUUUUUUUUUUUUUU\n";

    // Disable interrupts
    noInterrupts();

    int i=0;
    while(sentence[i] != 0)
    {
        rtty_tx_byte(sentence[i], baud);
        i++;
    }

    // Re-enable interrupts
    interrupts();
}

void rtty_tx_byte(char c, int baud)
{
    // Start bit
    rtty_tx_bit(0, baud);

    // Send byte
    for(int b=0; b<8; b++)
    {
        if(c & 1)
        {
            rtty_tx_bit(1, baud);
        }
        else
        {
            rtty_tx_bit(0, baud);
        }

        c = c >> 1;
    }

    // 2 Stop bits
    rtty_tx_bit(1, baud);
    rtty_tx_bit(1, baud);
}

void rtty_tx_bit(int b, int baud)
{
    if(b)
    {
        // If HIGH
        digitalWrite(TX_1,HIGH);
        digitalWrite(TX_0,LOW);
    }
    else
    {
        // If LOW
        digitalWrite(TX_1,LOW);
        digitalWrite(TX_0,HIGH);        
    }

    if(baud == 1)
    {
        // 300 baud
        delayMicroseconds(3370);
    }
    else if(baud == 0)
    {
        // 50 baud
        delayMicroseconds(10000);
        delayMicroseconds(10150);
    }
    else
    {
        // Otherwise default to 50 baud
        delayMicroseconds(10000);
        delayMicroseconds(10150);
    }
}

uint16_t rtty_crc16_ccitt_checksum(char* sentence)
{
    size_t i;
    uint16_t crc;
    uint8_t c;

    crc = 0xFFFF;

    // Skip the $$ at the beginning
    for (i=2; i<strlen(sentence); i++)
    {
        c = sentence[i];
        crc = _crc_xmodem_update(crc, c);
    }

    return crc;
}
