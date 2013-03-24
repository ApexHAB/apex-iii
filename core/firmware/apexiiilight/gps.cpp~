/**
 * gps.cpp
 *
 * Part of the Apex III project
 * http://www.apexhab.org/apex-iii/
 *
 * Priyesh Patel
 *
 * (c) Copyright ApexHAB 2011
 * team@apexhab.org
 */

#include "gps.h"

void gps_get(char* data)
{
    char buffer_gpgga[90];
    gps_nmea(buffer_gpgga, "GPGGA");

    // Check for GPS Fix
    if(buffer_gpgga[42] == '1' || buffer_gpgga[42] == '2')
    {
        char* dataptr = data;

        // Parse data
        memcpy(dataptr, buffer_gpgga + 7, 2); // Hours
        dataptr += 2;
        *dataptr++ = ':';

        memcpy(dataptr, buffer_gpgga + 9, 2); // Minutes
        dataptr += 2;
        *dataptr++ = ':';

        memcpy(dataptr, buffer_gpgga + 11, 2); // Seconds
        dataptr += 2;
        *dataptr++ = ',';

        // Prepend a '-' if the latitude is S
        if (buffer_gpgga[27] == 'S') *dataptr++ = '-';

        memcpy(dataptr, buffer_gpgga + 17, 9); // Latitude
        dataptr += 9;
        *dataptr++ = ',';

        // Prepend a '-' if the longitude is W
        if (buffer_gpgga[40] == 'W') *dataptr++ = '-';

        memcpy(dataptr, buffer_gpgga + 29, 10); // Longitude 
        dataptr += 10;
        *dataptr++ = ',';

        uint8_t shift = 0;
        if (buffer_gpgga[52] == '-') shift = 1;

        memcpy(dataptr, buffer_gpgga + 52 + shift, 5); // Altitude
        dataptr += 5;
        *dataptr++ = ',';

        memcpy(dataptr, buffer_gpgga + 44, 2); // No of Satellites
        dataptr += 2;

        // Append a null character
        *dataptr = '\0';
    }
    // If there is no fix, return empty fields
    else
    {
        strcpy(data, ",,,,");
    }
}

void gps_nmea(char* buffer, char* nmea)
{
    // Software serial
    SoftwareSerial gps(GPS_RX, GPS_TX);

    // Begin software serial at 4800 baud
    gps.begin(4800);
    // Set GPS as active software serial device
    gps.listen();

    // Boolean to store whether we have the correct NMEA 
    // sentence type currently
    bool correct = false;

    // While the current sentence type is not needed sentence type
    while(!correct)
    {
        // Wait for the start of a NMEA sentence
        while(gps.read() != '$');

        // Create a buffer for the NMEA sentence format
        char serbuf[strlen(nmea)+1];
        char* serbufptr = serbuf;

        // Put NMEA sentence format into buffer 
        for (int i=0; i<strlen(nmea); i++)
        {
            while (!gps.available());
            *serbufptr++ = gps.read();
        }

        *serbufptr = '\0';

        // Compare buffer with the specified NMEA type
        if((strcmp(nmea,serbuf) == 0)) correct = true;
    }

    // Keep a pointer to the next free char in the buffer
    char* bufptr = buffer;

    // Write a $ and then the sentence type
    *bufptr++ = '$';
    memcpy(bufptr, nmea, strlen(nmea));
    bufptr += strlen(nmea);

    // Place the rest of the sentence (upto the CR) into the buffer
    while (*(bufptr-1) != 13)
    {
        while(!gps.available());
        *bufptr++ = gps.read();
    }

    // End the software serial session to the GPS
    gps.end();

    // Null terminate the string
    *(bufptr-1) = '\0';
}
