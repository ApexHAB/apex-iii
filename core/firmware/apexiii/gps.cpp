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

char* gps_get()
{
    // Software serial
    NewSoftSerial gps(GPS_RX, GPS_TX);

    // Begin software serial at 4800 baud
    gps.begin(4800);
    // Set GPS as active software serial device
    gps.active();

    // Boolean to store whether we have the correct NMEA 
    // sentence type currently
    bool correctNmeaType = false;

    // The NMEA sentence type needed
    char nmeaType[6] = "GPGGA";

    // While the current sentence type is not needed sentence type
    while(!correctNmeaType)
    {
        // Wait for the start of a NMEA sentence
        while(gps.read() != '$') {}

        // Create a buffer for the NMEA sentence format
        // and then set the last character of the buffer
        // to a null character
        char serbuf[6];
        serbuf[5] = 0;

        // Put NMEA sentence format into buffer 
        for (int i=0; i<5; i++)
        {
            while (!gps.available()) {}
            serbuf[i] = gps.read();
        }

        // Compare buffer with the specified NMEA type
        // above and put the result in 'matches'
        if((strncmp(nmeaType,serbuf,5) == 0) && (strlen(serbuf) >= 5)) correctNmeaType = true;
    }

    // Create a char array to contain the rest of the sentence
    char data[100] = "";
    data[0] = 0;
    uint8_t i = 1;

    // Place the rest of the sentence (upto the CR) into a buffer
    while(data[i-1] != 13)
    {
        while(!gps.available()) {}
        data[i] = gps.read();
        i++;
    }

    // Check for GPS Fix
    if(data[37] == '1' || data[37] == '2')
    {
        // Create a char array for the parsed data
        char parsed[50]; 
        sprintf(parsed,"");

        // Parse data
        gps_strCopy(data,parsed+strlen(parsed),2,2);
        strcat(parsed,":");
        gps_strCopy(data,parsed+strlen(parsed),4,2);
        strcat(parsed,":");
        gps_strCopy(data,parsed+strlen(parsed),6,2);
        strcat(parsed,",");
        // Prepend a '-' if the latitude is S
        if(data[22] == 'S') strcat(parsed,"-");
        gps_strCopy(data,parsed+strlen(parsed),12,9);
        strcat(parsed,",");
        // Prepend a '-' if the longitude is W
        if(data[35] == 'W') strcat(parsed,"-");
        gps_strCopy(data,parsed+strlen(parsed),24,10);
        strcat(parsed,",");
        gps_strCopy(data,parsed+strlen(parsed),47,5);
        strcat(parsed,",");
        gps_strCopy(data,parsed+strlen(parsed),39,2);

        // Return the parsed data
        return parsed;
    }
    // If there is no fix, return empty fields
    else
    {
        return ",,,,";
    }

    // End the software serial session to the GPS
    gps.end();
}

void gps_strCopy(char* str, char* dest, int pos, int len)
{
    while(len > 0)
    {
        *(dest) = *(str + pos);
        dest++;
        str++;
        len--;
    }
    *dest = '\0';
}
