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

char* modules_request(addresses address, uint16_t altitude, commands command)
{
    modules_send(altitude, address, command);

    char* output = modules_receive();
    
    if (output[0] == address)
    {
        return output;
    }
    else
    {
        char error[7] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
        return error;
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

char* modules_receive()
{
    SoftwareSerial bus(MODULES_RX, MODULES_TX);
    bus.begin(2400);
    
    unsigned long time = millis();

    while (bus.read() != '#')
    {
        if (millis() - time > 2000) break;
    }

    char buffer[7];
    buffer[6] = 0;
    for (int i=0; i<6; i++)
    {
        time = millis();
        while (!bus.available())
        {
            if (millis() - time > 1000) break;
        }
        buffer[i] = bus.read();
    }

    return buffer;
}
