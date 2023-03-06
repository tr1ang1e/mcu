/*
    Use ESP8266 as an access point
    For testing:
        > coonect to WiFi access point with the name below
        $ ping <esp-IP>     // will be printed to the seral monitor 
*/


#include <Arduino.h>
#include <Esp.h>
#include <ESP8266WiFi.h>


// access point preferences
const char* ssid = "esp8266";         // service set identifier
const char* password = "password";    // use empty string "" to provide an open network


void setup() 
{
    // set serial port communication
    Serial.begin(9600);
    while(!Serial)
    {
    }
    Serial.println("Serial port is opened");

    // create access point
    WiFi.softAP(ssid, password);
    Serial.println("Access point is started");
    Serial.println(WiFi.softAPIP());


    // configure pins
    pinMode(LED_BUILTIN, OUTPUT);
    digitalWrite(LED_BUILTIN, LOW);
}


void loop() 
{
    // nothing to do
}
