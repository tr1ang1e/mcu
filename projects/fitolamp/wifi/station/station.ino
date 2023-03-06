/*
    Use ESP8266 in a station mode
    For testing:
        $ ping <esp-IP>     // will be printed to the seral monitor 
*/


#include <Arduino.h>
#include <Esp.h>
#include <ESP8266WiFi.h>


/* --------------------------------------------------------- */
/*                   H E L P   M A C R O S                   */
/* --------------------------------------------------------- */

#define __PINFO(_1, _2)  Serial.print(_1); Serial.println(_2);


/* --------------------------------------------------------- */
/*                   S T A T I C   D A T A                   */
/* --------------------------------------------------------- */

// access point preferences
const char* ssid = "Keenetic-4349";         // service set identifier
const char* password = "UDWMpKf6";          // access point password

// station network settings
IPAddress ip(192, 168, 1, 101);             // local IP address
IPAddress gateway(192, 168, 1, 1);          // see local network gateway
IPAddress mask(255, 255, 255, 0);           // subnet mask


/* --------------------------------------------------------- */
/*             S T A T I C   F U N C T I O N S               */
/* --------------------------------------------------------- */

static void blink(bool startLevel, int delayMs);


/* --------------------------------------------------------- */
/*                        M A I N                            */
/* --------------------------------------------------------- */

void setup() 
{
    // set serial port communication
    Serial.begin(9600);
    while(!Serial)
    {
    }
    Serial.println("Serial port is opened");

    // configure pin so that LED will indicates work
    pinMode(LED_BUILTIN, OUTPUT);
    digitalWrite(LED_BUILTIN, LOW);

    // set esp network settings
    bool result = WiFi.config(ip, gateway, mask);
    if (!result)
    {
        __PINFO("Failed to set custom ESP station network properties. ", "Drefaults are used")
    }

    // conect to existent access point
    WiFi.begin(ssid, password);
    while (WiFi.status() != WL_CONNECTED)
    {
        blink(HIGH, 100);
    }
    __PINFO("Connected to: ", ssid);

    __PINFO("ESP info", " = ")
    __PINFO("  local IP: ", WiFi.localIP());
}

void loop() 
{
    // nothing to do
}


/* --------------------------------------------------------- */
/*             S T A T I C   F U N C T I O N S               */
/* --------------------------------------------------------- */

static void blink(bool startLevel, int delayMs)
{
    digitalWrite(LED_BUILTIN, startLevel);
    delay(delayMs);
    digitalWrite(LED_BUILTIN, !startLevel);
    delay(delayMs);
}