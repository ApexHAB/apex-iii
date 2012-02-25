/**
 * modules.cpp
 *
 * Part of the Apex III project
 * http://www.apexhab.org/apex-iii/
 *
 * Priyesh Patel
 *
 * (c) Copyright ApexHAB 2011
 * team@apexhab.org
 */

#include "modules.h"

void modules_broadcast(uint16_t altitude, commands command)
{
    modules_send(altitude, BDCT, command);
}

void modules_request(char* buffer, addresses address, uint16_t altitude, commands command)
{
    modules_send(altitude, address, command);

    modules_receive(buffer);
    
    if (*buffer != address)
    {
        char error[6] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
        memcpy(buffer, error, 6);
    }
}

void modules_send(uint16_t altitude, addresses address, commands command)
{
    SoftwareSerial bus(MODULES_RX, MODULES_TX);
    bus.begin(2400);
    
    byte NULL_STARTER = 0;

    bus.write(NULL_STARTER);
    bus.write('#');
    bus.write(CORE);
    bus.write('+');
    bus.write(address);
    bus.write(command);
    bus.write(altitude & 0xFF);
    bus.write(altitude >> 8);
}

void modules_receive(char* buffer)
{
    SoftwareSerial bus(MODULES_RX, MODULES_TX);
    bus.begin(2400);
    
    unsigned long time = millis();

    while (bus.read() != '#')
    {
        if (millis() - time > 2000) break;
    }

    char* bufptr = buffer;
    for (int i=0; i<MODULES_BUFFER_LENGTH; i++)
    {
        time = millis();
        while (!bus.available())
        {
            if (millis() - time > 1000) break;
        }
        *bufptr++ = bus.read();
    }
}
