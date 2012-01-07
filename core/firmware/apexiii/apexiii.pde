/**
 * apexiii.pde
 *
 * Part of the Apex III project
 * http://www.apexhab.org/apex-iii/
 *
 * Priyesh Patel
 *
 * (c) Copyright ApexHAB 2011
 * team@apexhab.org
 */

// Include header files
#include "counter.h"
#include "temperature.h"
#include "gps.h"
#include "rtty.h"
#include "sdlogger.h"
#include "battery.h"

// Define constants (other pin numbers are defined in their respective header files
#define STATUS_LED_PIN 13

// Addresses of sensors
byte ext_temp_addr[8] = {0x28, 0x13, 0xF7, 0x73, 0x03, 0x00, 0x00, 0x2F};
byte int_temp_addr[8] = {0x28, 0x35, 0x8C, 0x5E, 0x03, 0x00, 0x00, 0xDB};

// Define packet variable
char packet[200];

void setup()
{
    // Setup serial
    Serial.begin(9600);

    Serial.println("/------------\\");
    Serial.println("| Apex Alpha |");
    Serial.println("\\------------/");

    // Initialise status LED and then turn it on
    pinMode(STATUS_LED_PIN,OUTPUT);
    digitalWrite(STATUS_LED_PIN,HIGH);

    // Initialise radio module
    rtty_init();

    // System booted
    Serial.println("");
    Serial.println("Apex Alpha successfully booted");
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
    sprintf(packet,rtty_prepare(packet));

    // Print packet to serial
    Serial.print(packet);

    // Write packet to SD card
    sdlogger_log(packet); 

    // Telemetry
    Serial.print("Telemetry started... ");

    // Send the packet with RTTY
    // @ 300 baud - preamble then 3 times
    rtty_preamble(1);
    rtty_tx("$$ALPHA\r\n", 1);
    rtty_tx(packet, 1);
    rtty_tx(packet, 1);
    rtty_tx(packet, 1);
    // @ 50 baud - preamble then 2 times
    rtty_preamble(0);
    rtty_tx("$$ALPHA\r\n", 0);
    rtty_tx(packet, 0);
    rtty_tx(packet, 0);

    Serial.println("finished");
    
    // Delay until the next packet
    // This window is also for UART commands to be entered in
    delay(4000);

    // Check for any inputted UART commands
    uart_commands();
}
/**
 * Build the packet
 */
void build_packet()
{
    // External temperature sensor
    char et[10];
    dtostrf(temperature_get(TEMPERATURE_PIN,ext_temp_addr),4,2,et);
    // Internal temperature sensor
    char it[10];
    dtostrf(temperature_get(TEMPERATURE_PIN,int_temp_addr),4,2,it);

    // Battery voltage
    char bv[10];
    dtostrf(battery_get_voltage(),4,2,bv);

    // Build the packet
    //sprintf(packet,"$$ALPHA,%u,%s,%s,%s",counter_get(),et,it,bv); // No GPS!
    sprintf(packet,"$$ALPHA,%u,%s,%s,%s,%s",counter_get(),gps_get(),et,it,bv);
}

/**
 * Check for any commands via UART 
 */
void uart_commands()
{
    if(Serial.available() >= 4)
    {
        // A $ signifies the start of a 3 character command
        while((Serial.available() > 0) && (Serial.read() != '$')) {}

        if(Serial.available() >= 3)
        {
            char data[4];
            for(int i=0; i<3; i++)
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
    if(strcmp(cmd,"RTC") == 0)
    {
        counter_reset();
    }
}
