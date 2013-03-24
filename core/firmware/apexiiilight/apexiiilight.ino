/**
 * apexiii.ino
 *
 * Part of the Apex III project
 * http://www.apexhab.org/apex-iii/
 *
 * Priyesh Patel
 *
 * (c) Copyright ApexHAB 2013
 * team@apexhab.org
 */

// Include header files
#include "counter.h"
#include "temperature.h"
#include "gps.h"
#include "rtty.h"
#include "battery.h"

// Define settings
uint8_t STATUS_LED_PIN = 13;
uint8_t EXT_TEMP_ADDR[8] = {0x28, 0x8D, 0x0C, 0x68, 0x03, 0x00, 0x00, 0xF7};
uint8_t INT_TEMP_ADDR[8] = {0x28, 0x42, 0x99, 0x5E, 0x03, 0x00, 0x00, 0x68};

// Define packet
char packet[200];
// Last GPS string with fix
char gps_lastfix[65] = ",,,,";

void setup()
{
    // Setup serial
    Serial.begin(9600);

    Serial.println();
    Serial.println("--------------");
    Serial.println("|  Apex III  |");
    Serial.println("|  Light     |");
    Serial.println("--------------");
    Serial.println("");

    // Initialise status LED and then turn it on
    pinMode(STATUS_LED_PIN,OUTPUT);
    digitalWrite(STATUS_LED_PIN,HIGH);

    // Initialise radio module
    rtty_init();

    // Initialise GPS last fix string 
    gps_get(gps_lastfix);

    // System booted
    Serial.println("");
    Serial.println("Apex III booted");
    Serial.println("");

    // Turn status LED off
    digitalWrite(STATUS_LED_PIN,LOW);

    // Blank line to serial
    Serial.println("");
}

void loop()
{
    // Increment the counter
    counter_inc();

    // Get data from external sensors and devices and then construct the packet
    build_packet();

    // Store sent packet and prepare packet
    rtty_prepare(packet);

    // Print packet to serial
    Serial.print(packet);
    delay(100);

    // @ 50 baud - preamble then 1 time
    rtty_preamble(0);
    rtty_tx(packet, 0);
}

/**
 * Build the packet
 */
void build_packet()
{
    // External temperature sensor
    char external_temp[10];
    dtostrf(temperature_get(EXT_TEMP_ADDR), 4, 2, external_temp);
    // Internal temperature sensor
    char internal_temp[10];
    dtostrf(temperature_get(INT_TEMP_ADDR), 4, 2, internal_temp);

    // Battery voltage
    char battery_voltage[10];
    dtostrf(battery_get_voltage(), 4, 2, battery_voltage);

    // GPS 
    char gps_data[65];
    gps_get(gps_data); 
    // If no fix, return last fix
    if (strcmp(gps_data, ",,,,") == 0)
    {
        strcpy(gps_data, gps_lastfix);
    }
    else
    {
        strcpy(gps_lastfix, gps_data);
    }

    // Build the packet
    sprintf(packet,"$$APEX,%u,%s,%s,%s,%s",
        counter_get(),
        gps_data,
        external_temp,
        internal_temp,
        battery_voltage
    );
}
