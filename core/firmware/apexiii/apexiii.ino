/**
 * apexiii.ino
 *
 * Part of the Apex III project
 * http://www.apexhab.org/apex-iii/
 *
 * Priyesh Patel
 *
 * (c) Copyright ApexHAB 2011
 * team@apexhab.org
 */

// Include libraries
#include <SD.h>

// Include header files
#include "counter.h"
#include "temperature.h"
#include "gps.h"
#include "rtty.h"
#include "battery.h"
#include "modules.h"
#include "protocol.h"

// Define settings
uint8_t STATUS_LED_PIN = 13;
uint8_t EXT_TEMP_ADDR[8] = {0x28, 0xF5, 0x8B, 0x3E, 0x03, 0x00, 0x00, 0x85};
uint8_t INT_TEMP_ADDR[8] = {0x28, 0xEF, 0xC6, 0x5E, 0x03, 0x00, 0x00, 0x84};
char SD_LOG_FILENAME[] = "APEXIII.LOG";
char SD_GPS_FILENAME[] = "GPS.LOG";
uint8_t SD_CS = 10;

// Define packet variable
char packet[200];
// Current altitude
char altitude_c[6] = "";
uint16_t altitude_i = 0x0000;
// Current command
commands command = NONE;

void setup()
{
    // Setup serial
    Serial.begin(9600);

    Serial.println();
    Serial.println("/------------\\");
    Serial.println("|  Apex III  |");
    Serial.println("\\------------/");

    // Initialise status LED and then turn it on
    pinMode(STATUS_LED_PIN,OUTPUT);
    digitalWrite(STATUS_LED_PIN,HIGH);

    // Initialise radio module
    rtty_init();

    // Initialise the SD card
    pinMode(SD_CS, OUTPUT);
    if (!SD.begin(SD_CS)) Serial.println("SD card failed to initialise");

    // System booted
    Serial.println("");
    Serial.println("Apex III successfully booted");
    Serial.println("");

    // Turn status LED off
    digitalWrite(STATUS_LED_PIN,LOW);

    // Blank line to serial
    Serial.println("");
}

void loop()
{
    sdcard_gps();

    // Increment the counter
    counter_inc();

    // Get altitude
    strcpy(altitude_c, gps_altitude());
    altitude_i = atoi(altitude_c);

    // Get data from external sensors and devices and then construct the packet
    build_packet();

    // Store sent packet and prepare packet
    sprintf(packet,rtty_prepare(packet));

    // Print packet to serial
    Serial.print(packet);

    // Write packet to SD card
    sdcard_log(packet); 

    sdcard_gps();

    // Telemetry
    Serial.print("Telemetry started... ");

    delay(200);

    sdcard_gps();

    // Send the packet with RTTY
    // @ 300 baud - preamble then 3 times
    rtty_preamble(1);
    rtty_tx("$$APEXIII\r\n", 1);
    rtty_tx(packet, 1);
    rtty_tx(packet, 1);
    rtty_tx(packet, 1);

    sdcard_gps();

    // @ 50 baud - preamble then 1 time
    rtty_preamble(0);
    rtty_tx("$$APEXIII\r\n", 0);
    rtty_tx(packet, 0);

    Serial.println("finished");
    
    sdcard_gps();

    // Delay until the next packet
    // This window is also for UART commands to be entered in
    delay(2000);
    sdcard_gps();
    delay(2000);

    // Check for any inputted UART commands
    uart_commands();
}

/**
 * Build the packet
 */
void build_packet()
{
    // Broadcast to modules
    modules_broadcast(altitude_i, command);
    command = NONE;

    // External temperature sensor
    char et[10];
    dtostrf(temperature_get(EXT_TEMP_ADDR),4,2,et);
    // Internal temperature sensor
    char it[10];
    dtostrf(temperature_get(INT_TEMP_ADDR),4,2,it);

    // Battery voltage
    char bv[10];
    dtostrf(battery_get_voltage(),4,2,bv);

    // Build the packet
    sprintf(packet,"$$APEXIII,%u,%s,%s,%s,%s",counter_get(),gps_get(),et,it,bv);
}

/**
 * Write sentence to SD card log 
 */
void sdcard_log(char* sentence)
{
    File logFile = SD.open(SD_LOG_FILENAME, FILE_WRITE);
    
    if (logFile)
    {
        logFile.print(sentence);
    }

    logFile.close();
}

/**
 * Write GPS data to SD card
 */
void sdcard_gps()
{
    File logFile = SD.open(SD_GPS_FILENAME, FILE_WRITE);
    
    if (logFile)
    {
        char buffer_gpgga[90];
        gps_nmea(buffer_gpgga, "GPGGA");
        logFile.println(buffer_gpgga);
        char buffer_gpvtg[90];
        gps_nmea(buffer_gpvtg, "GPVTG"); 
        logFile.println(buffer_gpvtg);
    }

    logFile.close();
}

/**
 * Check for any commands via UART 
 */
void uart_commands()
{
    if(Serial.available() >= 4)
    {
        // A $ signifies the start of a 3 character command
        while ((Serial.available() > 0) && (Serial.read() != '$')) {}

        if (Serial.available() >= 3)
        {
            char data[4];
            for (int i=0; i<3; i++)
            {
                data[i] = Serial.read();
            }
            data[3] = 0;
            
            uart_commands_parse(data);
        }
    }

    Serial.flush();
}

/**
 * Parse UART command 
 */
void uart_commands_parse(char* cmd)
{
    if (strcmp(cmd, "RTC") == 0)
    {
        counter_reset();
    }
    else if (strcmp(cmd, "CDN") == 0)
    {
        command = CDWN;
    }
    else if (strcmp(cmd, "SN0") == 0)
    {
        command = SRN0;
    }
    else if (strcmp(cmd, "SN1") == 0)
    {
        command = SRN1;
    }
    else if (strcmp(cmd, "PNG") == 0)
    {
        command = PING;
    }
    else if (strcmp(cmd, "ALT") == 0)
    {
        command = ALTR;
    }
}
