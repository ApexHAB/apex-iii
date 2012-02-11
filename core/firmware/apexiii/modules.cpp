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

void modules_broadcast(uint16_t altitude)
{
    modules_send(altitude, BDCT, NONE);
}

char* modules_request(addresses address, uint16_t altitude, commands command)
{
    modules_send(altitude, address, command);
    return modules_receive();
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
    
    while (bus.read() != '#');

    char buffer[7];
    buffer[6] = 0;
    for (int i=0; i<6; i++)
    {
        while (!bus.available());
        buffer[i] = bus.read();
    }

    return buffer;
}
