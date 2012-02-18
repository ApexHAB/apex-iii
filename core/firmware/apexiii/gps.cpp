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
    SoftwareSerial gps(GPS_RX, GPS_TX);

    // Begin software serial at 4800 baud
    gps.begin(4800);
    // Set GPS as active software serial device
    gps.listen();

    // Boolean to store whether we have the correct NMEA 
    // sentence type currently
    bool correctNmeaType = false;

    // The NMEA sentence type needed (GPGGA)
    char nmeaType[6] = "GPGGA";

    // While the current sentence type is not needed sentence type
    while(!correctNmeaType)
    {
        // Wait for the start of a NMEA sentence
        while(gps.read() != '$');

        // Create a buffer for the NMEA sentence format
        // and then set the last character of the buffer
        // to a null character
        char serbuf[6];
        serbuf[5] = 0;

        // Put NMEA sentence format into buffer 
        for (int i=0; i<5; i++)
        {
            while (!gps.available());
            serbuf[i] = gps.read();
        }

        // Compare buffer with the specified NMEA type
        // above and put the result in 'matches'
        if((strncmp(nmeaType,serbuf,5) == 0) && (strlen(serbuf) >= 5))
            correctNmeaType = true;
    }

    // Create a char array to contain the rest of the sentence
    char data[130];
    data[0] = 32; // Place a space in the first item
    uint8_t i = 1;

    // Place the rest of the sentence (upto the CR) into a buffer
    while(data[i-1] != 13)
    {
        while(!gps.available());
        data[i] = gps.read();
        i++;
    }

    // Listen for the next GPVTG string next
    correctNmeaType = false;
    strcpy(nmeaType, "GPVTG");
    i -= 1; // Erase the carriage return
    
    // While the current sentence type is not needed sentence type
    while(!correctNmeaType)
    {
        // Wait for the start of a NMEA sentence
        while(gps.read() != '$');

        // Create a buffer for the NMEA sentence format
        // and then set the last character of the buffer
        // to a null character
        char serbuf[6];
        serbuf[5] = 0;

        // Put NMEA sentence format into buffer 
        for (int i=0; i<5; i++)
        {
            while (!gps.available());
            serbuf[i] = gps.read();
        }

        // Compare buffer with the specified NMEA type
        // above and put the result in 'matches'
        if((strncmp(nmeaType,serbuf,5) == 0) && (strlen(serbuf) >= 5))
            correctNmeaType = true;
    }

    // Place the rest of the sentence (upto the CR) into a buffer
    while(data[i-1] != 13)
    {
        while(!gps.available());
        data[i] = gps.read();
        i++;
    }
    
    // End the software serial session to the GPS
    gps.end();

    // Null terminate the string
    data[i] = 0;

    // Check for GPS Fix
    if(data[37] == '1' || data[37] == '2')
    {
        // Create a char array for the parsed data
        char parsed[50]; 
        sprintf(parsed,"");

        // Parse data
        gps_strcpy(data,parsed+strlen(parsed),2,2);
        strcat(parsed,":");
        gps_strcpy(data,parsed+strlen(parsed),4,2);
        strcat(parsed,":");
        gps_strcpy(data,parsed+strlen(parsed),6,2);
        strcat(parsed,",");
        // Prepend a '-' if the latitude is S
        if(data[22] == 'S') strcat(parsed,"-");
        gps_strcpy(data,parsed+strlen(parsed),12,9);
        strcat(parsed,",");
        // Prepend a '-' if the longitude is W
        if(data[35] == 'W') strcat(parsed,"-");
        gps_strcpy(data,parsed+strlen(parsed),24,10);
        strcat(parsed,",");
        gps_strcpy(data,parsed+strlen(parsed),47,5);
        strcat(parsed,",");
        gps_strcpy(data,parsed+strlen(parsed),39,2);
        strcat(parsed,",");
        gps_strcpy(data,parsed+strlen(parsed),90,5);

        // Return the parsed data
        return parsed;
    }
    // If there is no fix, return empty fields
    else
    {
        return ",,,,,";
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
        // and then set the last character of the buffer
        // to a null character
        char serbuf[6];
        serbuf[5] = '\0';

        // Put NMEA sentence format into buffer 
        for (int i=0; i<5; i++)
        {
            while (!gps.available());
            serbuf[i] = gps.read();
        }

        // Compare buffer with the specified NMEA type
        if((strncmp(nmea,serbuf,5) == 0) && (strlen(serbuf) >= 5)) correct = true;
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

char* gps_altitude()
{
    // Software serial
    SoftwareSerial gps(GPS_RX, GPS_TX);

    // Begin software serial at 4800 baud
    gps.begin(4800);
    // Set GPS as active software serial device
    gps.listen();

    // Boolean to store whether we have the correct NMEA 
    // sentence type currently
    bool correctNmeaType = false;

    // The NMEA sentence type needed (GPGGA)
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
        if((strncmp(nmeaType,serbuf,5) == 0) && (strlen(serbuf) >= 5))
            correctNmeaType = true;
    }

    // Create a char array to contain the rest of the sentence
    char data[90];
    data[0] = 32; // Place a space in the first item
    uint8_t i = 1;

    // Place the rest of the sentence (upto the CR) into a buffer
    while(data[i-1] != 13)
    {
        while(!gps.available()) {}
        data[i] = gps.read();
        i++;
    }

    // End the software serial session to the GPS
    gps.end();

    // Null terminate the string
    data[i] = 0;

    // Check for GPS Fix
    if(data[37] == '1' || data[37] == '2')
    {
        // Create a char array for the parsed data
        char alt[6];
        sprintf(alt,"");

        // Parse data
        gps_strcpy(data,alt+strlen(alt),47,5);

        // Return the parsed data
        return alt;
    }
    // If there is no fix, return empty fields
    else
    {
        return "00000";
    }
}

void gps_strcpy(char* str, char* dest, int pos, int len)
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
