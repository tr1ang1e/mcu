/*
    Use ESP8266 in a station mode
    For testing:
        $ ping <esp-IP>     // will be printed to the serial monitor 
*/


#include <Arduino.h>
#include <Esp.h>
#include <ESP8266WiFi.h>
#include <ESP8266WiFiMulti.h>


/* --------------------------------------------------------- */
/*                   H E L P   M A C R O S                   */
/* --------------------------------------------------------- */

#define __PINFO(_1, _2)  Serial.print(_1); Serial.println(_2);


/* --------------------------------------------------------- */
/*                   S T A T I C   D A T A                   */
/* --------------------------------------------------------- */

// access points preferences >> the strongest one will be chosen
// ESP8266WiFiMulti::addAP(ssid, password):
//    ssid = service set identifier
//    password = access point password
ESP8266WiFiMulti wifiMulti;

// station network settings
IPAddress ip(192, 168, 1, 101);             // local IP address
IPAddress gateway(192, 168, 1, 1);          // see local network gateway
IPAddress mask(255, 255, 255, 0);           // subnet mask


/* --------------------------------------------------------- */
/*             S T A T I C   F U N C T I O N S               */
/* --------------------------------------------------------- */

// setup functions
static void set_access_points_preferences();
static void set_serial_communication(int baudrate);

// utilities
static void blink(bool startLevel, int delayMs);
static bool connect_access_point(int attempts, int attemptDelay);         // @attempts: negative for endless tries


/* --------------------------------------------------------- */
/*                        M A I N                            */
/* --------------------------------------------------------- */

void setup() 
{
    bool result;
    set_serial_communication(9600);

    // configure pin so that LED will indicates work
    pinMode(LED_BUILTIN, OUTPUT);
    digitalWrite(LED_BUILTIN, LOW);

    // set esp network settings
    result = WiFi.config(ip, gateway, mask);
    if (!result)
    {
        __PINFO("Failed to set custom ESP station network properties. ", "Defaults are used")
    }

    // connect to existent access point
    set_access_points_preferences();
    result = connect_access_point(-1 , 200);
    if (!result)
    {
        __PINFO("Failed to connect to access points. ", "")
    }

    __PINFO("Connected to = ", WiFi.SSID());
    __PINFO("ESP info", " = ")
    __PINFO("  MAC address : ", WiFi.macAddress());
    __PINFO("  local IP    : ", WiFi.localIP());
}

void loop() 
{
    // nothing to do
}


/* --------------------------------------------------------- */
/*             S T A T I C   F U N C T I O N S               */
/* --------------------------------------------------------- */

void set_access_points_preferences()
{
    wifiMulti.addAP("Keenetic-4349", "UDWMpKf6" );
    wifiMulti.addAP("MDC_5GHz", "ypswa5WCJ");
}

void set_serial_communication(int baudrate)
{
    Serial.begin(baudrate);
    while(!Serial)
    {
    }
    Serial.println("Serial port is opened");
}

void blink(bool startLevel, int delayMs)
{
    digitalWrite(LED_BUILTIN, startLevel);
    delay(delayMs);
    digitalWrite(LED_BUILTIN, !startLevel);
    delay(delayMs);
}

bool connect_access_point(int attempts, int attemptDelay)
{
    bool result = false;
    wl_status_t status;
    
    bool endless = (attempts < 0);
    while (endless || attempts--)
    {
        status = wifiMulti.run();
        if (status == WL_CONNECTED)
        {
          result = true;
          break;
        }
        blink(HIGH, attemptDelay / 2);
    }

    return result;
}
